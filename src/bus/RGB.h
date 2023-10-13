/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "esp_lcd_panel_rgb.h"
#include "ESP_IOExpander.h"
#include "base/esp_lcd_panel_io_additions.h"
#include "ESP_PanelBus.h"

/**
 * For the ESP32-S3, the RGB peripheral only supports 16-bit RGB565.and 8-bit RGB888 color formats. For more details,
 * please refer to the part `Display > LCD Screen > RGB LCD Introduction` in ESP-IoT-Solution Programming Guide
 * (https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/rgb_lcd.html#color-formats).
 *
 */

/**
 * @brief Macro for 16-bit RGB timing configuration
 *
 * @note  Typically, for a 480x480 LCD with 16-bit RGB565 color format, the refresh rate is 60Hz
 */
#define ESP_PANEL_RGB_TIMING_16BIT_CONFIG_DEFAULT(width, height) \
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

/**
 * @brief Macro for 16-bit RGB565 RGB configuration
 *
 */
#define ESP_PANEL_RGB_16BIT_CONFIG_DEFAULT(width, height, \
                                           d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,       \
                                           d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io, \
                                           hsync_io, vsync_io, pclk_io, de_io, disp_io)                  \
    {                                                                                   \
        .clk_src = LCD_CLK_SRC_DEFAULT,                                                 \
        .timings = ESP_PANEL_RGB_TIMING_16BIT_CONFIG_DEFAULT(width, height),            \
        .data_width = 16,                                                               \
        .bits_per_pixel = 16,                                                           \
        .num_fbs = 1,                                                                   \
        .bounce_buffer_size_px = 0,                                                     \
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
            .disp_active_low = 0,                                                       \
            .refresh_on_demand = 0,                                                     \
            .fb_in_psram = 1,                                                           \
            .double_fb = 0,                                                             \
            .no_fb = 0,                                                                 \
            .bb_invalidate_cache = 0,                                                   \
        },                                                                              \
    }

/**
 * @brief Macro for 8-bit RGB timing configuration
 *
 * @note  Typically, for a 320x480 LCD with 8-bit RGB888 color format, the refresh rate is 48Hz
 */
#define ESP_PANEL_RGB_TIMING_8BIT_CONFIG_DEFAULT(width, height) \
    {                                                 \
        .pclk_hz = 24 * 1000 * 1000,                  \
        .h_res = width,                               \
        .v_res = height,                              \
        .hsync_pulse_width = 3,                       \
        .hsync_back_porch = 3,                        \
        .hsync_front_porch = 6,                       \
        .vsync_pulse_width = 1,                       \
        .vsync_back_porch = 6,                        \
        .vsync_front_porch = 6,                       \
        .flags = {                                    \
            .hsync_idle_low = 0,                      \
            .vsync_idle_low = 0,                      \
            .de_idle_high = 0,                        \
            .pclk_active_neg = 0,                     \
            .pclk_idle_high = 0,                      \
        },                                            \
    }

/**
 * @brief Macro for 8-bit RGB888 RGB configuration
 *
 */
#define ESP_PANEL_RGB_8BIT_CONFIG_DEFAULT(width, height, \
                                          d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,       \
                                          hsync_io, vsync_io, pclk_io, de_io, disp_io)                  \
    {                                                                       \
        .clk_src = LCD_CLK_SRC_DEFAULT,                                     \
        .timings = ESP_PANEL_RGB_TIMING_8BIT_CONFIG_DEFAULT(width, height), \
        .data_width = 8,                                                    \
        .bits_per_pixel = 24,                                               \
        .num_fbs = 1,                                                       \
        .bounce_buffer_size_px = 0,                                         \
        .psram_trans_align = 64,                                            \
        .hsync_gpio_num = hsync_io,                                         \
        .vsync_gpio_num = vsync_io,                                         \
        .de_gpio_num = de_io,                                               \
        .pclk_gpio_num = pclk_io,                                           \
        .disp_gpio_num = disp_io,                                           \
        .data_gpio_nums = {                                                 \
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,         \
            -1, -1, -1, -1, -1, -1, -1, -1,                                 \
        },                                                                  \
        .flags = {                                                          \
            .disp_active_low = 0,                                           \
            .refresh_on_demand = 0,                                         \
            .fb_in_psram = 1,                                               \
            .double_fb = 0,                                                 \
            .no_fb = 0,                                                     \
            .bb_invalidate_cache = 0,                                       \
        },                                                                  \
    }

/**
 * @brief Macro for 3-wire SPI panel IO configuration
 *
 */
#define ESP_PANEL_IO_3WIRE_SPI_CONFIG_DEFAULT(cs_io, sck_io, sda_io)  \
    {                                                       \
        .line_config = {                                    \
            .cs_io_type = IO_TYPE_GPIO,                     \
            .cs_gpio_num = cs_io,                           \
            .scl_io_type = IO_TYPE_GPIO,                    \
            .scl_gpio_num = sck_io,                         \
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

/**
 * @brief RGB bus object class
 *
 * @note  This class is a derived class of `ESP_PanelBus`, user can use it directly
 */
class ESP_PanelBus_RGB: public ESP_PanelBus {
public:
    /**
     * @brief Construct a 3-wire SPI + 16-bit RGB bus object in a common way, the 3-wire SPI host will be initialized
     *        by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_IO_3WIRE_SPI_CONFIG_DEFAULT and
     *        ESP_PANEL_RGB_16BIT_CONFIG_DEFAULT) to config the bus object, use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param width    The width of the panel (horizontal resolution)
     * @param height   The height of the panel (vertical resolution)
     * @param cs_io    3-wire SPI CS pin
     * @param sck_io   3-wire SPI SCK pin
     * @param sda_io   3-wire SPI SDA pin
     * @param dN_io    RGB data pins, N is [0, 15]
     * @param hsync_io RGB HSYNC pin
     * @param vsync_io RGB VSYNC pin
     * @param pclk_io  RGB PCLK pin
     * @param de_io    RGB DE pin, set to -1 if not used
     * @param disp_io  RGB DISP pin, default is -1
     */
    ESP_PanelBus_RGB(uint16_t width, uint16_t height,
                     int cs_io, int sck_io, int sda_io,
                     int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
                     int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
                     int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1);

    /**
     * @brief Construct a 3-wire SPI + 8-bit RGB bus object in a common way, the 3-wire SPI host will be initialized
     *        by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_IO_3WIRE_SPI_CONFIG_DEFAULT and
     *        ESP_PANEL_RGB_8BIT_CONFIG_DEFAULT) to config the bus object, use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param width    The width of the panel (horizontal resolution)
     * @param height   The height of the panel (vertical resolution)
     * @param cs_io    3-wire SPI CS pin
     * @param sck_io   3-wire SPI SCK pin
     * @param sda_io   3-wire SPI SDA pin
     * @param dN_io    RGB data pins, N is [0, 7]
     * @param hsync_io RGB HSYNC pin
     * @param vsync_io RGB VSYNC pin
     * @param pclk_io  RGB PCLK pin
     * @param de_io    RGB DE pin, set to -1 if not used
     * @param disp_io  RGB DISP pin, default is -1
     */
    ESP_PanelBus_RGB(uint16_t width, uint16_t height,
                     int cs_io, int sck_io, int sda_io,
                     int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
                     int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1);

    /**
     * @brief Construct a single 16-bit RGB bus object in a common way, no host will be initialized
     *
     * @note  This function uses some default values (ESP_PANEL_RGB_16BIT_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param width    The width of the panel (horizontal resolution)
     * @param height   The height of the panel (vertical resolution)
     * @param dN_io    RGB data pins, N is [0, 16]
     * @param hsync_io RGB HSYNC pin
     * @param vsync_io RGB VSYNC pin
     * @param pclk_io  RGB PCLK pin
     * @param de_io    RGB DE pin, set to -1 if not used
     * @param disp_io  RGB DISP pin, default is -1
     */
    ESP_PanelBus_RGB(uint16_t width, uint16_t height,
                     int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
                     int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
                     int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1);

    /**
     * @brief Construct a single 8-bit RGB bus object in a common way, no host will be initialized
     *
     * @note  This function uses some default values (ESP_PANEL_RGB_8BIT_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param width    The width of the panel (horizontal resolution)
     * @param height   The height of the panel (vertical resolution)
     * @param dN_io    RGB data pins, N is [0, 16]
     * @param hsync_io RGB HSYNC pin
     * @param vsync_io RGB VSYNC pin
     * @param pclk_io  RGB PCLK pin
     * @param de_io    RGB DE pin, set to -1 if not used
     * @param disp_io  RGB DISP pin, default is -1
     */
    ESP_PanelBus_RGB(uint16_t width, uint16_t height,
                     int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
                     int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1);

    /**
     * @brief Construct a 3-wire SPI + 16-bit RGB bus object in a complex way, the 3-wire SPI host will be initialized
     *        by the driver
     *
     * @note  The `init()` function should be called after this function
     *
     * @param spi_config 3-wire SPI panel IO configuration
     * @param rgb_config RGB configuration
     * @param host_id    Host ID, it is only used to maintain consistency in formatting and set to `-1` by default
     */
    ESP_PanelBus_RGB(const esp_lcd_panel_io_3wire_spi_config_t &io_config, const esp_lcd_rgb_panel_config_t &rgb_config,
                     int host_id = -1);

    /**
     * @brief Construct a single RGB bus object in a complex way, no host will be initialized
     *
     * @note  The `init()` function should be called after this function
     *
     * @param rgb_config RGB configuration
     * @param host_id    Host ID, it is only used to maintain consistency in formatting and set to `-1` by default
     */
    ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t &rgb_config, int host_id = -1);

    /**
     * @brief Destroy the RGB bus object
     *
     */
    ~ESP_PanelBus_RGB() override;

    /**
     * @brief Here are some functions to configure the RGB bus object
     *
     * @note  These functions should be called before `begin()`
     *
     */
    void configRgbTimingFreqHz(uint32_t hz);
    void configRgbTimingPorch(uint16_t hpw, uint16_t hbp, uint16_t hfp, uint16_t vpw, uint16_t vbp, uint16_t vfp);
    void configRgbTimingFlags(bool hsync_idle_low, bool vsync_idle_low, bool de_idle_high, bool pclk_active_neg,
                              bool pclk_idle_high);
    void configRgbFrameBufferNumber(uint8_t num);
    void configRgbBounceBufferSize(uint32_t size_in_pixel);
    void configRgbFlagDispActiveLow(void);
    void configSpiLine(bool cs_use_expaneer, bool sck_use_expander, bool sda_use_expander, ESP_IOExpander *io_expander);

    /**
     * @brief Startup the bus.
     *
     * @return true if success, otherwise false
     */
    bool begin(void) override;

    const esp_lcd_rgb_panel_config_t *getRgbConfig();

private:
    esp_lcd_rgb_panel_config_t rgb_config;
    esp_lcd_panel_io_3wire_spi_config_t spi_config;
};

#endif /* SOC_LCD_RGB_SUPPORTED */
