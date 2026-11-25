/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_board_mosaico.hpp"

#if CONFIG_IDF_TARGET_ESP32S31 && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
#include "board/esp_panel_board.hpp"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_check.h"
#include "esp_efuse.h"
#include "esp_efuse_table.h"
#include "soc/gpio_reg.h"
#include "soc/soc.h"

namespace esp_panel::board {
using namespace drivers;

namespace {

// ESP-Mosaico BSP d1eef04: board pin maps, CO5300 cold-start commands and VCC_3V3 timing.
constexpr char TAG[] = "ESP-Mosaico";
constexpr int LCD_SIZE = 480;
constexpr gpio_num_t VCC_ENABLE = GPIO_NUM_60;
constexpr int VCC_ON = 0;
constexpr int VCC_OFF = 1;
constexpr ledc_mode_t RAMP_MODE = LEDC_LOW_SPEED_MODE;
constexpr ledc_timer_t RAMP_TIMER = LEDC_TIMER_1;
constexpr ledc_channel_t RAMP_CHANNEL = LEDC_CHANNEL_1;

constexpr uint8_t page_select[] = {0x20};
constexpr uint8_t page_default[] = {0x00};
constexpr uint8_t voltage[] = {0x10};
constexpr uint8_t timing[] = {0xA0};
constexpr uint8_t bus_mode[] = {0x80};
constexpr uint8_t rgb565[] = {0x55};
constexpr uint8_t display_control[] = {0x20};
constexpr uint8_t brightness[] = {0xFF};
constexpr uint8_t window[] = {0x00, 0x00, 0x01, 0xDF};
constexpr esp_panel_lcd_vendor_init_cmd_t LCD_INIT[] = {
    {0x11, nullptr, 0, 600},
    {0xFE, page_select, sizeof(page_select), 0},
    {0x19, voltage, sizeof(voltage), 0},
    {0x1C, timing, sizeof(timing), 0},
    {0xFE, page_default, sizeof(page_default), 0},
    {0xC4, bus_mode, sizeof(bus_mode), 0},
    {0x3A, rgb565, sizeof(rgb565), 0},
    {0x53, display_control, sizeof(display_control), 0},
    {0x51, brightness, sizeof(brightness), 0},
    {0x63, brightness, sizeof(brightness), 0},
    {0x2A, window, sizeof(window), 0},
    {0x2B, window, sizeof(window), 0},
    {0x29, nullptr, 0, 600},
};

esp_err_t setVccGpio(int level)
{
    gpio_config_t io = {};
    io.pin_bit_mask = 1ULL << VCC_ENABLE;
    io.mode = GPIO_MODE_OUTPUT;
    ESP_RETURN_ON_ERROR(gpio_set_level(VCC_ENABLE, level), TAG, "Set VCC enable level failed");
    return gpio_config(&io);
}

esp_err_t enableVcc()
{
    constexpr uint32_t mask = 1U << (VCC_ENABLE - 32);
    const bool already_on = (REG_READ(GPIO_ENABLE1_REG) & mask) && !(REG_READ(GPIO_OUT1_REG) & mask);
    ESP_RETURN_ON_ERROR(setVccGpio(already_on ? VCC_ON : VCC_OFF), TAG, "Configure VCC enable failed");
    ESP_RETURN_ON_ERROR(gpio_hold_dis(VCC_ENABLE), TAG, "Release VCC enable hold failed");
    if (already_on) {
        return ESP_OK;
    }

    ledc_timer_config_t timer = {};
    timer.speed_mode = RAMP_MODE;
    timer.duty_resolution = LEDC_TIMER_8_BIT;
    timer.timer_num = RAMP_TIMER;
    timer.freq_hz = 100000;
    timer.clk_cfg = LEDC_AUTO_CLK;
    ESP_RETURN_ON_ERROR(ledc_timer_config(&timer), TAG, "Configure VCC ramp timer failed");

    ledc_channel_config_t channel = {};
    channel.gpio_num = VCC_ENABLE;
    channel.speed_mode = RAMP_MODE;
    channel.channel = RAMP_CHANNEL;
    channel.timer_sel = RAMP_TIMER;
    channel.duty = 255;
    esp_err_t result = ledc_channel_config(&channel);
    if (result == ESP_OK) {
        const esp_err_t installed = ledc_fade_func_install(0);
        result = installed == ESP_ERR_INVALID_STATE ? ESP_OK : installed;
        if (result == ESP_OK) {
            result = ledc_set_fade_with_time(RAMP_MODE, RAMP_CHANNEL, 0, 50);
        }
        if (result == ESP_OK) {
            result = ledc_fade_start(RAMP_MODE, RAMP_CHANNEL, LEDC_FADE_WAIT_DONE);
        }
        if (installed == ESP_OK) {
            ledc_fade_func_uninstall();
        }
        ledc_stop(RAMP_MODE, RAMP_CHANNEL, result == ESP_OK ? VCC_ON : VCC_OFF);
    }

    ESP_RETURN_ON_ERROR(setVccGpio(result == ESP_OK ? VCC_ON : VCC_OFF), TAG, "Restore VCC GPIO mode failed");
    return result;
}

bool startLcdBus(void *context)
{
    auto *lcd = static_cast<Board *>(context)->getLCD();
    auto *bus = lcd ? lcd->getBus() : nullptr;
    ESP_RETURN_ON_FALSE(bus && bus->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_QSPI,
                        false, TAG, "ESP-Mosaico requires a QSPI LCD bus");
    auto *qspi = static_cast<BusQSPI *>(bus);
    ESP_RETURN_ON_FALSE(qspi->isOverState(Bus::State::BEGIN) || qspi->begin(), false, TAG, "Start QSPI bus failed");
    const auto &host = qspi->getConfig().host;
    if (!host) {
        return true;
    }

    const auto *pins_config = std::get_if<BusQSPI::HostFullConfig>(&*host);
    ESP_RETURN_ON_FALSE(pins_config, false, TAG, "Missing QSPI host configuration");
    const int pins[] = {pins_config->sclk_io_num, pins_config->data0_io_num, pins_config->data1_io_num,
                        pins_config->data2_io_num, pins_config->data3_io_num
                       };
    for (int pin : pins) {
        if (gpio_set_drive_capability(static_cast<gpio_num_t>(pin), GPIO_DRIVE_CAP_2) != ESP_OK) {
            ESP_LOGE(TAG, "Set QSPI drive strength failed on GPIO%d", pin);
            return false;
        }
    }
    return true;
}

} // namespace

BoardConfig getMosaicoBoardConfig()
{
#if !ESP_PANEL_DRIVERS_LCD_USE_CO5300 || !ESP_PANEL_DRIVERS_TOUCH_USE_CST9220 || \
    !ESP_PANEL_DRIVERS_BUS_USE_QSPI || !ESP_PANEL_DRIVERS_BUS_USE_I2C || !ESP_PANEL_DRIVERS_BACKLIGHT_USE_CUSTOM
    ESP_LOGE(TAG, "Enable QSPI, I2C, CO5300, CST9220 and Custom backlight in esp_panel_drivers_conf.h");
    return {};
#else
    uint16_t revision = 0;
    if (esp_efuse_read_field_blob(ESP_EFUSE_USER_DATA, &revision, sizeof(revision) * 8) != ESP_OK ||
            (revision != 0x0100 && revision != 0x0101 && revision != 0x0102)) {
        ESP_LOGE(TAG, "Unsupported or unprogrammed ESP-Mosaico board revision (0x%04x)", revision);
        return {};
    }
    const bool v1_0 = revision == 0x0100;

    BusQSPI::Config qspi = {
        .host_id = SPI3_HOST,
        .host = BusQSPI::HostPartialConfig{
            .sclk_io_num = v1_0 ? 44 : 42,
            .data0_io_num = 36,
            .data1_io_num = 51,
            .data2_io_num = 35,
            .data3_io_num = 9,
        },
        .control_panel = BusQSPI::ControlPanelPartialConfig{.cs_gpio_num = 50},
    };
    qspi.convertPartialToFull();
    std::get<BusQSPI::HostFullConfig>(*qspi.host).max_transfer_sz = LCD_SIZE * 8 * sizeof(uint16_t);
    std::get<BusQSPI::ControlPanelFullConfig>(qspi.control_panel).flags.psram_dma_direct = true;

    BusI2C::ControlPanelFullConfig touch_io = ESP_LCD_TOUCH_IO_I2C_CST9220_CONFIG();
    touch_io.scl_speed_hz = 400000;
    touch_io.transaction_timeout_ms = 100;

    BoardConfig config = {
        .name = "Espressif:ESP-Mosaico",
        .lcd = BoardConfig::LCD_Config{
            .bus_config = qspi,
            .device_name = LCD_CO5300::BASIC_ATTRIBUTES_DEFAULT.name,
            .device_config = {
                .device = LCD::DevicePartialConfig{.reset_gpio_num = v1_0 ? 42 : 44},
                .vendor = LCD::VendorPartialConfig{
                    .hor_res = LCD_SIZE,
                    .ver_res = LCD_SIZE,
                    .init_cmds = LCD_INIT,
                    .init_cmds_size = sizeof(LCD_INIT) / sizeof(LCD_INIT[0]),
                },
            },
            .pre_process = {},
        },
        .touch = BoardConfig::TouchConfig{
            .bus_config = BusI2C::Config{
                .host = BusI2C::HostPartialConfig{.sda_io_num = v1_0 ? 0 : 56, .scl_io_num = v1_0 ? 1 : 3},
                .control_panel = touch_io,
            },
            .device_name = TouchCST9220::BASIC_ATTRIBUTES_DEFAULT.name,
            .device_config = {
                .device = Touch::DevicePartialConfig{.x_max = LCD_SIZE - 1, .y_max = LCD_SIZE - 1, .int_gpio_num = 6},
            },
            .pre_process = {},
        },
        .backlight = BoardConfig::BacklightConfig{
            .config = BacklightCustom::Config{},
            .pre_process = {},
        },
        .stage_callbacks = {},
    };
    config.stage_callbacks[BoardConfig::STAGE_CALLBACK_PRE_BOARD_BEGIN] = [](void *) {
        return enableVcc() == ESP_OK;
    };
    config.stage_callbacks[BoardConfig::STAGE_CALLBACK_PRE_LCD_BEGIN] = startLcdBus;
    config.stage_callbacks[BoardConfig::STAGE_CALLBACK_PRE_BACKLIGHT_BEGIN] = [](void *context) {
        auto *board = static_cast<Board *>(context);
        auto *backlight = static_cast<BacklightCustom *>(board->getBacklight());
        backlight->configCallback([](int percent, void *device) {
            return static_cast<LCD_CO5300 *>(device)->setBrightness(percent);
        }, board->getLCD());
        return true;
    };
    return config;
#endif
}

} // namespace esp_panel::board
#endif
