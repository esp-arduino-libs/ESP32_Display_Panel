/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>
#include "ESP_PanelLog.h"
#include "ESP_PanelHost.h"

using namespace std;

static const char *TAG = "ESP_PanelHost";

ESP_PanelHost::ESP_PanelHost()
{
}

ESP_PanelHost::~ESP_PanelHost()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (!del()) {
        ESP_LOGE(TAG, "Delete panel host failed");
    }
    ESP_LOGD(TAG, "Destroyed");
}

bool ESP_PanelHost::addHostI2C(const i2c_config_t &host_config, i2c_port_t host_id)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    auto ret = _i2c_host_config_map.find(host_id);
    if (ret == _i2c_host_config_map.end()) {
        _i2c_host_config_map.insert(pair<i2c_port_t, i2c_config_t>(host_id, host_config));
        ESP_LOGD(TAG, "Add host I2C[%d]", (int)host_id);

        return true;
    }
    ESP_LOGD(TAG, "Host I2C[%d] is already exist", (int)host_id);
    ESP_PANEL_CHECK_FALSE_RET(!memcmp(&ret->second, &host_config, sizeof(i2c_config_t)), false,
                              "Attempt to add with a different configuration");

    return true;
}

bool ESP_PanelHost::addHostI2C(int scl_io, int sda_io, i2c_port_t host_id)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    i2c_config_t host_config = ESP_PANEL_HOST_I2C_CONFIG_DEFAULT(scl_io, sda_io);

    auto ret = _i2c_host_config_map.find(host_id);
    if (ret == _i2c_host_config_map.end()) {
        _i2c_host_config_map.insert(pair<i2c_port_t, i2c_config_t>(host_id, host_config));
        ESP_LOGD(TAG, "Add host I2C[%d]", (int)host_id);

        return true;
    }
    ESP_LOGD(TAG, "Host I2C[%d] is already exist", (int)host_id);
    ESP_PANEL_CHECK_FALSE_RET(!memcmp(&ret->second, &host_config, sizeof(i2c_config_t)), false,
                              "Attempt to add with a different configuration");

    return true;
}

bool ESP_PanelHost::addHostSPI(const spi_bus_config_t &host_config, spi_host_device_t host_id)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    auto ret = _spi_host_config_map.find(host_id);
    if (ret == _spi_host_config_map.end()) {
        _spi_host_config_map.insert(pair<spi_host_device_t, spi_bus_config_t>(host_id, host_config));
        ESP_LOGD(TAG, "Add host SPI[%d]", (int)host_id);

        return true;
    }
    ESP_LOGD(TAG, "Host SPI[%d] is already exist", (int)host_id);

    ESP_PANEL_CHECK_FALSE_RET(compare_spi_host_config(ret->second, host_config), false,
                              "Attempt to add with a different configuration");

    return true;
}

bool ESP_PanelHost::addHostSPI(int sck_io, int sda_io, int sdo_io, spi_host_device_t host_id)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    spi_bus_config_t host_config = ESP_PANEL_HOST_SPI_CONFIG_DEFAULT(sck_io, sda_io, sdo_io);

    auto ret = _spi_host_config_map.find(host_id);
    if (ret == _spi_host_config_map.end()) {
        _spi_host_config_map.insert(pair<spi_host_device_t, spi_bus_config_t>(host_id, host_config));
        ESP_LOGD(TAG, "Add host SPI[%d]", (int)host_id);

        return true;
    }
    ESP_LOGD(TAG, "Host SPI[%d] is already exist", (int)host_id);

    ESP_PANEL_CHECK_FALSE_RET(compare_spi_host_config(ret->second, host_config), false,
                              "Attempt to add with a different configuration");

    return true;
}

bool ESP_PanelHost::addHostQSPI(const spi_bus_config_t &host_config, spi_host_device_t host_id)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    auto ret = _spi_host_config_map.find(host_id);
    if (ret == _spi_host_config_map.end()) {
        _spi_host_config_map.insert(pair<spi_host_device_t, spi_bus_config_t>(host_id, host_config));
        ESP_LOGD(TAG, "Add host SPI[%d]", (int)host_id);

        return true;
    }
    ESP_LOGD(TAG, "Host SPI[%d] is already exist", (int)host_id);
    ESP_PANEL_CHECK_FALSE_RET(!memcmp(&ret->second, &host_config, sizeof(spi_bus_config_t)), false,
                              "Attempt to add with a different configuration");

    return true;
}

bool ESP_PanelHost::addHostQSPI(int sck_io, int d0_io, int d1_io, int d2_io, int d3_io, spi_host_device_t host_id)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    spi_bus_config_t host_config = ESP_PANEL_HOST_QSPI_CONFIG_DEFAULT(sck_io, d0_io, d1_io, d2_io, d3_io);

    auto ret = _spi_host_config_map.find(host_id);
    if (ret == _spi_host_config_map.end()) {
        _spi_host_config_map.insert(pair<spi_host_device_t, spi_bus_config_t>(host_id, host_config));
        ESP_LOGD(TAG, "Add host SPI[%d]", (int)host_id);

        return true;
    }
    ESP_LOGD(TAG, "Host SPI[%d] is already exist", (int)host_id);
    ESP_PANEL_CHECK_FALSE_RET(!memcmp(&ret->second, &host_config, sizeof(spi_bus_config_t)), false,
                              "Attempt to add with a different configuration");

    return true;
}

bool ESP_PanelHost::begin(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    // Initialize all I2C hosts
    if (_i2c_host_config_map.size() > 0) {
        for (auto &it : _i2c_host_config_map) {
            ESP_PANEL_CHECK_ERR_RET(i2c_param_config(it.first, &it.second), false, "I2C[%d] config param failed", it.first);
            ESP_PANEL_CHECK_ERR_RET(i2c_driver_install(it.first, it.second.mode, 0, 0, 0), false, "I2C[%d] install driver failed",
                                    it.first);
            ESP_LOGD(TAG, "Initialize host I2C[%d]", (int)it.first);
        }
    }

    // Initialize all SPI hosts
    if (_spi_host_config_map.size() > 0) {
        for (auto &it : _spi_host_config_map) {
            ESP_PANEL_CHECK_ERR_RET(spi_bus_initialize(it.first, &it.second, SPI_DMA_CH_AUTO), false, "SPI[%d] initialize failed",
                                    it.first);
            ESP_LOGD(TAG, "Initialize host SPI[%d]", (int)it.first);
        }
    }

    return true;
}

bool ESP_PanelHost::del(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    // Uninstall all I2C hosts
    if (_i2c_host_config_map.size() > 0) {
        for (auto &it : _i2c_host_config_map) {
            ESP_PANEL_CHECK_ERR_RET(i2c_driver_delete(it.first), false, "I2C[%d] delete driver failed", it.first);
            ESP_LOGD(TAG, "Delete host I2C[%d]", (int)it.first);
        }
        _i2c_host_config_map.clear();
    }

    // Uninstall all SPI hosts
    if (_spi_host_config_map.size() > 0) {
        for (auto &it : _spi_host_config_map) {
            ESP_PANEL_CHECK_ERR_RET(spi_bus_free(it.first), false, "SPI[%d] free failed", it.first);
            ESP_LOGD(TAG, "Delete host SPI[%d]", (int)it.first);
        }
        _spi_host_config_map.clear();
    }

    return true;
}

bool ESP_PanelHost::compare_spi_host_config(spi_bus_config_t &old_config, const spi_bus_config_t &new_config)
{
    spi_bus_config_t temp_config = { };
    memcpy(&temp_config, &new_config, sizeof(spi_bus_config_t));

    if (temp_config.miso_io_num == -1) {
        temp_config.miso_io_num = old_config.miso_io_num;
    } else if (old_config.miso_io_num == -1) {
        old_config.miso_io_num = temp_config.miso_io_num;
    }

    return !memcmp(&old_config, &temp_config, sizeof(spi_bus_config_t));
}
