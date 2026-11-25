/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_panel_lcd.hpp"

namespace esp_panel::drivers {

/** SPI/QSPI CO5300 display controller. */
class LCD_CO5300 : public LCD {
public:
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {.name = "CO5300"};

    LCD_CO5300(Bus *bus, int width, int height, int color_bits, int rst_io):
        LCD(BASIC_ATTRIBUTES_DEFAULT, bus, width, height, color_bits, rst_io) {}
    LCD_CO5300(Bus *bus, const Config &config): LCD(BASIC_ATTRIBUTES_DEFAULT, bus, config) {}
    LCD_CO5300(const BusFactory::Config &bus, const Config &config): LCD(BASIC_ATTRIBUTES_DEFAULT, bus, config) {}

    ~LCD_CO5300() override;
    bool init() override;

    /** Set the panel brightness to a percentage in the range 0 to 100 after begin(). */
    bool setBrightness(int percent);
};

} // namespace esp_panel::drivers
