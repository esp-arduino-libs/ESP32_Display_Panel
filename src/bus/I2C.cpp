/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>

#include "private/CheckResult.h"
#include "I2C.h"

static const char *TAG = "ESP_PanelBus_I2C";

ESP_PanelBus_I2C::ESP_PanelBus_I2C(const esp_lcd_panel_io_i2c_config_t *io_config, const i2c_config_t *host_config, i2c_port_t host_id):
    host_id(host_id)
{
    CHECK_NULL_RETURN(io_config);
    CHECK_NULL_RETURN(host_config);

    this->io_config = *io_config;
    this->host_config = *host_config;
    flags.host_need_init = true;

    if (this->io_config.on_color_trans_done == NULL) {
        ctx.bus = this;
        this->io_config.on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)callback;
        this->io_config.user_ctx = (void *)&ctx;
    }
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(const esp_lcd_panel_io_i2c_config_t *io_config, i2c_port_t host_id):
    host_id(host_id)
{
    CHECK_NULL_RETURN(io_config);

    this->io_config = *io_config;
    flags.host_need_init = false;

    if (this->io_config.on_color_trans_done == NULL) {
        ctx.bus = this;
        this->io_config.on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)callback;
        this->io_config.user_ctx = (void *)&ctx;
    }
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(uint8_t address, int scl, int sda):
    host_id(I2C_HOST_ID_DEFAULT),
    host_config((i2c_config_t)I2C_HOST_CONFIG_DEFAULT(scl, sda)),
    io_config((esp_lcd_panel_io_i2c_config_t)I2C_PANEL_IO_CONFIG_DEFAULT(address))
{
    flags.host_need_init = true;
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(uint8_t address):
    host_id(I2C_HOST_ID_DEFAULT),
    io_config((esp_lcd_panel_io_i2c_config_t)I2C_PANEL_IO_CONFIG_DEFAULT(address))
{
    flags.host_need_init = false;
}

ESP_PanelBus_I2C::~ESP_PanelBus_I2C()
{
    if (handle) {
        del();
        if (flags.host_need_init) {
            i2c_driver_delete(host_id);
        }
    }
}

void ESP_PanelBus_I2C::init(void)
{
    if (flags.host_need_init) {
        CHECK_ERROR_RETURN(i2c_param_config(host_id, &host_config));
        CHECK_ERROR_RETURN(i2c_driver_install(host_id, host_config.mode, 0, 0, 0));
    }
    CHECK_ERROR_RETURN(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)host_id, &io_config, &handle));
}
