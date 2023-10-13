/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "../private/CheckResult.h"
#include "base/esp_lcd_gc9a01.h"
#include "GC9A01.h"

static const char *TAG = "gc9a01";

ESP_PanelLcd_GC9A01::~ESP_PanelLcd_GC9A01()
{
    if (handle) {
        del();
    }
}

void ESP_PanelLcd_GC9A01::init(void)
{
    CHECK_NULL_RETURN(bus);
    CHECK_ERROR_RETURN(esp_lcd_new_panel_gc9a01(bus->getHandle(), &panel_config, &handle));
}
