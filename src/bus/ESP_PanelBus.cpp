/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>
#include "ESP_PanelLog.h"
#include "esp_lcd_panel_io.h"
#include "ESP_PanelBus.h"

#define FLAGS_DEFAULT(host_init)     \
    {                                \
        .host_need_init = host_init, \
        .del_skip_panel_io = 0,      \
    }

static const char *TAG = "ESP_PanelBus";

ESP_PanelBus::ESP_PanelBus(int host_id, uint8_t bus_type, bool host_need_init):
    flags(FLAGS_DEFAULT(host_need_init)),
    host_id(host_id),
    bus_type(bus_type),
    handle(NULL)
{
}

bool ESP_PanelBus::readRegisterData(uint32_t address, void *data, uint32_t data_size)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_io_rx_param(handle, address, data, data_size), false,
                            "Read register(0x%" PRIx32 ") failed", address);

    return true;
}

bool ESP_PanelBus::writeRegisterData(uint32_t address, const void *data, uint32_t data_size)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_io_tx_param(handle, address, data, data_size), false,
                            "Read register(0x%" PRIx32 ") failed", address);

    return true;
}

bool ESP_PanelBus::writeColorData(uint32_t address, const void *color, uint32_t color_size)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_io_tx_param(handle, address, color, color_size), false,
                            "Read register(0x%" PRIx32 ") failed", address);

    return true;
}

bool ESP_PanelBus::del(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    // RGB bus which needs to initialize the host inside and not skip panel IO can be deleted
    if ((bus_type == ESP_PANEL_BUS_TYPE_RGB) && (!flags.host_need_init || flags.del_skip_panel_io)) {
        ESP_LOGD(TAG, "Use RGB bus without host init or enable skip panel IO, skip delete panel IO");
        goto end;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_io_del(handle), false, "Delete panel IO failed");
    ESP_LOGD(TAG, "Delete panel IO @%p", handle);

end:
    handle = NULL;

    return true;
}

uint8_t ESP_PanelBus::getType(void)
{
    if ((bus_type == ESP_PANEL_BUS_TYPE_UNKNOWN) || (bus_type >= ESP_PANEL_BUS_TYPE_MAX)) {
        ESP_PANEL_ENABLE_TAG_DEBUG_LOG();
        ESP_LOGD(TAG, "Get invalid type");

        bus_type = ESP_PANEL_BUS_TYPE_UNKNOWN;
    }

    return bus_type;
}
