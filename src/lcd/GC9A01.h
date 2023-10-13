/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "ESP_PanelLcd.h"
#include "base/esp_lcd_gc9a01.h"

class ESP_PanelLcd_GC9A01: public ESP_PanelLcd {
public:
    ESP_PanelLcd_GC9A01(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t *panel_config): ESP_PanelLcd(bus, panel_config) { };
    ESP_PanelLcd_GC9A01(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config): ESP_PanelLcd(bus, panel_config) { };
    ESP_PanelLcd_GC9A01(ESP_PanelBus *bus, int bits_per_pixel = 16, int rst_io = -1, lcd_init_cmd_t init_cmd[] = NULL,
                        int init_cmd_size = 0):
        ESP_PanelLcd(bus, bits_per_pixel, rst_io, init_cmd, init_cmd_size) { };
    ESP_PanelLcd_GC9A01(ESP_PanelBus *bus): ESP_PanelLcd(bus) { };
    ~ESP_PanelLcd_GC9A01() override;

    void init(void) override;
};
