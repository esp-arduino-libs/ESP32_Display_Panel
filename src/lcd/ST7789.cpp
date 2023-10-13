/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "../private/CheckResult.h"
#include "base/esp_lcd_st7789.h"
#include "ST7789.h"

static const char *TAG = "st7789";

ESP_PanelLcd_ST7789::~ESP_PanelLcd_ST7789()
{
    if (handle) {
        del();
    }
}

void ESP_PanelLcd_ST7789::init(void)
{
    CHECK_NULL_RETURN(bus);
    CHECK_ERROR_RETURN(esp_lcd_new_panel_st7789(bus->getHandle(), &panel_config, &handle));
}
