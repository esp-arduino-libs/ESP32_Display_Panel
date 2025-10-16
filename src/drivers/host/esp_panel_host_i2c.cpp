/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "utils/esp_panel_utils_log.h"
#include "esp_panel_host_i2c.hpp"

namespace esp_panel::drivers {

HostI2C::~HostI2C()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (isOverState(State::BEGIN)) {
        ESP_UTILS_CHECK_ERROR_EXIT(
            i2c_del_master_bus(static_cast<i2c_master_bus_handle_t>(handle_)), "I2C driver delete failed"
        );
        ESP_UTILS_LOGD("Delete I2C host(%d)", getID());

        setState(State::DEINIT);
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool HostI2C::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (isOverState(State::BEGIN)) {
        goto end;
    }

    {
        ESP_UTILS_CHECK_ERROR_RETURN(
            i2c_new_master_bus(&config_, reinterpret_cast<i2c_master_bus_handle_t *>(&handle_)), false,
            "I2C new master bus failed"
        );
        ESP_UTILS_LOGD("Initialize I2C host(%d)", getID());
    }

    setState(State::BEGIN);

end:
    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool HostI2C::calibrateConfig(const i2c_master_bus_config_t &config)
{
    if (memcmp(&config, &this->config_, sizeof(i2c_master_bus_config_t))) {
        ESP_UTILS_LOGI(
            "\n{Original config}\n"
            "\t- [i2c_port]: %d\n"
            "\t- [sda_io_num]: %d\n"
            "\t- [scl_io_num]: %d\n"
            "\t- [clk_source]: %d\n"
            "\t- [glitch_ignore_cnt]: %d\n"
            "\t- [intr_priority]: %d\n"
            "\t- [trans_queue_depth]: %d\n"
            "\t- {flags}\n"
            "\t\t- [enable_internal_pullup]: %d\n"
            "\t\t- [allow_pd]: %d\n"
            , static_cast<int>(this->config_.i2c_port)
            , static_cast<int>(this->config_.sda_io_num)
            , static_cast<int>(this->config_.scl_io_num)
            , static_cast<int>(this->config_.clk_source)
            , static_cast<int>(this->config_.glitch_ignore_cnt)
            , static_cast<int>(this->config_.intr_priority)
            , static_cast<int>(this->config_.trans_queue_depth)
            , static_cast<int>(this->config_.flags.enable_internal_pullup)
            , static_cast<int>(this->config_.flags.allow_pd)
        );
        ESP_UTILS_LOGI(
            "\n{New config}\n"
            "\t- [i2c_port]: %d\n"
            "\t- [sda_io_num]: %d\n"
            "\t- [scl_io_num]: %d\n"
            "\t- [clk_source]: %d\n"
            "\t- [glitch_ignore_cnt]: %d\n"
            "\t- [intr_priority]: %d\n"
            "\t- [trans_queue_depth]: %d\n"
            "\t- {flags}\n"
            "\t\t- [enable_internal_pullup]: %d\n"
            "\t\t- [allow_pd]: %d\n"
            , static_cast<int>(config.i2c_port)
            , static_cast<int>(config.sda_io_num)
            , static_cast<int>(config.scl_io_num)
            , static_cast<int>(config.clk_source)
            , static_cast<int>(config.glitch_ignore_cnt)
            , static_cast<int>(config.intr_priority)
            , static_cast<int>(config.trans_queue_depth)
            , static_cast<int>(config.flags.enable_internal_pullup)
            , static_cast<int>(config.flags.allow_pd)
        );
        ESP_UTILS_CHECK_FALSE_RETURN(false, false, "Config mismatch");
    }

    return true;
}

} // namespace esp_panel::drivers
