/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <map>
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "sdkconfig.h"

/**
 * I2C Host Default Configuration
 *
 */
#define ESP_PANEL_HOST_I2C_CONFIG_DEFAULT(scl_io, sda_io)       \
    {                                                           \
        .mode = I2C_MODE_MASTER,                                \
        .sda_io_num = sda_io,                                   \
        .scl_io_num = scl_io,                                   \
        .sda_pullup_en = GPIO_PULLUP_ENABLE,                    \
        .scl_pullup_en = GPIO_PULLUP_ENABLE,                    \
        .master = {                                             \
            .clk_speed = 400000,                                \
        },                                                      \
        .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,               \
    }

/**
 * SPI & QSPI Host Default Configuration
 *
 */
/* Refer to `hal/spi_ll.h` in SDK (ESP-IDF) */
#ifdef CONFIG_IDF_TARGET_ESP32
#define SPI_MAX_TRANSFER_SIZE   ((1 << 24) >> 3)
#elif CONFIG_IDF_TARGET_ESP32S2
#define SPI_MAX_TRANSFER_SIZE   ((1 << 23) >> 3)
#elif CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32H2
#define SPI_MAX_TRANSFER_SIZE   ((1 << 18) >> 3)
#endif
#define ESP_PANEL_HOST_SPI_CONFIG_DEFAULT(sck_io, sda_io) \
    {                                                     \
        .mosi_io_num = sda_io,                            \
        .miso_io_num = -1,                                \
        .sclk_io_num = sck_io,                            \
        .quadwp_io_num = -1,                              \
        .quadhd_io_num = -1,                              \
        .data4_io_num = -1,                               \
        .data5_io_num = -1,                               \
        .data6_io_num = -1,                               \
        .data7_io_num = -1,                               \
        .max_transfer_sz = SPI_MAX_TRANSFER_SIZE,         \
        .flags = SPICOMMON_BUSFLAG_MASTER,                \
        .intr_flags = 0,                                  \
    }
#define ESP_PANEL_HOST_QSPI_CONFIG_DEFAULT(sck_io, d0_io, d1_io, d2_io, d3_io) \
    {                                             \
        .data0_io_num = d0_io,                     \
        .data1_io_num = d1_io,                        \
        .sclk_io_num = sck_io,                    \
        .data2_io_num = d2_io,                   \
        .data3_io_num = d3_io,                   \
        .data4_io_num = -1,                    \
        .data5_io_num = -1,                    \
        .data6_io_num = -1,                       \
        .data7_io_num = -1,                       \
        .max_transfer_sz = SPI_MAX_TRANSFER_SIZE, \
        .flags = SPICOMMON_BUSFLAG_MASTER,        \
        .intr_flags = 0,                          \
    }

class ESP_PanelHost {
public:
    ESP_PanelHost();
    ~ESP_PanelHost();

    void init();
    bool begin(void);

    bool addHostI2C(const i2c_config_t &host_config, i2c_port_t host_id);
    bool addHostI2C(int scl_io, int sda_io, i2c_port_t host_id);

    bool addHostSPI(const spi_bus_config_t &host_config, spi_host_device_t host_id);
    bool addHostSPI(int sck_io, int sda_io, spi_host_device_t host_id);

    bool addHostQSPI(const spi_bus_config_t &host_config, spi_host_device_t host_id);
    bool addHostQSPI(int sck_io, int d0_io, int d1_io, int d2_io, int d3_io, spi_host_device_t host_id);

private:
    std::map<i2c_port_t, i2c_config_t> _i2c_host_config_map;
    std::map<spi_host_device_t, spi_bus_config_t> _spi_host_config_map;
};
