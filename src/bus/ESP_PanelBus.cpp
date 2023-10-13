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

#define PANEL_BUS_FLAGS(type, need_init)    \
    {                                       \
        .bus_type = type,                   \
        .host_need_init = need_init,        \
    }

#define PANEL_BUS_CALLBACK_CTX()            \
    {                                       \
        .bus = this,                        \
        .user_data = NULL,                  \
    }

static const char *TAG = "ESP_PanelBus";

ESP_PanelBus::ESP_PanelBus(int bus_type, bool host_need_init):
    onTransmitFinishCallback(NULL),
    callback_ctx(PANEL_BUS_CALLBACK_CTX()),
    flags(PANEL_BUS_FLAGS(bus_type, host_need_init)),
    sem_transmit_finish(NULL)
{
}

void ESP_PanelBus::readParam(int lcd_cmd, void *param, int param_size)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_io_rx_param(handle, lcd_cmd, param, param_size));
}

void ESP_PanelBus::writeParam(int lcd_cmd, const void *param, int param_size)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_io_tx_param(handle, lcd_cmd, param, param_size));
}

void ESP_PanelBus::writeColor(int lcd_cmd, const void *color, int color_size)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_io_tx_param(handle, lcd_cmd, color, color_size));
}

void ESP_PanelBus::del(void)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_io_del(handle));
}

void ESP_PanelBus::attachTransmitFinishCallback(ESP_BusCallback_t callback, void *user_data)
{
    onTransmitFinishCallback = callback;
    callback_ctx.user_data = user_data;
}

esp_lcd_panel_io_handle_t ESP_PanelBus::getHandle(void)
{
    CHECK_NULL_GOTO(handle, err);
    return handle;

err:
    return NULL;
}

int ESP_PanelBus::getType(void)
{
    return flags.bus_type;
}

void ESP_PanelBus::createTransmitFinishSemaphore(void)
{
    if (sem_transmit_finish == NULL) {
        sem_transmit_finish = xSemaphoreCreateBinary();
        CHECK_NULL_RETURN(sem_transmit_finish);
    }
}

bool ESP_PanelBus::on_transmit_finish_callback(void *panel_io, void *edata, void *user_ctx)
{
    ESP_BusUserData_t *callback_ctx = (ESP_BusUserData_t *)user_ctx;
    ESP_PanelBus *bus = (ESP_PanelBus *)callback_ctx->bus;
    BaseType_t need_yield = pdFALSE;

    if (bus->sem_transmit_finish) {
        xSemaphoreGiveFromISR(bus->sem_transmit_finish, &need_yield);
    }

    if (bus->onTransmitFinishCallback) {
        return bus->onTransmitFinishCallback(callback_ctx->user_data);
    }
    return (need_yield == pdTRUE);
}
