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

#include "private/CheckResult.h"
#include "RGB.h"

static const char *TAG = "ESP_PanelBus_RGB";

ESP_PanelBus_RGB::ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t *rgb_config, const esp_lcd_panel_io_3wire_spi_config_t *spi_config, int host_id)
{
    CHECK_NULL_RETURN(rgb_config);
    CHECK_NULL_RETURN(spi_config);

    this->rgb_config = *rgb_config;
    this->spi_config = *spi_config;
    flags.host_need_init = true;

    if (this->rgb_config.on_frame_trans_done == NULL) {
        ctx.bus = this;
        this->rgb_config.on_frame_trans_done = (esp_lcd_rgb_panel_frame_trans_done_cb_t)callback;
        this->rgb_config.user_ctx = (void *)&ctx;
    }
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t *rgb_config, int host_id)
{
    CHECK_NULL_RETURN(rgb_config);

    this->rgb_config = *rgb_config;
    flags.host_need_init = false;

    if (this->rgb_config.on_frame_trans_done == NULL) {
        ctx.bus = this;
        this->rgb_config.on_frame_trans_done = (esp_lcd_rgb_panel_frame_trans_done_cb_t)callback;
        this->rgb_config.user_ctx = (void *)&ctx;
    }
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(uint16_t width, uint16_t height,  int hsync, int vsync, int pclk,
                                   int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7,
                                   int D8, int D9, int D10, int D11, int D12, int D13, int D14, int D15,
                                   int de, int disp, int cs, int scl, int sda):
    rgb_config((esp_lcd_rgb_panel_config_t)RGB_HOST_CONFIG_DEFAULT(width, height, hsync, vsync, pclk, D0, D1, D2, D3, D4,
               D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15, de, disp)),
    spi_config((esp_lcd_panel_io_3wire_spi_config_t)RGB_PANEL_IO_CONFIG_DEFAULT(cs, scl, sda))
{
    flags.host_need_init = ((cs != -1) && (scl != -1) && (sda != -1));
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(uint16_t width, uint16_t height, int hsync, int vsync, int pclk,
                                   int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7,
                                   int de, int disp, int cs, int scl, int sda):
    rgb_config((esp_lcd_rgb_panel_config_t)RGB_HOST_CONFIG_DEFAULT(width, height, hsync, vsync, pclk, D0, D1, D2, D3, D4,
               D5, D6, D7, -1, -1, -1, -1, -1, -1, -1, -1, de, disp)),
    spi_config((esp_lcd_panel_io_3wire_spi_config_t)RGB_PANEL_IO_CONFIG_DEFAULT(cs, scl, sda))
{
    flags.host_need_init = ((cs != -1) && (scl != -1) && (sda != -1));
}

ESP_PanelBus_RGB::~ESP_PanelBus_RGB()
{
    if (handle && flags.host_need_init) {
        del();
    }
}

const esp_lcd_rgb_panel_config_t *ESP_PanelBus_RGB::getRGBConfig()
{
    return &rgb_config;
}

void ESP_PanelBus_RGB::init(void)
{
    if (flags.host_need_init) {
        CHECK_ERROR_RETURN(esp_lcd_new_panel_io_3wire_spi(&spi_config, &handle));
    }
}

#endif /* SOC_LCD_RGB_SUPPORTED */
