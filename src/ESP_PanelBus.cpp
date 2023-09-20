/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>

#include "esp_lcd_panel_io.h"

#include "private/CheckResult.h"
#include "ESP_PanelBus.h"

static const char *TAG = "ESP_PanelBus";

ESP_PanelBus::ESP_PanelBus():
    handle(NULL),
    ctx({NULL, NULL}),
    cbs(NULL)
{
}

void ESP_PanelBus::readParam(int lcd_cmd, void *param, size_t param_size)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_io_rx_param(handle, lcd_cmd, param, param_size));
}

void ESP_PanelBus::writeParam(int lcd_cmd, const void *param, size_t param_size)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_io_tx_param(handle, lcd_cmd, param, param_size));
}

void ESP_PanelBus::writeColor(int lcd_cmd, const void *color, size_t color_size)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_io_tx_param(handle, lcd_cmd, color, color_size));
}

void ESP_PanelBus::del(void)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_io_del(handle));
}

void ESP_PanelBus::setCallback(const ESP_PanelBusCallback_t cbs, void *user_ctx)
{
    this->cbs = cbs;
    ctx.user_ctx = user_ctx;
}

bool ESP_PanelBus::callback(void *panel_io, void *edata, void *user_ctx)
{
    ESP_CallbackCtx_t *callback_ctx = (ESP_CallbackCtx_t *)user_ctx;
    ESP_PanelBus *bus = (ESP_PanelBus *)callback_ctx->bus;

    if (bus->cbs) {
        return bus->cbs(callback_ctx->user_ctx);
    }
    return false;
}

esp_lcd_panel_io_handle_t ESP_PanelBus::getHandle(void)
{
    CHECK_NULL_GOTO(handle, err);
    return handle;

err:
    return NULL;
}
