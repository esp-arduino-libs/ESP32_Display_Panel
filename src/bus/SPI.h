/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELBUS_SPI_H
#define ESP_PANELBUS_SPI_H

#include <stdint.h>

#include "driver/spi_master.h"
#include "esp_idf_version.h"
#include "esp_intr_alloc.h"
#include "esp_lcd_panel_io.h"

#include "ESP_PanelBus.h"

/* Refer to `hal/spi_ll.h` */
#ifdef CONFIG_IDF_TARGET_ESP32
#define SPI_MAX_TRANSFER_SIZE   ((1 << 24) >> 3)
#elif CONFIG_IDF_TARGET_ESP32S2
#define SPI_MAX_TRANSFER_SIZE   ((1 << 23) >> 3)
#elif CONFIG_IDF_TARGET_ESP32S3
#define SPI_MAX_TRANSFER_SIZE   ((1 << 18) >> 3)
#elif CONFIG_IDF_TARGET_ESP32C3
#define SPI_MAX_TRANSFER_SIZE   ((1 << 18) >> 3)
#endif

#define SPI_HOST_ID_DEFAULT         (SPI2_HOST)

#define SPI_HOST_CONFIG_DEFAULT(clk_io, mosi_io, miso_io)       \
    {                                                           \
        .mosi_io_num = mosi_io,                                 \
        .miso_io_num = miso_io,                                 \
        .sclk_io_num = clk_io,                                  \
        .quadwp_io_num = -1,                                    \
        .quadhd_io_num = -1,                                    \
        .data4_io_num = -1,                                     \
        .data5_io_num = -1,                                     \
        .data6_io_num = -1,                                     \
        .data7_io_num = -1,                                     \
        .max_transfer_sz = SPI_MAX_TRANSFER_SIZE,               \
        .flags = SPICOMMON_BUSFLAG_MASTER,                      \
        .intr_flags = 0,                                        \
    }

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
#define SPI_PANEL_IO_CONFIG_DEFAULT(cs_io, dc_io, callback, callback_ctx)       \
    {                                                           \
        .cs_gpio_num = cs_io,                                   \
        .dc_gpio_num = dc_io,                                   \
        .spi_mode = 0,                                          \
        .pclk_hz = SPI_MASTER_FREQ_40M,                         \
        .trans_queue_depth = 10,                                \
        .on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)callback, \
        .user_ctx = callback_ctx,                               \
        .lcd_cmd_bits = 8,                                      \
        .lcd_param_bits = 8,                                    \
        .flags = {                                              \
            .dc_as_cmd_phase = 0,                               \
            .dc_low_on_data = 0,                                \
            .octal_mode = 0,                                    \
            .lsb_first = 0,                                     \
        },                                                      \
    }
#else
#define SPI_PANEL_IO_CONFIG_DEFAULT(cs_io, dc_io, callback, callback_ctx)       \
    {                                                           \
        .cs_gpio_num = cs_io,                                   \
        .dc_gpio_num = dc_io,                                   \
        .spi_mode = 0,                                          \
        .pclk_hz = SPI_MASTER_FREQ_40M,                         \
        .trans_queue_depth = 10,                                \
        .on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)callback, \
        .user_ctx = callback_ctx,                               \
        .lcd_cmd_bits = 8,                                      \
        .lcd_param_bits = 8,                                    \
        .flags = {                                              \
            .dc_low_on_data = 0,                                \
            .octal_mode = 0,                                    \
            .quad_mode = 0,                                     \
            .sio_mode = 0,                                      \
            .lsb_first = 0,                                     \
            .cs_high_active = 0,                                \
        },                                                      \
    }
#endif  /* ESP_IDF_VERSION */

class ESP_PanelBus_SPI: public ESP_PanelBus {
public:
    ESP_PanelBus_SPI(const spi_bus_config_t *host_config, const esp_lcd_panel_io_spi_config_t *io_config, spi_host_device_t host_id = SPI_HOST_ID_DEFAULT);
    ESP_PanelBus_SPI(const esp_lcd_panel_io_spi_config_t *io_config, spi_host_device_t host_id = SPI_HOST_ID_DEFAULT);
    ESP_PanelBus_SPI(int cs_io, int dc_io, int clk_io, int mosi_io, int miso_io = -1);
    ESP_PanelBus_SPI(int cs_io, int dc_io);
    ~ESP_PanelBus_SPI() override;

    void init(void) override;

private:
    spi_host_device_t host_id;
    spi_bus_config_t host_config;
    esp_lcd_panel_io_spi_config_t io_config;
};

#endif
