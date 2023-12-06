/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>
#include "esp_lcd_panel_io.h"
#include "ESP_PanelPrivate.h"
#include "ESP_PanelBus.h"

static const char *TAG = "ESP_PanelBus";

ESP_PanelBus::ESP_PanelBus(uint8_t host_type, bool host_need_init):
    host_need_init(host_need_init),
    host_type((uint8_t)host_type),
    handle(NULL)
{
}

void ESP_PanelBus::init(void)
{
    ENABLE_TAG_PRINT_DEBUG_LOG();
}

bool ESP_PanelBus::readCommandParameter(int lcd_cmd, void *param, int param_size)
{
    CHECK_NULL_RET(handle, false, "Invalid handle, need to call `begin()` first");
    CHECK_ERR_RET(esp_lcd_panel_io_rx_param(handle, lcd_cmd, param, param_size), false,
                  "Read command parameter failed, cmd: 0x%02x", lcd_cmd);

    return true;
}

bool ESP_PanelBus::writeCommandParameter(int lcd_cmd, const void *param, int param_size)
{
    CHECK_NULL_RET(handle, false, "Invalid handle, need to call `begin()` first");
    CHECK_ERR_RET(esp_lcd_panel_io_tx_param(handle, lcd_cmd, param, param_size), false,
                  "Write command parameter failed, cmd: 0x%02x", lcd_cmd);

    return true;
}

bool ESP_PanelBus::writeCommandColor(int lcd_cmd, const void *color, int color_size)
{
    CHECK_NULL_RET(handle, false, "Invalid handle, need to call `begin()` first");
    CHECK_ERR_RET(esp_lcd_panel_io_tx_param(handle, lcd_cmd, color, color_size), false,
                  "Write command color failed, cmd: 0x%02x", lcd_cmd);

    return true;
}

bool ESP_PanelBus::del(void)
{
    CHECK_NULL_RET(handle, false, "Invalid handle, need to call `begin()` first");
    CHECK_ERR_RET(esp_lcd_panel_io_del(handle), false, "Delete panel io failed");

    ESP_LOGI(TAG, "Delete panel io @ %p", handle);

    return true;
}

esp_lcd_panel_io_handle_t ESP_PanelBus::getHandle(void)
{
    if (handle == NULL) {
        ESP_LOGD(TAG, "Invalid handle, need to call `begin()` first");
    }

    return handle;
}

uint8_t ESP_PanelBus::getType(void)
{
    return host_type;
}
