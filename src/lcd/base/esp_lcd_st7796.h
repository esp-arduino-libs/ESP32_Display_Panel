/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * @file
 * @brief ESP LCD: ST7796
 */

#pragma once

#include "hal/lcd_types.h"
#include "esp_lcd_panel_vendor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ESP_LCD_ST7796_VER_MAJOR    (1)
#define ESP_LCD_ST7796_VER_MINOR    (2)
#define ESP_LCD_ST7796_VER_PATCH    (1)
/**
 * @brief Create LCD panel for model ST7796
 *
 * @note  Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for initialization sequence code.
 *
 * @param[in] io LCD panel IO handle
 * @param[in] panel_dev_config general panel device configuration
 * @param[out] ret_panel Returned LCD panel handle
 * @return
 *          - ESP_ERR_INVALID_ARG   if parameter is invalid
 *          - ESP_ERR_NO_MEM        if out of memory
 *          - ESP_OK                on success
 */
esp_err_t esp_lcd_new_panel_st7796(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config, esp_lcd_panel_handle_t *ret_panel);

/**
 * @brief LCD panel bus configuration structure
 *
 * @param[in] max_trans_bytes Maximum bytes of data to be transferred in one transaction. Normally set to the size of one frame bytes.
 * @param[in] data_width Data bus width
 * @param[in] dc I80 data/command pin number
 * @param[in] wr I80 write clock pin number
 * @param[in] d[0:15] I80 data pin number 0 ~ 15. Set to -1 if not used.
 *
 */
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(4, 4, 1)
#define ST7796_PANEL_BUS_I80_CONFIG(max_trans_bytes, data_width, dc, wr, d0, d1, d2, d3, d4,    \
                                    d5, d6, d7, d8 , d9, d10, d11, d12, d13, d14, d15)          \
    {                                                                                           \
        .clk_src = LCD_CLK_SRC_PLL160M,                                                         \
        .dc_gpio_num = dc,                                                                      \
        .wr_gpio_num = wr,                                                                      \
        .data_gpio_nums = {                                                                     \
            d0, d1, d2, d3, d4, d5, d6, d7, d8 , d9, d10, d11, d12, d13, d14, d15               \
        },                                                                                      \
        .bus_width = data_width,                                                                \
        .max_transfer_bytes = max_trans_bytes,                                                  \
    }
#else
#define ST7796_PANEL_BUS_I80_CONFIG(max_trans_bytes, data_width, dc, wr, d0, d1, d2, d3, d4,    \
                                    d5, d6, d7, d8 , d9, d10, d11, d12, d13, d14, d15)          \
    {                                                                                           \
        .clk_src = LCD_CLK_SRC_PLL160M,                                                         \
        .dc_gpio_num = dc,                                                                      \
        .wr_gpio_num = wr,                                                                      \
        .data_gpio_nums = {                                                                     \
            d0, d1, d2, d3, d4, d5, d6, d7, d8 , d9, d10, d11, d12, d13, d14, d15               \
        },                                                                                      \
        .bus_width = data_width,                                                                \
        .max_transfer_bytes = max_trans_bytes,                                                  \
        .psram_trans_align = 64,                                                                \
        .sram_trans_align = 4,                                                                  \
    }
#endif

/**
 * @brief LCD panel IO configuration structure
 *
 * @param[in] cs I80 chip select pin number
 * @param[in] cb Callback function when data transfer is done
 * @param[in] cb_ctx Callback function context
 *
 */
#define ST7796_PANEL_IO_I80_CONFIG(cs, cb, cb_ctx)  \
    {                                               \
        .cs_gpio_num = cs,                          \
        .pclk_hz = 10 * 1000 * 1000,                \
        .trans_queue_depth = 10,                    \
        .dc_levels = {                              \
            .dc_idle_level = 0,                     \
            .dc_cmd_level = 0,                      \
            .dc_dummy_level = 0,                    \
            .dc_data_level = 1,                     \
        },                                          \
        .flags = {                                  \
            .swap_color_bytes = 1,                  \
        },                                          \
        .on_color_trans_done = cb,                  \
        .user_ctx = cb_ctx,                         \
        .lcd_cmd_bits = 8,                          \
        .lcd_param_bits = 8,                        \
    }

#ifdef __cplusplus
}
#endif
