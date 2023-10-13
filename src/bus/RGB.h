/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELBUS_RGB_H
#define ESP_PANELBUS_RGB_H

#include "soc/soc_caps.h"
#include "esp_idf_version.h"

#if SOC_LCD_RGB_SUPPORTED && (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(5, 0, 0))

#include <stdint.h>

#include "esp_lcd_panel_rgb.h"
#include "esp_idf_version.h"
#include "ESP_IOExpander.h"

#include "base/esp_lcd_panel_io_additions.h"
#include "ESP_PanelBus.h"

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
            .hsync_idle_low = 0,                            \
            .vsync_idle_low = 0,                            \
            .de_idle_high = 0,                              \
            .pclk_active_neg = 0,                           \
            .pclk_idle_high = 0,                            \
        },                                                  \
    }

#define RGB_PANEL_CONFIG_DEFAULT(width, height, hsync_io, vsync_io, pclk_io, d0_io, d1_io,       \
                                 d2_io, d3_io, d4_io, d5_io, d6_io, d7_io, d8_io, d9_io,         \
                                 d10_io, d11_io, d12_io, d13_io, d14_io, d15_io, de_io, disp_io) \
    {                                                                                   \
        .clk_src = LCD_CLK_SRC_DEFAULT,                                                 \
        .timings = RGB_TIMING_CONFIG_DEFAULT(width, height),                            \
        .data_width = 16,                                                               \
        .bits_per_pixel = 16,                                                           \
        .num_fbs = 1,                                                                   \
        .bounce_buffer_size_px = width * 10,                                            \
        .psram_trans_align = 64,                                                        \
        .hsync_gpio_num = hsync_io,                                                     \
        .vsync_gpio_num = vsync_io,                                                     \
        .de_gpio_num = de_io,                                                           \
        .pclk_gpio_num = pclk_io,                                                       \
        .disp_gpio_num = disp_io,                                                       \
        .data_gpio_nums = {                                                             \
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,                     \
            d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io                \
        },                                                                              \
        .flags = {                                                                      \
            .fb_in_psram = 1,                                                           \
        },                                                                              \
    }

#define RGB_PANEL_IO_CONFIG_DEFAULT(cs_io, sck_io, sda_io)  \
    {                                                       \
        .line_config = {                                    \
            .cs_io_type = IO_TYPE_GPIO,                     \
            .cs_gpio_num = cs_io,                           \
            .scl_io_type = IO_TYPE_GPIO,                    \
            .scl_gpio_num = scl_io,                         \
            .sda_io_type = IO_TYPE_GPIO,                    \
            .sda_gpio_num = sda_io,                         \
            .io_expander = NULL,                            \
        },                                                  \
        .expect_clk_speed = PANEL_IO_3WIRE_SPI_CLK_MAX,     \
        .spi_mode = 0,                                      \
        .lcd_cmd_bytes = 1,                                 \
        .lcd_param_bytes = 1,                               \
        .flags = {                                          \
            .use_dc_bit = 1,                                \
            .dc_zero_on_data = 0,                           \
            .lsb_first = 0,                                 \
            .cs_high_active = 0,                            \
            .del_keep_cs_inactive = 1,                      \
        },                                                  \
    }

class ESP_PanelBus_RGB: public ESP_PanelBus {
public:
    /* For 3-wire SPI + RGB  */
    ESP_PanelBus_RGB(const esp_lcd_panel_io_3wire_spi_config_t *spi_config, const esp_lcd_rgb_panel_config_t *rgb_config,
                     int host_id = -1);
    /* For RGB only */
    ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t *rgb_config, int host_id = -1);

    /* For 3-wire SPI + 16-bit RGB  */
    ESP_PanelBus_RGB(uint16_t width, uint16_t height, int cs_io, int scl_io, int sda_io, int hsync_io, int vsync_io,
                     int pclk_io, int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io,
                     int d7_io, int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io,
                     int d15_io, int de_io = -1, int disp_io = -1);
    /* For 16-bit RGB only */
    ESP_PanelBus_RGB(uint16_t width, uint16_t height, int hsync_io, int vsync_io, int pclk_io, int d0_io, int d1_io,
                     int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io, int d8_io, int d9_io, int d10_io,
                     int d11_io, int d12_io, int d13_io, int d14_io, int d15_io, int de_io = -1, int disp_io = -1);

    /* For 3-wire SPI + 8-bit  */
    ESP_PanelBus_RGB(uint16_t width, uint16_t height, int cs_io, int scl_io, int sda_io, int hsync_io, int vsync_io,
                     int pclk_io, int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io,
                     int d7_io, int de_io = -1, int disp_io = -1);
    /* For 8-bit RGB only */
    ESP_PanelBus_RGB(uint16_t width, uint16_t height, int hsync_io, int vsync_io, int pclk_io, int d0_io, int d1_io,
                    int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io, int de_io = -1, int disp_io = -1);
    ~ESP_PanelBus_RGB() override;

    void enableSpiCsUseExpander(void);
    void enableSpiSclUseExpander(void);
    void enableSpiSdaUseExpander(void);
    void addIOExpander(ESP_IOExpander *io_expander);
    void setRgbPclkFrequency(int hz);
    void setRgbResolution(int width, int height);
    void setRgbPorchValue(int hsync_pulse_width, int hsync_back_porch, int hsync_front_porch,
                          int vsync_pulse_width, int vsync_back_porch, int vsync_front_porch);
    void enableRgbPclkActiveNegative(void);
    void setRgbDataWidth(int num);
    void setRgbColorBits(int num);
    void setRgbFrameBufferCount(int num);
    void setRgbBounceBufferSize(int size_in_pixel);

    const esp_lcd_rgb_panel_config_t *getRgbConfig();

    void init(void) override;

private:
    esp_lcd_rgb_panel_config_t rgb_config;
    esp_lcd_panel_io_3wire_spi_config_t spi_config;
};

#endif /* SOC_LCD_RGB_SUPPORTED */

#endif
