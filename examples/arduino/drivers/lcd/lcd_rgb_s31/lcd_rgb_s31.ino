/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <esp_display_panel.hpp>

using esp_panel::drivers::BusRGB;
using esp_panel::drivers::LCD_ST7262;

#if !CONFIG_IDF_TARGET_ESP32S31
#error "Select ESP32S31 Dev Module for this example"
#endif

static constexpr int PANEL_WIDTH = 800;
static constexpr int PANEL_HEIGHT = 480;

static BusRGB bus(BusRGB::Config{
    .refresh_panel = BusRGB::RefreshPanelPartialConfig{
        .pclk_hz = 20 * 1000 * 1000,
        .h_res = PANEL_WIDTH,
        .v_res = PANEL_HEIGHT,
        .hsync_pulse_width = 1,
        .hsync_back_porch = 40,
        .hsync_front_porch = 20,
        .vsync_pulse_width = 1,
        .vsync_back_porch = 10,
        .vsync_front_porch = 5,
        .data_width = 24,
        .bits_per_pixel = 24,
        .hsync_gpio_num = 44,
        .vsync_gpio_num = 45,
        .de_gpio_num = 43,
        .pclk_gpio_num = 40,
        .data_gpio_nums = {
            8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
            33, 34, 35, 36, 37, 38, 39, 2, 3, 4, 5, 7,
        },
        .flags_pclk_active_neg = true,
    },
});
static LCD_ST7262 lcd(&bus, PANEL_WIDTH, PANEL_HEIGHT, 24, -1);

void setup()
{
    Serial.begin(115200);
    if (!lcd.begin()) {
        Serial.println("ST7262 initialization failed");
        return;
    }
    if (!lcd.colorBarTest()) {
        Serial.println("ST7262 color bar failed");
        return;
    }
    Serial.printf("ST7262 color bars: %d x %d, %d bpp\n",
                  lcd.getFrameWidth(), lcd.getFrameHeight(), lcd.getFrameColorBits());
}

void loop()
{
    delay(1000);
}
