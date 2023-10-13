/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELBUS_I2C_H
#define ESP_PANELBUS_I2C_H

#include <stdint.h>

#include "driver/i2c.h"
#include "esp_lcd_panel_io.h"

#include "ESP_PanelBus.h"

#define I2C_HOST_ID_DEFAULT         (I2C_NUM_0)
#define I2C_HOST_CONFIG_DEFAULT(scl_io, sda_io)                 \
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
#define I2C_PANEL_IO_CONFIG_DEFAULT(address, cb)                \
    {                                                           \
        .dev_addr = address,                                    \
        .on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)cb, \
        .user_ctx = NULL,                                       \
        .control_phase_bytes = 1,                               \
        .dc_bit_offset = 6,                                     \
        .lcd_cmd_bits = 8,                                      \
        .lcd_param_bits = 8,                                    \
        .flags = {                                              \
            .dc_low_on_data = 0,                                \
            .disable_control_phase = 0,                         \
        },                                                      \
    }

class ESP_PanelBus_I2C: public ESP_PanelBus {
public:
    ESP_PanelBus_I2C(const i2c_config_t *host_config, const esp_lcd_panel_io_i2c_config_t *io_config, i2c_port_t host_id = I2C_HOST_ID_DEFAULT);
    ESP_PanelBus_I2C(const esp_lcd_panel_io_i2c_config_t *io_config, i2c_port_t host_id = I2C_HOST_ID_DEFAULT);
    ESP_PanelBus_I2C(uint8_t address, int scl_io, int sda_io);
    ESP_PanelBus_I2C(uint8_t address);
    ~ESP_PanelBus_I2C() override;

    void init(void) override;

private:
    i2c_port_t host_id;
    i2c_config_t host_config;
    esp_lcd_panel_io_i2c_config_t io_config;
};

#endif
