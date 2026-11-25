/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/i2c_master.h"
#include "utils/esp_panel_utils_log.h"

namespace esp_panel::board {

inline constexpr int M5STACK_POWER_I2C_TIMEOUT_MS = 1000;

inline bool withM5StackPowerDevice(
    i2c_master_bus_handle_t bus, uint8_t address, bool (*operation)(i2c_master_dev_handle_t)
)
{
    const i2c_device_config_t config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = 400 * 1000,
    };
    i2c_master_dev_handle_t device = nullptr;
    ESP_UTILS_CHECK_ERROR_RETURN(
        i2c_master_bus_add_device(bus, &config, &device), false, "Add M5Stack power device failed"
    );

    const bool success = operation(device);
    ESP_UTILS_CHECK_ERROR_RETURN(
        i2c_master_bus_rm_device(device), false, "Remove M5Stack power device failed"
    );
    return success;
}

inline bool readM5StackPowerRegister(i2c_master_dev_handle_t device, uint8_t reg, uint8_t &value)
{
    ESP_UTILS_CHECK_ERROR_RETURN(
        i2c_master_transmit_receive(device, &reg, 1, &value, 1, M5STACK_POWER_I2C_TIMEOUT_MS),
        false, "Read M5Stack power register 0x%02x failed", static_cast<unsigned>(reg)
    );
    return true;
}

inline bool writeM5StackPowerRegister(i2c_master_dev_handle_t device, uint8_t reg, uint8_t value)
{
    const uint8_t data[] = {reg, value};
    ESP_UTILS_CHECK_ERROR_RETURN(
        i2c_master_transmit(device, data, sizeof(data), M5STACK_POWER_I2C_TIMEOUT_MS),
        false, "Write M5Stack power register 0x%02x failed", static_cast<unsigned>(reg)
    );
    return true;
}

} // namespace esp_panel::board
