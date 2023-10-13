/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "ESP_PanelLcd.h"

class ESP_PanelLcd_ST7262: public ESP_PanelLcd {
public:
    ESP_PanelLcd_ST7262(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t *panel_config): ESP_PanelLcd(bus, panel_config) { };
    ESP_PanelLcd_ST7262(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config): ESP_PanelLcd(bus, panel_config) { };
    ~ESP_PanelLcd_ST7262() override;

    void reset(void);

    void init(void) override;
};

#endif /* SOC_LCD_RGB_SUPPORTED */
