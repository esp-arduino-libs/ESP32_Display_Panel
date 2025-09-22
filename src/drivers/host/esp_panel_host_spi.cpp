/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "driver/spi_master.h"
#include "esp_panel_host_spi.hpp"

namespace esp_panel::drivers {

HostSPI::~HostSPI()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (isOverState(State::BEGIN)) {
        int id = getID();
        ESP_UTILS_CHECK_ERROR_EXIT(
            spi_bus_free(static_cast<spi_host_device_t>(id)), "Delete SPI host(%d) failed", id
        );
        ESP_UTILS_LOGD("Delete SPI host(%d)", id);

        setState(State::DEINIT);
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool HostSPI::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (isOverState(State::BEGIN)) {
        goto end;
    }

    {
        int id = getID();
        ESP_UTILS_CHECK_ERROR_RETURN(
            spi_bus_initialize(static_cast<spi_host_device_t>(id), &config_, SPI_DMA_CH_AUTO), false,
            "SPI host(%d) initialize failed", id
        );
        ESP_UTILS_LOGD("Initialize SPI host(%d)", id);
    }

    setState(State::BEGIN);

end:
    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool HostSPI::calibrateConfig(const spi_bus_config_t &config)
{
    spi_bus_config_t temp_config = config;

    // Keep the compatibility between SPI and QSPI
    if (temp_config.miso_io_num < 0) {
        temp_config.miso_io_num = this->config_.miso_io_num;
    } else if (this->config_.miso_io_num < 0) {
        this->config_.miso_io_num = temp_config.miso_io_num;
    }
    if (temp_config.quadwp_io_num < 0) {
        temp_config.quadwp_io_num = this->config_.quadwp_io_num;
    } else if (this->config_.quadwp_io_num < 0) {
        this->config_.quadwp_io_num = temp_config.quadwp_io_num;
    }
    if (temp_config.quadhd_io_num < 0) {
        temp_config.quadhd_io_num = this->config_.quadhd_io_num;
    } else if (this->config_.quadhd_io_num < 0) {
        this->config_.quadhd_io_num = temp_config.quadhd_io_num;
    }

    if (memcmp(&config, &this->config_, sizeof(spi_bus_config_t))) {
        ESP_UTILS_LOGI(
            "\n{Original config}\n"
            "\t- mosi_io_num: %d\n"
            "\t- miso_io_num: %d\n"
            "\t- sclk_io_num: %d\n"
            "\t- quadwp_io_num: %d\n"
            "\t- quadhd_io_num: %d\n"
            , this->config_.mosi_io_num
            , this->config_.miso_io_num
            , this->config_.sclk_io_num
            , this->config_.quadwp_io_num
            , this->config_.quadhd_io_num
        );
        ESP_UTILS_LOGI(
            "\n{New config}\n"
            "\t- mosi_io_num: %d\n"
            "\t- miso_io_num: %d\n"
            "\t- sclk_io_num: %d\n"
            "\t- quadwp_io_num: %d\n"
            "\t- quadhd_io_num: %d\n"
            , temp_config.mosi_io_num
            , temp_config.miso_io_num
            , temp_config.sclk_io_num
            , temp_config.quadwp_io_num
            , temp_config.quadhd_io_num
        );
        ESP_UTILS_CHECK_FALSE_RETURN(false, false, "Config mismatch");
    }

    return true;
}

} // namespace esp_panel::drivers
