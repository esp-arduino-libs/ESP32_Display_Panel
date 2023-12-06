/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_lcd_panel_io.h"
#include "driver/i2c.h"
#include "ESP_PanelHost.h"
#include "ESP_PanelBus.h"

#define ESP_PANEL_BUS_I2C_HOST_ID_DEFAULT   (I2C_NUM_0)
#define ESP_PANEL_BUS_I2C_CONFIG_DEFAULT(address) \
    {                                   \
        .dev_addr = address,            \
        .on_color_trans_done = NULL,    \
        .user_ctx = NULL,               \
        .control_phase_bytes = 1,       \
        .dc_bit_offset = 6,             \
        .lcd_cmd_bits = 8,              \
        .lcd_param_bits = 8,            \
        .flags = {                      \
            .dc_low_on_data = 0,        \
            .disable_control_phase = 0, \
        },                              \
    }

class ESP_PanelBus_I2C: public ESP_PanelBus {
public:
    ESP_PanelBus_I2C(uint8_t address, int scl_io, int sda_io, i2c_port_t host_id = ESP_PANEL_BUS_I2C_HOST_ID_DEFAULT);
    ESP_PanelBus_I2C(uint8_t address, i2c_port_t host_id = ESP_PANEL_BUS_I2C_HOST_ID_DEFAULT);
    ESP_PanelBus_I2C(const i2c_config_t host_config, const esp_lcd_panel_io_i2c_config_t io_config,
                     i2c_port_t host_id = ESP_PANEL_BUS_I2C_HOST_ID_DEFAULT);
    ESP_PanelBus_I2C(const esp_lcd_panel_io_i2c_config_t io_config,
                     i2c_port_t host_id = ESP_PANEL_BUS_I2C_HOST_ID_DEFAULT);
    ~ESP_PanelBus_I2C() override;

    bool begin(void) override;

private:
    i2c_port_t host_id;
    i2c_config_t host_config;
    esp_lcd_panel_io_i2c_config_t io_config;
};
