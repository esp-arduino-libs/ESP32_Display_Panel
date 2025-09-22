/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_bus_conf_internal.h"
#if ESP_PANEL_DRIVERS_BUS_ENABLE_I2C

#include "inttypes.h"
#include "utils/esp_panel_utils_log.h"
#include "drivers/host/esp_panel_host_i2c.hpp"
#include "esp_panel_bus_i2c.hpp"

namespace esp_panel::drivers {

void BusI2C::Config::convertPartialToFull()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (isHostConfigValid() && std::holds_alternative<HostPartialConfig>(host.value())) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printHostConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<HostPartialConfig>(host.value());
        host = HostFullConfig{
            .i2c_port = static_cast<i2c_port_t>(host_id),
            .sda_io_num = static_cast<gpio_num_t>(config.sda_io_num),
            .scl_io_num = static_cast<gpio_num_t>(config.scl_io_num),
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags = {
                .enable_internal_pullup = config.enable_internal_pullup,
            },
        };
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::Config::printHostConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (!isHostConfigValid()) {
        ESP_UTILS_LOGI("\n\t{Host config}[skipped]");
        goto end;
    }

    if (isHostConfigValid() && std::holds_alternative<HostFullConfig>(host.value())) {
        auto &config = std::get<HostFullConfig>(host.value());
        ESP_UTILS_LOGI(
            "\n\t{Host config}[full]\n"
            "\t\t-> [i2c_port]: %d\n"
            "\t\t-> [sda_io_num]: %d\n"
            "\t\t-> [scl_io_num]: %d\n"
            "\t\t-> [clk_source]: %d\n"
            "\t\t-> [glitch_ignore_cnt]: %d\n"
            "\t\t-> [intr_priority]: %d\n"
            "\t\t-> [trans_queue_depth]: %d\n"
            "\t\t-> [flags]:\n"
            "\t\t\t-> [enable_internal_pullup]: %d\n"
            "\t\t\t-> [allow_pd]: %d\n"
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
    } else {
        auto &config = std::get<HostPartialConfig>(host.value());
        ESP_UTILS_LOGI(
            "\n\t{Host config}[partial]\n"
            "\t\t-> [id]: %d\n"
            "\t\t-> [sda_io_num]: %d\n"
            "\t\t-> [scl_io_num]: %d\n"
            "\t\t-> [enable_internal_pullup]: %d\n"
            , static_cast<int>(host_id)
            , static_cast<int>(config.sda_io_num)
            , static_cast<int>(config.scl_io_num)
            , static_cast<int>(config.enable_internal_pullup)
        );
    }

end:
    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::Config::printControlPanelConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_LOGI(
        "\n\t{Control panel config}[full]"
        "\n\t\t-> [host_id]: %d"
        "\n\t\t-> [dev_addr]: 0x%02" PRIX32
        "\n\t\t-> [control_phase_bytes]: %d"
        "\n\t\t-> [dc_bit_offset]: %d"
        "\n\t\t-> [lcd_cmd_bits]: %d"
        "\n\t\t-> [lcd_param_bits]: %d"
        "\n\t\t-> {flags}"
        "\n\t\t\t-> [dc_low_on_data]: %d"
        "\n\t\t\t-> [disable_control_phase]: %d"
        , static_cast<int>(host_id)
        , control_panel.dev_addr
        , static_cast<int>(control_panel.control_phase_bytes)
        , static_cast<int>(control_panel.dc_bit_offset)
        , static_cast<int>(control_panel.lcd_cmd_bits)
        , static_cast<int>(control_panel.lcd_param_bits)
        , static_cast<int>(control_panel.flags.dc_low_on_data)
        , static_cast<int>(control_panel.flags.disable_control_phase)
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

BusI2C::~BusI2C()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BusI2C::configI2C_HostSkipInit()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    _config.host = std::nullopt;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::configI2C_PullupEnable(bool enable)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");
    ESP_UTILS_CHECK_FALSE_RETURN(!isHostSkipInit(), false, "Host is skipped initialization");

    ESP_UTILS_LOGD("Param: enable(%d)", enable);
    auto &host_config = getHostFullConfig();
    host_config.flags.enable_internal_pullup = enable;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::configI2C_FreqHz(uint32_t hz)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");
    ESP_UTILS_CHECK_FALSE_RETURN(!isHostSkipInit(), false, "Host is skipped initialization");

    ESP_UTILS_LOGD("Param: hz(%d)", static_cast<int>(hz));
    getControlPanelFullConfig().scl_speed_hz = hz;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::configI2C_Address(uint8_t address)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: address(0x%02X)", address);
    getControlPanelFullConfig().dev_addr = address;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::configI2C_CtrlPhaseBytes(uint8_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    getControlPanelFullConfig().control_phase_bytes = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::configI2C_DC_BitOffset(uint8_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    getControlPanelFullConfig().dc_bit_offset = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::configI2C_CommandBits(uint8_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    getControlPanelFullConfig().lcd_cmd_bits = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::configI2C_ParamBits(uint8_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    getControlPanelFullConfig().lcd_param_bits = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::configI2C_Flags(bool dc_low_on_data, bool disable_control_phase)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: dc_low_on_data(%d), disable_control_phase(%d)", dc_low_on_data, disable_control_phase);
    auto &config = getControlPanelFullConfig();
    config.flags.dc_low_on_data = dc_low_on_data;
    config.flags.disable_control_phase = disable_control_phase;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::init()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Already initialized");

    // Convert the partial configuration to full configuration
    _config.convertPartialToFull();
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
    _config.printHostConfig();
    _config.printControlPanelConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG

    // Get the host instance if not skipped
    if (!isHostSkipInit()) {
        auto host_id = getConfig().host_id;
        _host = HostI2C::getInstance(host_id, getHostFullConfig());
        ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get I2C host(%d) instance failed", host_id);
        ESP_UTILS_LOGD("Get I2C host(%d) instance", host_id);
    }

    setState(State::INIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::BEGIN), false, "Already begun");

    // Initialize the bus if not initialized
    if (!isOverState(State::INIT)) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    // Startup the host if not skipped
    auto host_id = getConfig().host_id;
    if (_host != nullptr) {
        ESP_UTILS_CHECK_FALSE_RETURN(_host->begin(), false, "Begin I2C host(%d) failed", host_id);
        ESP_UTILS_LOGD("Begin I2C host(%d)", host_id);
    }

    // Create the control panel
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_i2c(
            reinterpret_cast<i2c_master_bus_handle_t>(_host->getNativeHandle()), &getControlPanelFullConfig(),
            &control_panel
        ), false, "create control panel failed"
    );
    ESP_UTILS_LOGD("Create control panel @%p", control_panel);

    setState(State::BEGIN);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::del()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    // Delete the control panel if valid
    if (isControlPanelValid()) {
        ESP_UTILS_CHECK_FALSE_RETURN(delControlPanel(), false, "Delete control panel failed");
    }

    // Release the host instance if valid
    if (_host != nullptr) {
        _host = nullptr;
        auto host_id = getConfig().host_id;
        ESP_UTILS_CHECK_FALSE_RETURN(
            HostI2C::tryReleaseInstance(host_id), false, "Release I2C host(%d) failed", host_id
        );
    }

    setState(State::DEINIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

BusI2C::HostFullConfig &BusI2C::getHostFullConfig()
{
    if (std::holds_alternative<HostPartialConfig>(_config.host.value())) {
        _config.convertPartialToFull();
    }

    return std::get<HostFullConfig>(_config.host.value());
}

BusI2C::ControlPanelFullConfig &BusI2C::getControlPanelFullConfig()
{
    return _config.control_panel;
}

} // namespace esp_panel::drivers

#endif // ESP_PANEL_DRIVERS_BUS_ENABLE_I2C
