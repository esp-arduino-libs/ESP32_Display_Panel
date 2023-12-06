/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "ESP_PanelHost.h"
#include "ESP_PanelBus.h"

#define ESP_PANEL_HOST_QSPI_ID_DEFAULT  (SPI2_HOST)
#define ESP_PANEL_IO_QSPI_CONFIG_DEFAULT(cs_io) \
    {                                       \
        .cs_gpio_num = cs_io,               \
        .dc_gpio_num = -1,                  \
        .spi_mode = 0,                      \
        .pclk_hz = SPI_MASTER_FREQ_40M,     \
        .trans_queue_depth = 10,            \
        .on_color_trans_done = NULL,        \
        .user_ctx = NULL,                   \
        .lcd_cmd_bits = 32,                 \
        .lcd_param_bits = 8,                \
        .flags = {                          \
            .dc_low_on_data = 0,            \
            .octal_mode = 0,                \
            .quad_mode = 1,                 \
            .sio_mode = 0,                  \
            .lsb_first = 0,                 \
            .cs_high_active = 0,            \
        },                                  \
    }

class ESP_PanelBus_QSPI: public ESP_PanelBus {
public:
    ESP_PanelBus_QSPI(int cs_io, int sck_io, int d0_io, int d1_io, int d2_io, int d3_io,
                      spi_host_device_t host_id = ESP_PANEL_HOST_QSPI_ID_DEFAULT);
    ESP_PanelBus_QSPI(int cs_io, spi_host_device_t host_id = ESP_PANEL_HOST_QSPI_ID_DEFAULT);
    ESP_PanelBus_QSPI(const spi_bus_config_t &host_config, const esp_lcd_panel_io_spi_config_t &io_config,
                      spi_host_device_t host_id = ESP_PANEL_HOST_QSPI_ID_DEFAULT);
    ESP_PanelBus_QSPI(const esp_lcd_panel_io_spi_config_t &io_config,
                      spi_host_device_t host_id = ESP_PANEL_HOST_QSPI_ID_DEFAULT);
    ~ESP_PanelBus_QSPI() override;

    bool begin(void) override;

private:
    spi_host_device_t host_id;
    spi_bus_config_t host_config;
    esp_lcd_panel_io_spi_config_t io_config;
};
