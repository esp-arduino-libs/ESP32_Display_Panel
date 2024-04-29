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
 * @brief Macro for SPI panel IO configuration
 *
 */
#define ESP_PANEL_IO_SPI_CONFIG_DEFAULT(cs_io, dc_io) \
    {                                       \
        .cs_gpio_num = cs_io,               \
        .dc_gpio_num = dc_io,               \
        .spi_mode = 0,                      \
        .pclk_hz = SPI_MASTER_FREQ_40M,     \
        .trans_queue_depth = 10,            \
        .on_color_trans_done = NULL,        \
        .user_ctx = NULL,                   \
        .lcd_cmd_bits = 8,                  \
        .lcd_param_bits = 8,                \
        .flags = {                          \
            .dc_low_on_data = 0,            \
            .octal_mode = 0,                \
            .quad_mode = 0,                 \
            .sio_mode = 0,                  \
            .lsb_first = 0,                 \
            .cs_high_active = 0,            \
        },                                  \
    }

/**
 * @brief SPI bus object class
 *
 * @note  This class is a derived class of `ESP_PanelBus`, user can use it directly
 */
class ESP_PanelBus_SPI: public ESP_PanelBus {
public:
    /**
     * @brief Construct a SPI bus object in a common way, the host will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_HOST_SPI_CONFIG_DEFAULT && ESP_PANEL_IO_SPI_CONFIG_DEFAULT)
     *        to config the bus object, use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param cs_io  SPI CS pin
     * @param dc_io  SPI DC pin
     * @param sck_io SPI SCK pin
     * @param sda_io SPI MOSI pin
     * @param sdo_io SPI MISO pin, default is `-1`
     */
    ESP_PanelBus_SPI(int cs_io, int dc_io, int sck_io, int sda_io, int sdo_io = -1);

    /**
     * @brief Construct a SPI bus object in a common way, the host will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_HOST_SPI_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param sck_io    SPI SCK pin
     * @param mosi_io   SPI MOSI pin
     * @param miso_io   SPI MISO pin
     * @param io_config SPI panel IO configuration
     */
    ESP_PanelBus_SPI(int sck_io, int mosi_io, int miso_io, const esp_lcd_panel_io_spi_config_t &io_config);

    /**
     * @brief Construct a SPI bus object in a common way, the host needs to be initialized by the user
     *
     * @note  This function uses some default values (ESP_PANEL_IO_SPI_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param cs_io SPI CS pin
     * @param dc_io SPI DC pin
     */
    ESP_PanelBus_SPI(int cs_io, int dc_io);

    /**
     * @brief Construct a SPI bus object in a complex way, the host will be initialized by the driver
     *
     * @note  The `init()` function should be called after this function
     *
     * @param host_config SPI host configuration
     * @param io_config   SPI panel IO configuration
     * @param host_id     SPI host ID, default is `ESP_PANEL_HOST_SPI_ID_DEFAULT`
     */
    ESP_PanelBus_SPI(const spi_bus_config_t &host_config, const esp_lcd_panel_io_spi_config_t &io_config,
                     spi_host_device_t host_id = ESP_PANEL_HOST_SPI_ID_DEFAULT);

    /**
     * @brief Construct a SPI bus object in a complex way, the host needs to be initialized by the user
     *
     * @note  The `init()` function should be called after this function
     *
     * @param io_config SPI panel IO configuration
     * @param host_id   SPI host ID, default is `ESP_PANEL_HOST_SPI_ID_DEFAULT`
     */
    ESP_PanelBus_SPI(const esp_lcd_panel_io_spi_config_t &io_config, spi_host_device_t host_id = ESP_PANEL_HOST_SPI_ID_DEFAULT);

    /**
     * @brief Destroy the SPI bus object
     *
     */
    ~ESP_PanelBus_SPI() override;

    /**
     * @brief Here are some functions to configure the SPI bus object. These functions should be called before `begin()`
     *
     */
    void configSpiMode(uint8_t mode);
    void configSpiFreqHz(uint32_t hz);
    void configSpiCommandBits(uint32_t num);
    void configSpiParamBits(uint32_t num);
    void configSpiTransQueueDepth(uint8_t depth);

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
