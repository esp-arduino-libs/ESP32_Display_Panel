/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>
#include "ESP_PanelPrivate.h"
#include "I2C.h"

static const char *TAG = "ESP_PanelBus_I2C";

ESP_PanelBus_I2C::ESP_PanelBus_I2C(uint8_t address, int scl_io, int sda_io, i2c_port_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_I2C, true),
    host_id(host_id),
    host_config((i2c_config_t)ESP_PANEL_HOST_I2C_CONFIG_DEFAULT(scl_io, sda_io)),
    io_config((esp_lcd_panel_io_i2c_config_t)ESP_PANEL_BUS_I2C_CONFIG_DEFAULT(address))
{
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(uint8_t address, i2c_port_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_I2C, false),
    host_id(host_id),
    io_config((esp_lcd_panel_io_i2c_config_t)ESP_PANEL_BUS_I2C_CONFIG_DEFAULT(address))
{
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(const i2c_config_t host_config, const esp_lcd_panel_io_i2c_config_t io_config,
                                   i2c_port_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_I2C, true),
    host_id(host_id),
    host_config(host_config),
    io_config(io_config)
{
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(const esp_lcd_panel_io_i2c_config_t io_config, i2c_port_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_I2C, false),
    host_id(host_id),
    io_config(io_config)
{
}

ESP_PanelBus_I2C::~ESP_PanelBus_I2C()
{
    if (handle == NULL) {
        ESP_LOGD(TAG, "Bus is not initialized");
        return;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete panel io failed");
    }

    if (host_need_init) {
        if (i2c_driver_delete(host_id) != ESP_OK) {
            ESP_LOGE(TAG, "Delete host[%d] driver failed", (int)host_id);
        } else {
            ESP_LOGD(TAG, "Delete host[%d] driver", (int)host_id);
        }
    }

    ESP_LOGD(TAG, "Destory");
}

bool ESP_PanelBus_I2C::begin(void)
{
    ENABLE_TAG_PRINT_DEBUG_LOG();

    if (host_need_init) {
        CHECK_ERR_RET(i2c_param_config(host_id, &host_config), false, "Configure host[%d] failed", (int)host_id);
        CHECK_ERR_RET(i2c_driver_install(host_id, host_config.mode, 0, 0, 0), false, "Install host[%d] failed",
                      (int)host_id);
        ESP_LOGD(TAG, "Init host[%d]", (int)host_id);
    }

    CHECK_ERR_RET(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)host_id, &io_config, &handle), false, "Create panel io failed");
    ESP_LOGD(TAG, "Create panel io @%p", handle);

    return true;
}
