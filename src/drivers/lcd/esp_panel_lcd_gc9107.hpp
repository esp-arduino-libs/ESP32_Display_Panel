/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_panel_lcd.hpp"

namespace esp_panel::drivers {

/** SPI GC9107 display controller. */
class LCD_GC9107 : public LCD {
public:
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {.name = "GC9107"};

    LCD_GC9107(Bus *bus, int width, int height, int color_bits, int rst_io):
        LCD(BASIC_ATTRIBUTES_DEFAULT, bus, width, height, color_bits, rst_io) {}
    LCD_GC9107(Bus *bus, const Config &config): LCD(BASIC_ATTRIBUTES_DEFAULT, bus, config) {}
    LCD_GC9107(const BusFactory::Config &bus, const Config &config): LCD(BASIC_ATTRIBUTES_DEFAULT, bus, config) {}

    ~LCD_GC9107() override;
    bool init() override;
};

} // namespace esp_panel::drivers
