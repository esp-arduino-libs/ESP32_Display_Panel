/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_lcd_panel_rgb.h"
#include "soc/soc_caps.h"
#include "sdkconfig.h"

/**
 * @brief Panel bus type macros
 *
 */
#define ESP_PANEL_BUS_TYPE_UNKNOWN  (0)
#define ESP_PANEL_BUS_TYPE_SPI      (1)
#define ESP_PANEL_BUS_TYPE_QSPI     (2)
#define ESP_PANEL_BUS_TYPE_RGB      (3)
#define ESP_PANEL_BUS_TYPE_I2C      (4)
#define ESP_PANEL_BUS_TYPE_I80      (5)
#define ESP_PANEL_BUS_TYPE_MAX      (6)

#define _ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name) ESP_LCD_TOUCH_IO_I2C_ ## name ## _CONFIG()
/**
 * @brief This macro is used to generate the I2C panel IO configuration according to the touch panel name.
 *
 * Taking GT911 as an example, the following is the actual code after macro expansion:
 *
 *      ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(GT911) => ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG()
 */
#define ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name)  _ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name)

#define _ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(name, cs_io) ESP_LCD_TOUCH_IO_SPI_ ## name ## _CONFIG(cs_io)
/**
 * @brief This macro is used to generate the SPI panel IO configuration according to the touch panel name.
 *
 * Taking XPT2046 as an example, the following is the actual code after macro expansion:
 *
 *      ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(XPT2046, 5) => ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(5)
 */
#define ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(name, cs_io)  _ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(name, cs_io)

/**
 * @brief Formater for single LCD vendor command with 8-bit parameter
 *
 * @param delay_ms Delay in milliseconds after this command
 * @param command  LCD command
 * @param ...      Array of 8-bit command parameters, should be like `{data0, data1, data2, ...}`
 *
 */
#define ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, ...) {command, (uint8_t []) __VA_ARGS__, sizeof((uint8_t []) \
                                                                __VA_ARGS__), delay_ms}
/**
 * @brief Formater for single LCD vendor command with no parameter
 *
 * @param delay_ms Delay in milliseconds after this command
 * @param command  LCD command
 *
 */
#define ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command) {command, (uint8_t []){ 0x00 }, 0, delay_ms}
