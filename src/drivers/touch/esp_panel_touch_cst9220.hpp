/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_panel_touch.hpp"
#include "port/esp_lcd_touch_cst9220.h"

namespace esp_panel::drivers {

/** CST9220-compatible touch controller, including the CST9217 in ESP-Mosaico. */
class TouchCST9220 : public Touch {
public:
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .name = "CST9220",
        .max_points_num = ESP_LCD_TOUCH_CST9220_MAX_POINTS,
    };

    TouchCST9220(Bus *bus, uint16_t width, uint16_t height, int rst_io = -1, int int_io = -1):
        Touch(BASIC_ATTRIBUTES_DEFAULT, bus, width, height, rst_io, int_io) {}
    TouchCST9220(Bus *bus, const Config &config): Touch(BASIC_ATTRIBUTES_DEFAULT, bus, config) {}
    TouchCST9220(const BusFactory::Config &bus, const Config &config): Touch(BASIC_ATTRIBUTES_DEFAULT, bus, config) {}

    ~TouchCST9220() override;
    bool begin() override;
};

} // namespace esp_panel::drivers
