/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include <stdlib.h>
#include <string.h>
#include "ESP_PanelLog.h"
#include "esp_lcd_panel_io.h"
#include "RGB.h"

static const char *TAG = "ESP_PanelBus_RGB";

ESP_PanelBus_RGB::ESP_PanelBus_RGB(
    uint16_t width, uint16_t height,
    int cs_io, int sck_io, int sda_io,
    int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
    int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
    int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io):
    ESP_PanelBus(-1, ESP_PANEL_BUS_TYPE_RGB, true),
    rgb_config((esp_lcd_rgb_panel_config_t)ESP_PANEL_RGB_16BIT_CONFIG_DEFAULT(
                   width, height,
                   d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
                   d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
                   hsync_io, vsync_io, pclk_io, de_io, disp_io)),
    spi_config((esp_lcd_panel_io_3wire_spi_config_t)ESP_PANEL_IO_3WIRE_SPI_CONFIG_DEFAULT(cs_io, sck_io, sda_io))
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(
    uint16_t width, uint16_t height,
    int cs_io, int sck_io, int sda_io,
    int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
    int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io):
    ESP_PanelBus(-1, ESP_PANEL_BUS_TYPE_RGB, true),
    rgb_config((esp_lcd_rgb_panel_config_t)ESP_PANEL_RGB_8BIT_CONFIG_DEFAULT(
                   width, height,
                   d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
                   hsync_io, vsync_io, pclk_io, de_io, disp_io)),
    spi_config((esp_lcd_panel_io_3wire_spi_config_t)ESP_PANEL_IO_3WIRE_SPI_CONFIG_DEFAULT(cs_io, sck_io, sda_io))
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(
    uint16_t width, uint16_t height,
    int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
    int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
    int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io):
    ESP_PanelBus(-1, ESP_PANEL_BUS_TYPE_RGB, false),
    rgb_config((esp_lcd_rgb_panel_config_t)ESP_PANEL_RGB_16BIT_CONFIG_DEFAULT(
                   width, height,
                   d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
                   d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
                   hsync_io, vsync_io, pclk_io, de_io, disp_io))
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(
    uint16_t width, uint16_t height,
    int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
    int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io):
    ESP_PanelBus(-1, ESP_PANEL_BUS_TYPE_RGB, false),
    rgb_config((esp_lcd_rgb_panel_config_t)ESP_PANEL_RGB_8BIT_CONFIG_DEFAULT(
                   width, height,
                   d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
                   hsync_io, vsync_io, pclk_io, de_io, disp_io))
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(const esp_lcd_panel_io_3wire_spi_config_t &spi_config,
                                   const esp_lcd_rgb_panel_config_t &rgb_config, int host_id):
    ESP_PanelBus(host_id, ESP_PANEL_BUS_TYPE_RGB, true),
    rgb_config(rgb_config),
    spi_config(spi_config)
{
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t &rgb_config, int host_id):
    ESP_PanelBus(host_id, ESP_PANEL_BUS_TYPE_RGB, false),
    rgb_config(rgb_config)
{
}

ESP_PanelBus_RGB::~ESP_PanelBus_RGB()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (host_need_init) {
        if (handle == NULL) {
            goto end;
        }

        if (!del()) {
            ESP_LOGE(TAG, "Delete panel io failed");
        }
    }

end:
    ESP_LOGD(TAG, "Destroyed");
}

void ESP_PanelBus_RGB::configRgbTimingFreqHz(uint32_t hz)
{
    rgb_config.timings.pclk_hz = hz;
}

void ESP_PanelBus_RGB::configRgbTimingPorch(uint16_t hpw, uint16_t hbp, uint16_t hfp,
        uint16_t vpw, uint16_t vbp, uint16_t vfp)
{
    rgb_config.timings.hsync_pulse_width = hpw;
    rgb_config.timings.hsync_back_porch = hbp;
    rgb_config.timings.hsync_front_porch = hfp;
    rgb_config.timings.vsync_pulse_width = vpw;
    rgb_config.timings.vsync_back_porch = vbp;
    rgb_config.timings.vsync_front_porch = vfp;
}

void ESP_PanelBus_RGB::configRgbTimingFlags(bool hsync_idle_low, bool vsync_idle_low, bool de_idle_high,
        bool pclk_active_neg, bool pclk_idle_high)
{
    rgb_config.timings.flags.hsync_idle_low = hsync_idle_low;
    rgb_config.timings.flags.vsync_idle_low = vsync_idle_low;
    rgb_config.timings.flags.de_idle_high = de_idle_high;
    rgb_config.timings.flags.pclk_active_neg = pclk_active_neg;
    rgb_config.timings.flags.pclk_idle_high = pclk_idle_high;
}

void ESP_PanelBus_RGB::configRgbFrameBufferNumber(uint8_t num)
{
    rgb_config.num_fbs = num;
}

void ESP_PanelBus_RGB::configRgbBounceBufferSize(uint32_t size_in_pixel)
{
    rgb_config.bounce_buffer_size_px = size_in_pixel;
}

void ESP_PanelBus_RGB::configRgbFlagDispActiveLow(void)
{
    rgb_config.timings.flags.pclk_active_neg = 1;
}

void ESP_PanelBus_RGB::configSpiLine(bool cs_use_expaneer, bool sck_use_expander, bool sda_use_expander,
                                     ESP_IOExpander *io_expander)
{
    if (cs_use_expaneer) {
        spi_config.line_config.cs_io_type = IO_TYPE_EXPANDER;
        spi_config.line_config.cs_expander_pin = (esp_io_expander_pin_num_t)BIT(spi_config.line_config.cs_gpio_num);
    }
    if (sck_use_expander) {
        spi_config.line_config.scl_io_type = IO_TYPE_EXPANDER;
        spi_config.line_config.scl_expander_pin = (esp_io_expander_pin_num_t)BIT(spi_config.line_config.scl_gpio_num);
    }
    if (sda_use_expander) {
        spi_config.line_config.sda_io_type = IO_TYPE_EXPANDER;
        spi_config.line_config.sda_expander_pin = (esp_io_expander_pin_num_t)BIT(spi_config.line_config.sda_gpio_num);
    }
    if (io_expander != NULL) {
        spi_config.line_config.io_expander = io_expander->getHandle();
    }
}

bool ESP_PanelBus_RGB::begin(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (host_need_init) {
        ESP_PANEL_CHECK_ERR_RET(esp_lcd_new_panel_io_3wire_spi(&spi_config, &handle), false, "Create panel io failed");
        ESP_LOGD(TAG, "Create panel io @%p", handle);
    }

    return true;
}

const esp_lcd_rgb_panel_config_t *ESP_PanelBus_RGB::getRgbConfig()
{
    return &rgb_config;
}

#endif /* SOC_LCD_RGB_SUPPORTED */
