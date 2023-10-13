/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "soc/soc_caps.h"
#include "esp_idf_version.h"

#if SOC_LCD_RGB_SUPPORTED && (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(5, 0, 0))
#include <stdint.h>

#include "hal/lcd_types.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_rgb.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create LCD panel for model ST7701
 *
 * @note  When `auto_del_panel_io` is set to 1, this function will first initialize the ST7701 with vendor specific initialization and then calls `esp_lcd_new_rgb_panel()` to create an RGB LCD panel. And the `esp_lcd_panel_init()` function will only initialize RGB.
 * @note  When `auto_del_panel_io` is set to 0, this function will only call `esp_lcd_new_rgb_panel()` to create an RGB LCD panel. And the `esp_lcd_panel_init()` function will initialize both the ST7701 and RGB.
 * @note  Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for initialization sequence code.
 *
 * @param[in]  io LCD panel IO handle
 * @param[in]  panel_dev_config General panel device configuration (`vendor_config` and `rgb_config` are necessary)
 * @param[out] ret_panel Returned LCD panel handle
 * @return
 *      - ESP_ERR_INVALID_ARG   if parameter is invalid
 *      - ESP_OK                on success
 *      - Otherwise             on fail
 */
esp_err_t esp_lcd_new_panel_st7701(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config,
                                   esp_lcd_panel_handle_t *ret_panel);

/**
 * @brief 3-wire SPI panel IO configuration structure
 *
 * @param[in] line_cfg SPI line configuration
 * @param[in] scl_active_edge SCL signal active edge, 0: rising edge, 1: falling edge
 *
 */
#define ST7701_PANEL_IO_3WIRE_SPI_CONFIG(line_cfg, scl_active_edge) \
    {                                                               \
        .line_config = line_cfg,                                    \
        .expect_clk_speed = PANEL_IO_3WIRE_SPI_CLK_MAX,             \
        .spi_mode = scl_active_edge ? 1 : 0,                        \
        .lcd_cmd_bytes = 1,                                         \
        .lcd_param_bytes = 1,                                       \
        .flags = {                                                  \
            .use_dc_bit = 1,                                        \
            .dc_zero_on_data = 0,                                   \
            .lsb_first = 0,                                         \
            .cs_high_active = 0,                                    \
            .del_keep_cs_inactive = 1,                              \
        },                                                          \
    }

/**
 * @brief RGB timing structure
 *
 * @note  refresh_rate = (pclk_hz * data_width) / (h_res + hsync_pulse_width + hsync_back_porch + hsync_front_porch)
 *                                              / (v_res + vsync_pulse_width + vsync_back_porch + vsync_front_porch)
 *                                              / bits_per_pixel
 *
 */
#define ST7701_480_480_PANEL_60HZ_RGB_TIMING()      \
    {                                               \
        .pclk_hz = 16 * 1000 * 1000,                \
        .h_res = 480,                               \
        .v_res = 480,                               \
        .hsync_pulse_width = 10,                    \
        .hsync_back_porch = 10,                     \
        .hsync_front_porch = 20,                    \
        .vsync_pulse_width = 10,                    \
        .vsync_back_porch = 10,                     \
        .vsync_front_porch = 10,                    \
    }

#ifdef __cplusplus
}
#endif
#endif
