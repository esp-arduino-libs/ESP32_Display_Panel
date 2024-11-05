/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "host/ESP_PanelHost.h"
#include "ESP_PanelBus.h"

/**
 * @brief Macro for QSPI panel IO configuration
 *
 */
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

/**
 * @brief QSPI bus object class
 *
 * @note  This class is a derived class of `ESP_PanelBus`, user can use it directly
 */
class ESP_PanelBus_QSPI: public ESP_PanelBus {
public:
    /**
     * @brief Construct a QSPI bus object in a common way, the host will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_IO_QSPI_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param cs_io   QSPI CS pin
     * @param sck_io  QSPI SCK pin
     * @param d0_io   QSPI D0 pin
     * @param d1_io   QSPI D1 pin
     * @param d2_io   QSPI D2 pin
     * @param d3_io   QSPI D3 pin
     */
    ESP_PanelBus_QSPI(int cs_io, int sck_io, int d0_io, int d1_io, int d2_io, int d3_io);

    /**
     * @brief Construct a QSPI bus object in a common way, the host needs to be initialized by the user
     *
     * @note  This function uses some default values (ESP_PANEL_IO_QSPI_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param cs_io QSPI CS pin
     */
    ESP_PanelBus_QSPI(int cs_io);

    /**
     * @brief Construct a QSPI bus object in a complex way, the host will be initialized by the driver
     *
     * @param host_config QSPI host configuration
     * @param io_config   QSPI panel IO configuration
     * @param host_id     QSPI host ID, default is `ESP_PANEL_HOST_SPI_ID_DEFAULT`
     */
    ESP_PanelBus_QSPI(const spi_bus_config_t &host_config, const esp_lcd_panel_io_spi_config_t &io_config,
                      spi_host_device_t host_id = ESP_PANEL_HOST_SPI_ID_DEFAULT);

    /**
     * @brief Construct a QSPI bus object in a complex way, the host needs to be initialized by the user
     *
     * @param io_config QSPI panel IO configuration
     * @param host_id   QSPI host ID, default is `ESP_PANEL_HOST_SPI_ID_DEFAULT`
     */
    ESP_PanelBus_QSPI(const esp_lcd_panel_io_spi_config_t &io_config, spi_host_device_t host_id = ESP_PANEL_HOST_SPI_ID_DEFAULT);

    /**
     * @brief Destroy the QSPI bus object
     *
     */
    ~ESP_PanelBus_QSPI() override;

    /**
     * @brief Here are some functions to configure the QSPI bus object. These functions should be called before `begin()`
     *
     */
    void configQspiMode(uint8_t mode);
    void configQspiFreqHz(uint32_t hz);
    void configQspiTransQueueDepth(uint8_t depth);

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     */
    bool begin(void) override;

private:
    spi_bus_config_t host_config;
    esp_lcd_panel_io_spi_config_t io_config;
};
