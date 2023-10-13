/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "ESP_PanelLcdTouch.h"

class ESP_PanelLcdTouch_CST816S : public ESP_PanelLcdTouch {
public:
    ESP_PanelLcdTouch_CST816S(ESP_PanelBus *bus, const esp_lcd_touch_config_t *config): ESP_PanelLcdTouch(bus, config) { };
    ESP_PanelLcdTouch_CST816S(ESP_PanelBus *bus, uint16_t width, uint16_t height): ESP_PanelLcdTouch(bus, width, height) { };
    ~ESP_PanelLcdTouch_CST816S() override;

    void begin(void) override;
};

/**
 * @brief I2C address of the CST816S controller
 *
 */
#define ESP_LCD_TOUCH_IO_I2C_CST816S_ADDRESS    (0x15)

/**
 * @brief Touch IO configuration structure
 *
 */
#define ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG()               \
    {                                                       \
        .dev_addr = ESP_LCD_TOUCH_IO_I2C_CST816S_ADDRESS,   \
        .on_color_trans_done = NULL,                        \
        .user_ctx = NULL,                                   \
        .control_phase_bytes = 1,                           \
        .dc_bit_offset = 0,                                 \
        .lcd_cmd_bits = 8,                                  \
        .lcd_param_bits = 0,                                \
        .flags =                                            \
        {                                                   \
            .dc_low_on_data = 0,                            \
            .disable_control_phase = 1,                     \
        }                                                   \
    }
