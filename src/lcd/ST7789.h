/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "../ESP_PanelLcd.h"

class ESP_PanelLcd_ST7789 : public ESP_PanelLcd {
public:
    ESP_PanelLcd_ST7789(ESP_PanelBus *bus, const ESP_PanelLcdConfig_t *config): ESP_PanelLcd(bus, config) { };
    ESP_PanelLcd_ST7789(ESP_PanelBus *bus): ESP_PanelLcd(bus) { };
    ~ESP_PanelLcd_ST7789() override;

    void init(void) override;
};
