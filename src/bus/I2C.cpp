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

ESP_PanelBus_I2C::ESP_PanelBus_I2C(const i2c_config_t *host_config, const esp_lcd_panel_io_i2c_config_t *io_config,
                                   i2c_port_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_I2C, true),
    host_id(host_id),
    host_config(*host_config),
    io_config(*io_config)
{
    if (this->io_config.on_color_trans_done == NULL) {
        callback_ctx.bus = this;
        this->io_config.on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)on_transmit_finish_callback;
        this->io_config.user_ctx = (void *)&callback_ctx;
    }
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(const esp_lcd_panel_io_i2c_config_t *io_config, i2c_port_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_I2C, false),
    host_id(host_id),
    io_config(*io_config)
{
    if (this->io_config.on_color_trans_done == NULL) {
        callback_ctx.bus = this;
        this->io_config.on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)on_transmit_finish_callback;
        this->io_config.user_ctx = (void *)&callback_ctx;
    }
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(uint8_t address, int scl_io, int sda_io):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_I2C, true),
    host_id(I2C_HOST_ID_DEFAULT),
    host_config((i2c_config_t)I2C_HOST_CONFIG_DEFAULT(scl_io, sda_io)),
    io_config((esp_lcd_panel_io_i2c_config_t)I2C_PANEL_IO_CONFIG_DEFAULT(address, on_transmit_finish_callback))
{
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(uint8_t address):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_I2C, false),
    host_id(I2C_HOST_ID_DEFAULT),
    io_config((esp_lcd_panel_io_i2c_config_t)I2C_PANEL_IO_CONFIG_DEFAULT(address, on_transmit_finish_callback))
{
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
    createTransmitFinishSemaphore();
}
