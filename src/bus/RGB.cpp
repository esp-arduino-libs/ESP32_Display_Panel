/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include <stdlib.h>
#include <string.h>
#include "esp_lcd_panel_io.h"
#include "ESP_PanelPrivate.h"
#include "RGB.h"

static const char *TAG = "ESP_PanelBus_RGB";

ESP_PanelBus_RGB::ESP_PanelBus_RGB(uint16_t width, uint16_t height, int cs_io, int scl_io, int sda_io, int hsync_io,
                                   int vsync_io, int pclk_io, int d0_io, int d1_io, int d2_io, int d3_io, int d4_io,
                                   int d5_io, int d6_io, int d7_io, int d8_io, int d9_io, int d10_io, int d11_io,
                                   int d12_io, int d13_io, int d14_io, int d15_io, int de_io, int disp_io):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_RGB, true),
    rgb_config((esp_lcd_rgb_panel_config_t)RGB_16BIT_PANEL_CONFIG_DEFAULT(width, height, hsync_io, vsync_io, pclk_io,
               d0_io, d1_io, d2_io, d3_io, d4_io, d5_io,
               d6_io, d7_io, d8_io, d9_io, d10_io, d11_io,
               d12_io, d13_io, d14_io, d15_io, de_io,
               disp_io)),
    spi_config((esp_lcd_panel_io_3wire_spi_config_t)RGB_PANEL_IO_CONFIG_DEFAULT(cs_io, scl_io, sda_io))
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(uint16_t width, uint16_t height, int hsync_io, int vsync_io, int pclk_io, int d0_io,
                                   int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
                                   int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io,
                                   int d15_io, int de_io, int disp_io):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_RGB, false),
    rgb_config((esp_lcd_rgb_panel_config_t)RGB_16BIT_PANEL_CONFIG_DEFAULT(width, height, hsync_io, vsync_io, pclk_io,
               d0_io, d1_io, d2_io, d3_io, d4_io, d5_io,
               d6_io, d7_io, d8_io, d9_io, d10_io, d11_io,
               d12_io, d13_io, d14_io, d15_io, de_io,
               disp_io))
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(uint16_t width, uint16_t height, int cs_io, int scl_io, int sda_io, int hsync_io,
                                   int vsync_io, int pclk_io, int d0_io, int d1_io, int d2_io, int d3_io, int d4_io,
                                   int d5_io, int d6_io, int d7_io, int de_io, int disp_io):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_RGB, true),
    rgb_config((esp_lcd_rgb_panel_config_t)RGB_8BIT_PANEL_CONFIG_DEFAULT(width, height, hsync_io, vsync_io, pclk_io,
               d0_io, d1_io, d2_io, d3_io, d4_io, d5_io,
               d6_io, d7_io, de_io, disp_io)),
    spi_config((esp_lcd_panel_io_3wire_spi_config_t)RGB_PANEL_IO_CONFIG_DEFAULT(cs_io, scl_io, sda_io))
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(uint16_t width, uint16_t height, int hsync_io, int vsync_io, int pclk_io, int d0_io,
                                   int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
                                   int de_io, int disp_io):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_RGB, false),
    rgb_config((esp_lcd_rgb_panel_config_t)RGB_8BIT_PANEL_CONFIG_DEFAULT(width, height, hsync_io, vsync_io, pclk_io,
               d0_io, d1_io, d2_io, d3_io, d4_io, d5_io,
               d6_io, d7_io, de_io, disp_io))
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(const esp_lcd_panel_io_3wire_spi_config_t &spi_config,
                                   const esp_lcd_rgb_panel_config_t &rgb_config, int host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_RGB, true),
    rgb_config(rgb_config),
    spi_config(spi_config)
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t &rgb_config, int host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_RGB, false),
    rgb_config(rgb_config)
{
}

ESP_PanelBus_RGB::~ESP_PanelBus_RGB()
{
    if (handle == NULL) {
        ESP_LOGD(TAG, "Panel IO is not initialized");
        return;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete panel io failed");
    }

    ESP_LOGD(TAG, "Destory");
}

void ESP_PanelBus_RGB::configSpiLine(bool cs_use_expaneer, bool scl_use_expander, bool sda_use_expander,
                                     ESP_IOExpander *io_expander)
{
    if (cs_use_expaneer) {
        spi_config.line_config.cs_io_type = IO_TYPE_EXPANDER;
    }
    if (scl_use_expander) {
        spi_config.line_config.cs_io_type = IO_TYPE_EXPANDER;
    }
    if (sda_use_expander) {
        spi_config.line_config.cs_io_type = IO_TYPE_EXPANDER;
    }
    if (cs_use_expaneer || scl_use_expander || sda_use_expander) {
        CHECK_NULL_RETURN(io_expander);
        spi_config.line_config.io_expander = io_expander->getHandle();
    }
}

void ESP_PanelBus_RGB::setSpiExpander(ESP_IOExpander *io_expander)
{
    CHECK_NULL_RETURN(io_expander);
    spi_config.line_config.io_expander = io_expander->getHandle();
}

void ESP_PanelBus_RGB::setRgbPclkFrequency(int hz)
{
    CHECK_FALSE_RETURN(hz > 0);
    rgb_config.timings.pclk_hz = hz;
}

void ESP_PanelBus_RGB::setRgbResolution(int width, int height)
{
    CHECK_FALSE_RETURN((width > 0) && (height > 0));
    rgb_config.timings.h_res = width;
    rgb_config.timings.v_res = height;
}

void ESP_PanelBus_RGB::setRgbPorchValue(int hsync_pulse_width, int hsync_back_porch, int hsync_front_porch,
                                        int vsync_pulse_width, int vsync_back_porch, int vsync_front_porch)
{
    CHECK_FALSE_RETURN((hsync_pulse_width >= 0) && (hsync_back_porch > 0) && (hsync_front_porch > 0) &&
                       (vsync_pulse_width >= 0) && (vsync_back_porch > 0) && (vsync_front_porch > 0));
    rgb_config.timings.hsync_pulse_width = hsync_pulse_width;
    rgb_config.timings.hsync_back_porch = hsync_back_porch;
    rgb_config.timings.hsync_front_porch = hsync_front_porch;
    rgb_config.timings.vsync_pulse_width = vsync_pulse_width;
    rgb_config.timings.vsync_back_porch = vsync_back_porch;
    rgb_config.timings.vsync_front_porch = vsync_front_porch;
}

void ESP_PanelBus_RGB::enableRgbPclkActiveNegative(void)
{
    rgb_config.timings.flags.pclk_active_neg = 1;
}

void ESP_PanelBus_RGB::setRgbDataWidth(int num)
{
    CHECK_FALSE_RETURN((num == 8) || (num == 16));
    rgb_config.data_width = num;
}

void ESP_PanelBus_RGB::setRgbColorBits(int num)
{
    CHECK_FALSE_RETURN(num > 0);
    rgb_config.bits_per_pixel = num;
}

void ESP_PanelBus_RGB::setRgbFrameBufferCount(int num)
{
    CHECK_FALSE_RETURN((num >= 1) && (num <= 3));
    rgb_config.num_fbs = num;
}

void ESP_PanelBus_RGB::setRgbBounceBufferSize(int size_in_pixel)
{
    CHECK_FALSE_RETURN(size_in_pixel > 0);
    rgb_config.bounce_buffer_size_px = size_in_pixel;
}

const esp_lcd_rgb_panel_config_t *ESP_PanelBus_RGB::rgbConfig()
{
    return &rgb_config;
}

bool ESP_PanelBus_RGB::begin(void)
{
    ENABLE_TAG_PRINT_DEBUG_LOG();

    if (host_need_init) {
        CHECK_ERR_RET(esp_lcd_new_panel_io_3wire_spi(&spi_config, &handle), false, "Create panel io failed");
        ESP_LOGD(TAG, "Create panel io @%p", handle);
    }

    return true;
}

#endif /* SOC_LCD_RGB_SUPPORTED */
