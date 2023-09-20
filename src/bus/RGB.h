/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELBUS_RGB_H
#define ESP_PANELBUS_RGB_H

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED

#include <stdint.h>

#include "esp_lcd_panel_rgb.h"
#include "ESP_IOExpander.h"

#include "base/esp_lcd_panel_io_additions.h"
#include "../ESP_PanelBus.h"

#define RGB_TIMING_CONFIG_DEFAULT(width, height)            \
    {                                                       \
        .pclk_hz = 16 * 1000 * 1000,                        \
        .h_res = width,                                     \
        .v_res = height,                                    \
        .hsync_pulse_width = 10,                            \
        .hsync_back_porch = 10,                             \
        .hsync_front_porch = 20,                            \
        .vsync_pulse_width = 10,                            \
        .vsync_back_porch = 10,                             \
        .vsync_front_porch = 10,                            \
        .flags = {                                          \
            .pclk_active_neg = false,                       \
        },                                                  \
    }
#define RGB_HOST_CONFIG_DEFAULT(width, height, hsync, vsync, pclk, D0, D1, D2, D3, D4, D5, \
                                D6, D7, D8, D9, D10, D11, D12, D13, D14, D15, de, disp)   \
    {                                                                                   \
        .clk_src = LCD_CLK_SRC_PLL160M,                                                 \
        .timings = RGB_TIMING_CONFIG_DEFAULT(width, height),                            \
        .data_width = 16,                                                               \
        .psram_trans_align = 64,                                                        \
        .hsync_gpio_num = hsync,                                                        \
        .vsync_gpio_num = vsync,                                                        \
        .de_gpio_num = de,                                                              \
        .pclk_gpio_num = pclk,                                                          \
        .data_gpio_nums = {                                                             \
            D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15        \
        },                                                                              \
        .disp_gpio_num = disp,                                                          \
        .on_frame_trans_done = (esp_lcd_rgb_panel_frame_trans_done_cb_t)callback,       \
        .flags = {                                                                      \
            .fb_in_psram = 1,                                                           \
        },                                                                              \
    }
#define RGB_PANEL_IO_CONFIG_DEFAULT(cs, sck, sda)           \
    {                                                       \
        .line_config = {                 \
            .cs_io_type = IO_TYPE_GPIO,                     \
            .cs_gpio_num = cs,                              \
            .scl_io_type = IO_TYPE_GPIO,                    \
            .scl_gpio_num = scl,                            \
            .sda_io_type = IO_TYPE_GPIO,                    \
            .sda_gpio_num = sda,                            \
        },                                                  \
        .expect_clk_speed = PANEL_IO_3WIRE_SPI_CLK_MAX,     \
        .spi_mode = 0,                                      \
        .lcd_cmd_bytes = 1,                                 \
        .lcd_param_bytes = 1,                               \
        .flags = {                                          \
            .use_dc_bit = true,                             \
            .del_keep_cs_inactive = true,                   \
        },                                                  \
    }

class ESP_PanelBus_RGB: public ESP_PanelBus {
public:
    ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t *rgb_config, const esp_lcd_panel_io_3wire_spi_config_t *spi_config, int host_id = -1);
    ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t *rgb_config, int host_id = -1);
    ESP_PanelBus_RGB(uint16_t width, uint16_t height, int hsync, int vsync, int pclk,
                     int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7,
                     int D8, int D9, int D10, int D11, int D12, int D13, int D14, int D15,
                     int de = -1, int disp = -1, int cs = -1, int scl = -1, int sda = -1);
    ESP_PanelBus_RGB(uint16_t width, uint16_t height, int hsync, int vsync, int pclk,
                     int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7,
                     int de = -1, int disp = -1, int cs = -1, int scl = -1, int sda = -1);
    ~ESP_PanelBus_RGB() override;

    const esp_lcd_rgb_panel_config_t *getRGBConfig();

    void init(void) override;

private:
    esp_lcd_rgb_panel_config_t rgb_config;
    esp_lcd_panel_io_3wire_spi_config_t spi_config;
};

#endif /* SOC_LCD_RGB_SUPPORTED */

#endif
