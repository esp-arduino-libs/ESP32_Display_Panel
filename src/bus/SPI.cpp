/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>

#include "esp_lcd_panel_io.h"

#include "../private/CheckResult.h"
#include "SPI.h"

static const char *TAG = "ESP_PanelBus_SPI";

ESP_PanelBus_SPI::ESP_PanelBus_SPI(const spi_bus_config_t *host_config, const esp_lcd_panel_io_spi_config_t *io_config,
                                   spi_host_device_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_SPI, true),
    host_config(*host_config),
    io_config(*io_config),
    host_id(host_id)
{
    if (io_config->on_color_trans_done == NULL) {
        this->io_config.on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)on_transmit_finish_callback;
        this->io_config.user_ctx = (void *)&callback_ctx;
    }
}

ESP_PanelBus_SPI::ESP_PanelBus_SPI(const esp_lcd_panel_io_spi_config_t *io_config, spi_host_device_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_SPI, false),
    io_config(*io_config),
    host_id(host_id)
{
    if (io_config->on_color_trans_done == NULL) {
        this->io_config.on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)on_transmit_finish_callback;
        this->io_config.user_ctx = (void *)&callback_ctx;
    }
}

ESP_PanelBus_SPI::ESP_PanelBus_SPI(int cs_io, int dc_io, int clk_io, int mosi_io, int miso_io):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_SPI, true),
    host_id(SPI_HOST_ID_DEFAULT),
    host_config((spi_bus_config_t)SPI_HOST_CONFIG_DEFAULT(clk_io, mosi_io, miso_io)),
    io_config((esp_lcd_panel_io_spi_config_t)SPI_PANEL_IO_CONFIG_DEFAULT(cs_io, dc_io, on_transmit_finish_callback, &callback_ctx))
{
}

ESP_PanelBus_SPI::ESP_PanelBus_SPI(int cs_io, int dc_io):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_SPI, false),
    host_id(SPI_HOST_ID_DEFAULT),
    io_config((esp_lcd_panel_io_spi_config_t)SPI_PANEL_IO_CONFIG_DEFAULT(cs_io, dc_io, on_transmit_finish_callback, &callback_ctx))
{
}

ESP_PanelBus_SPI::~ESP_PanelBus_SPI()
{
    if (handle) {
        del();
        if (flags.host_need_init) {
            spi_bus_free(host_id);
        }
    }
}

void ESP_PanelBus_SPI::init(void)
{
    if (flags.host_need_init) {
        CHECK_ERROR_RETURN(spi_bus_initialize(host_id, &host_config, SPI_DMA_CH_AUTO));
    }
    CHECK_ERROR_RETURN(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)host_id, &io_config, &handle));
    createTransmitFinishSemaphore();
}
