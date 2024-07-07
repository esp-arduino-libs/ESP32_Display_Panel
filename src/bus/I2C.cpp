/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdlib.h>
#include <string.h>
#include "ESP_PanelLog.h"
#include "I2C.h"

static const char *TAG = "ESP_PanelBus_I2C";

ESP_PanelBus_I2C::ESP_PanelBus_I2C(int scl_io, int sda_io, const esp_lcd_panel_io_i2c_config_t &io_config):
    ESP_PanelBus((int)ESP_PANEL_HOST_I2C_ID_DEFAULT, ESP_PANEL_BUS_TYPE_I2C, true),
    host_config((i2c_config_t)ESP_PANEL_HOST_I2C_CONFIG_DEFAULT(scl_io, sda_io)),
    io_config(io_config)
{
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(const i2c_config_t &host_config, const esp_lcd_panel_io_i2c_config_t &io_config,
                                   i2c_port_t host_id):
    ESP_PanelBus((int)host_id, ESP_PANEL_BUS_TYPE_I2C, true),
    host_config(host_config),
    io_config(io_config)
{
}

ESP_PanelBus_I2C::ESP_PanelBus_I2C(const esp_lcd_panel_io_i2c_config_t &io_config, i2c_port_t host_id):
    ESP_PanelBus((int)host_id, ESP_PANEL_BUS_TYPE_I2C, false),
    io_config(io_config)
{
}

ESP_PanelBus_I2C::~ESP_PanelBus_I2C()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (handle == NULL) {
        goto end;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete panel io failed");
    }

    if (host_need_init) {
        if (i2c_driver_delete((i2c_port_t)host_id) != ESP_OK) {
            ESP_LOGE(TAG, "Delete host[%d] driver failed", host_id);
        } else {
            ESP_LOGD(TAG, "Delete host[%d] driver", host_id);
        }
    }

end:
    ESP_LOGD(TAG, "Destroyed");
}

void ESP_PanelBus_I2C::configI2cPullupEnable(bool sda_pullup_en, bool scl_pullup_en)
{
    host_config.sda_pullup_en = sda_pullup_en ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    host_config.scl_pullup_en = scl_pullup_en ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
}

void ESP_PanelBus_I2C::configI2cFreqHz(uint32_t hz)
{
    host_config.master.clk_speed = hz;
}

void ESP_PanelBus_I2C::configI2cAddress(uint32_t address)
{
    io_config.dev_addr = address;
}

void ESP_PanelBus_I2C::configI2cCtrlPhaseBytes(uint32_t num)
{
    io_config.control_phase_bytes = num;
}

void ESP_PanelBus_I2C::configI2cDcBitOffset(uint32_t num)
{
    io_config.dc_bit_offset = num;
}

void ESP_PanelBus_I2C::configI2cCommandBits(uint32_t num)
{
    io_config.lcd_cmd_bits = num;
}

void ESP_PanelBus_I2C::configI2cParamBits(uint32_t num)
{
    io_config.lcd_param_bits = num;
}

void ESP_PanelBus_I2C::configI2cFlags(bool dc_low_on_data, bool disable_control_phase)
{
    io_config.flags.dc_low_on_data = dc_low_on_data;
    io_config.flags.disable_control_phase = disable_control_phase;
}

uint32_t ESP_PanelBus_I2C::getI2cAddress(void)
{
    return io_config.dev_addr;
}

bool ESP_PanelBus_I2C::begin(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (host_need_init) {
        ESP_PANEL_CHECK_ERR_RET(i2c_param_config((i2c_port_t)host_id, &host_config), false, "Configure host[%d] failed", host_id);
        ESP_PANEL_CHECK_ERR_RET(i2c_driver_install((i2c_port_t)host_id, host_config.mode, 0, 0, 0), false,
                                "Install host[%d] failed", host_id);
        ESP_LOGD(TAG, "Init host[%d]", (int)host_id);
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)host_id, &io_config, &handle), false,
                            "Create panel io failed");
    ESP_LOGD(TAG, "Panel IO @%p created", handle);

    return true;
}
