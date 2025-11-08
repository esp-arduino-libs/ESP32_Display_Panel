/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file   esp_lcd_touch_cst9217.h
 * @brief  ESP LCD touch: CST9217
  */

#pragma once

#include "esp_lcd_touch.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ESP_LCD_TOUCH_CST9217_VER_MAJOR    (1)
#define ESP_LCD_TOUCH_CST9217_VER_MINOR    (0)
#define ESP_LCD_TOUCH_CST9217_VER_PATCH    (0)

/**
 * @brief Create a new CST9217 touch driver
 *
 * @note  The I2C communication should be initialized before use this function.
 *
 * @param io LCD panel IO handle, it should be created by `esp_lcd_new_panel_io_i2c()`
 * @param config Touch panel configuration
 * @param tp Touch panel handle
 * @return
 *      - ESP_OK: on success
 */
esp_err_t esp_lcd_touch_new_i2c_cst9217(const esp_lcd_panel_io_handle_t io, const esp_lcd_touch_config_t *config, esp_lcd_touch_handle_t *tp);

/**
 * @brief I2C address of the CST9217 controller
 *
 */
#define ESP_LCD_TOUCH_IO_I2C_CST9217_ADDRESS    (0x5A)

/**
 * @brief Touch IO configuration structure
 *
 */
#define ESP_LCD_TOUCH_IO_I2C_CST9217_CONFIG()             \
    {                                                    \
        .dev_addr = ESP_LCD_TOUCH_IO_I2C_CST9217_ADDRESS, \
        .control_phase_bytes = 1,                        \
        .dc_bit_offset = 0,                              \
        .lcd_cmd_bits = 8,                              \
        .flags =                                         \
        {                                                \
            .disable_control_phase = 1,                  \
        }                                                \
    }

/**
 * @brief Touch IO configuration structure with input address
 *
 * @param[in] addr I2C address of the touch panel
 *
 */
#define ESP_LCD_TOUCH_IO_I2C_CST9217_CONFIG_WITH_ADDR(addr) \
    {                                       \
        .dev_addr = addr,                   \
        .control_phase_bytes = 1,           \
        .dc_bit_offset = 0,                 \
        .lcd_cmd_bits = 8,                 \
        .flags =                            \
        {                                   \
            .disable_control_phase = 1,     \
        }                                   \
    }

#ifdef __cplusplus
}
#endif
