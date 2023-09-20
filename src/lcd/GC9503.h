/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "../ESP_PanelLcd.h"

class ESP_PanelLcd_GC9503: public ESP_PanelLcd {
public:
    ESP_PanelLcd_GC9503(ESP_PanelBus *bus, const ESP_PanelLcdConfig_t *config): ESP_PanelLcd(bus, config) { };
    ESP_PanelLcd_GC9503(ESP_PanelBus *bus): ESP_PanelLcd(bus) { };
    ~ESP_PanelLcd_GC9503() override;

    void reset(void);

    void init(void) override;
};

#endif /* SOC_LCD_RGB_SUPPORTED */
