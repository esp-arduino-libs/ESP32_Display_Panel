/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_lcd_panel_io.h"
#include "driver/i2c.h"
#include "host/ESP_PanelHost.h"
#include "ESP_PanelBus.h"

/**
 * @brief I2C bus object class
 *
 * @note  This class is a derived class of `ESP_PanelBus`, user can use it directly
 */
class ESP_PanelBus_I2C: public ESP_PanelBus {
public:
    /**
     * @brief Construct a I2C bus object in a common way, the host will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_HOST_I2C_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param scl_io    I2C SCL pin
     * @param sda_io    I2C SDA pin
     * @param io_config I2C panel IO configuration
     */
    ESP_PanelBus_I2C(int scl_io, int sda_io, const esp_lcd_panel_io_i2c_config_t &io_config);

    /**
     * @brief Construct a I2C bus object in a complex way, the host will be initialized by the driver
     *
     * @note  The `init()` function should be called after this function
     *
     * @param host_config I2C host configuration
     * @param io_config   I2C panel IO configuration
     * @param host_id   I2C host ID, default is `ESP_PANEL_HOST_I2C_ID_DEFAULT`
     */
    ESP_PanelBus_I2C(const i2c_config_t &host_config, const esp_lcd_panel_io_i2c_config_t &io_config,
                     i2c_port_t host_id = ESP_PANEL_HOST_I2C_ID_DEFAULT);

    /**
     * @brief Construct a I2C bus object in a complex way, the host needs to be initialized by the user
     *
     * @note  The `init()` function should be called after this function
     *
     * @param io_config I2C panel IO configuration
     * @param host_id   I2C host ID, default is `ESP_PANEL_HOST_I2C_ID_DEFAULT`
     */
    ESP_PanelBus_I2C(const esp_lcd_panel_io_i2c_config_t &io_config, i2c_port_t host_id = ESP_PANEL_HOST_I2C_ID_DEFAULT);

    /**
     * @brief Destroy the I2C bus object
     *
     */
    ~ESP_PanelBus_I2C() override;

    /**
     * @brief Here are some functions to configure the I2C bus object. These functions should be called before `begin()`
     *
     */
    void configI2cPullupEnable(bool sda_pullup_en, bool scl_pullup_en);
    void configI2cFreqHz(uint32_t hz);
    void configI2cAddress(uint32_t address);
    void configI2cCtrlPhaseBytes(uint32_t num);
    void configI2cDcBitOffset(uint32_t num);
    void configI2cCommandBits(uint32_t num);
    void configI2cParamBits(uint32_t num);
    void configI2cFlags(bool dc_low_on_data, bool disable_control_phase);

    uint32_t getI2cAddress(void);

    /**
     * @brief Startup the bus
     *
     * @note  This function should be called after `init()`
     *
     * @return true if success, otherwise false
     */
    bool begin(void) override;

private:
    i2c_config_t host_config;
    esp_lcd_panel_io_i2c_config_t io_config;
};
