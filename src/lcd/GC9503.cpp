/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "ESP_PanelLog.h"
#include "GC9503.h"

static const char *TAG = "GC9503_CPP";

ESP_PanelLcd_GC9503::ESP_PanelLcd_GC9503(ESP_PanelBus *bus, uint8_t color_bits, int rst_io):
    ESP_PanelLcd(bus, color_bits, rst_io)
{
}

ESP_PanelLcd_GC9503::ESP_PanelLcd_GC9503(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config):
    ESP_PanelLcd(bus, panel_config)
{
}

ESP_PanelLcd_GC9503::~ESP_PanelLcd_GC9503()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (handle == NULL) {
        goto end;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete device failed");
    }

end:
    ESP_LOGD(TAG, "Destroyed");
}

bool ESP_PanelLcd_GC9503::init(void)
{
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_new_panel_gc9503(bus->getHandle(), &panel_config, &handle), false, "Create panel failed");

    return true;
}
#endif
