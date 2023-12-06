/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "ESP_PanelPrivate.h"
#include "ST7701.h"

static const char *TAG = "ST7701";

ESP_PanelLcd_ST7701::ESP_PanelLcd_ST7701(ESP_PanelBus *bus, int color_bits, int rst_io,
        const esp_lcd_panel_vendor_init_cmd_t init_cmd[], int init_cmd_size):
    ESP_PanelLcd(bus, color_bits, rst_io, init_cmd, init_cmd_size)
{
}

ESP_PanelLcd_ST7701::ESP_PanelLcd_ST7701(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config):
    ESP_PanelLcd(bus, panel_config)
{
}

ESP_PanelLcd_ST7701::~ESP_PanelLcd_ST7701()
{
    if (handle) {
        del();
    }
}

void ESP_PanelLcd_ST7701::init(void)
{
    CHECK_NULL_RETURN(bus);
    CHECK_ERROR_RETURN(esp_lcd_new_panel_st7701(bus->getHandle(), &panel_config, &handle));
}

#endif
