/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ESP_PanelLcd.h"
#include "base/esp_lcd_custom_types.h"
#include "base/esp_lcd_sh8601.h"

class ESP_PanelLcd_SH8601: public ESP_PanelLcd {
public:
    ESP_PanelLcd_SH8601(ESP_PanelBus *bus, int color_bits = 16, int rst_io = -1,
                        const esp_lcd_panel_vendor_init_cmd_t init_cmd[] = NULL, int init_cmd_size = 0);
    ESP_PanelLcd_SH8601(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config);
    ~ESP_PanelLcd_SH8601() override;

    void init(void) override;
};
