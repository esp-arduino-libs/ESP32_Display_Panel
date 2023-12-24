/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "ESP_PanelLcdTouch.h"

class ESP_PanelLcdTouch_STMPE610 : public ESP_PanelLcdTouch {
public:
    ESP_PanelLcdTouch_STMPE610(ESP_PanelBus *bus, const esp_lcd_touch_config_t &config): ESP_PanelLcdTouch(bus, config) { };
    ESP_PanelLcdTouch_STMPE610(ESP_PanelBus *bus, uint16_t width, uint16_t height): ESP_PanelLcdTouch(bus, width, height) { };
    ~ESP_PanelLcdTouch_STMPE610() override;

    void begin(void) override;
};

/**
 * @brief Recommended clock for SPI read of the STMPE610
 *
 */
#define ESP_LCD_TOUCH_SPI_CLOCK_HZ   (1 * 1000 * 1000)

/**
 * @brief Communication SPI device IO structure
 *
 */
#define ESP_LCD_TOUCH_IO_SPI_STMPE610_CONFIG(touch_cs)  \
    {                                                   \
        .cs_gpio_num = touch_cs,                        \
        .dc_gpio_num = -1,                              \
        .spi_mode = 1,                              \
        .pclk_hz = ESP_LCD_TOUCH_SPI_CLOCK_HZ,      \
        .trans_queue_depth = 1,                    \
        .lcd_cmd_bits = 8,                          \
        .lcd_param_bits = 8,                        \
                                                           \
        .spi_mode = 0,                                          \
        .pclk_hz = SPI_MASTER_FREQ_40M,                         \
        .trans_queue_depth = 10,                                \
        .on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)cb, \
        .user_ctx = cb_ctx,                                     \
        .lcd_cmd_bits = 8,                                      \
        .lcd_param_bits = 8,                                    \
        .flags = {                                              \
            .dc_as_cmd_phase = 0,                               \
            .dc_low_on_data = 0,                                \
            .octal_mode = 0,                                    \
            .lsb_first = 0,                                     \
        },                                                      \
    }
