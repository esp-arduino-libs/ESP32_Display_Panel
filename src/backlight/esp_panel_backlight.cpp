/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_idf_version.h"
#include "utils/esp_panel_utils.hpp"
#include "esp_panel_backlight.hpp"

namespace esp_panel {

#define FLAGS_DEFAULT(enable_pwm, light_on_level, callback) \
    {                                          \
        .is_begun = 0,                         \
        .use_pwm = enable_pwm,                 \
        .use_callback = (callback != nullptr), \
        .light_up_level = light_on_level,      \
    }
#define CALLBACK_DEFAULT(callback, usr_data)   \
    {                                          \
        .cbx_function = callback,              \
        .user_data = usr_data,                 \
    }

/**
 * @brief Backlight LEDC default configuration macros
 *
 */
#define LEDC_TIMER_CONFIG_DEFAULT() \
    {                                                   \
        .speed_mode = LEDC_LOW_SPEED_MODE,              \
        .duty_resolution = LEDC_TIMER_10_BIT,           \
        .timer_num = LEDC_TIMER_0,                      \
        .freq_hz = 5000,                                \
        .clk_cfg = LEDC_AUTO_CLK,                       \
    }
#define LEDC_CHANNEL_CONFIG_DEFAULT(io_num, on_level) \
    {                                                   \
        .gpio_num = io_num,                             \
        .speed_mode = LEDC_LOW_SPEED_MODE,              \
        .channel = LEDC_CHANNEL_0,                      \
        .intr_type = LEDC_INTR_DISABLE,                 \
        .timer_sel = LEDC_TIMER_0,                      \
        .duty = 0,                                      \
        .hpoint = 0,                                    \
        .flags = {                                      \
            .output_invert = !on_level,                 \
        },                                              \
    }

/**
 * @brief Backlight GPIO default configuration macro
 *
 */
#define GPIO_CONFIG_DEFAULT(io_num) \
    {                                                   \
        .pin_bit_mask = BIT64(io_num),                  \
        .mode = GPIO_MODE_OUTPUT,                       \
        .pull_up_en = GPIO_PULLUP_DISABLE,              \
        .pull_down_en = GPIO_PULLDOWN_DISABLE,          \
        .intr_type = GPIO_INTR_DISABLE,                 \
    }

Backlight::Backlight(int io_num, bool light_up_level, bool use_pwm):
    _flags(FLAGS_DEFAULT(use_pwm, light_up_level, nullptr))
{
    if (use_pwm) {
        _pwm.timer_config = LEDC_TIMER_CONFIG_DEFAULT();
        _pwm.channel_config = LEDC_CHANNEL_CONFIG_DEFAULT(io_num, light_up_level);
    } else {
        _io_num = io_num;
    }
}

Backlight::Backlight(SetBrightnessCallback callback, void *user_data):
    _flags(FLAGS_DEFAULT(0, 0, callback)),
    _callback(CALLBACK_DEFAULT(callback, user_data))
{
}

Backlight::~Backlight()
{
    if (checkBegun() && !del()) {
        LOGE("Delete failed");
    }

    LOGD("Destroyed");
}

bool Backlight::begin(void)
{
    CHECK_FALSE_RETURN(!checkBegun(), false, "Already begun");
    LOGD("begin start");

    if (_flags.use_callback) {
        CHECK_NULL_RETURN(_callback.cbx_function, false, "Invalid callback function");
        LOGD("Use custom function to control");
    } else if (_flags.use_pwm) {
        LOGD("Use PWM(LEDC) to control");

        CHECK_NOT_ESP_OK_RETURN(ledc_timer_config(&_pwm.timer_config), false, "LEDC timer config failed");
        CHECK_NOT_ESP_OK_RETURN(ledc_channel_config(&_pwm.channel_config), false, "LEDC channel config failed");
    } else {
        CHECK_FALSE_RETURN(_io_num >= 0, false, "Invalid IO number");
        LOGD("Use GPIO to control");

        gpio_config_t io_config = GPIO_CONFIG_DEFAULT(_io_num);
        CHECK_NOT_ESP_OK_RETURN(gpio_config(&io_config), false, "GPIO config failed");
        CHECK_NOT_ESP_OK_RETURN(
            gpio_set_level((gpio_num_t)_io_num, _flags.light_up_level), false, "GPIO set level failed"
        );
    }

    _flags.is_begun = true;
    LOGD("begin end");

    return true;
}

bool Backlight::setBrightness(uint8_t percent)
{
    CHECK_FALSE_RETURN(checkBegun(), false, "Not begun");
    LOGD("Set brightness to %d%%", percent);

    percent = percent > 100 ? 100 : percent;
    if (_flags.use_callback) {
        return _callback.cbx_function(percent, _callback.user_data);
    }
    if (_flags.use_pwm) {
        uint32_t duty_cycle = (BIT(_pwm.timer_config.duty_resolution) * percent) / 100;
        ledc_channel_t channel = _pwm.channel_config.channel;
        ledc_mode_t mode = _pwm.channel_config.speed_mode;

        CHECK_NOT_ESP_OK_RETURN(ledc_set_duty(mode, channel, duty_cycle), false, "LEDC set duty failed");
        CHECK_NOT_ESP_OK_RETURN(ledc_update_duty(mode, channel), false, "LEDC update duty failed");
    } else {
        bool level = (percent > 0) ? _flags.light_up_level : !_flags.light_up_level;

        CHECK_NOT_ESP_OK_RETURN(gpio_set_level((gpio_num_t)_io_num, level), false, "GPIO set level failed");
    }

    return true;
}

bool Backlight::on(void)
{
    return setBrightness(100);
}

bool Backlight::off(void)
{
    return setBrightness(0);
}

bool Backlight::del(void)
{
    CHECK_FALSE_RETURN(checkBegun(), false, "Not begun");

    if (_flags.use_pwm) {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 3, 0)
        CHECK_NOT_ESP_OK_RETURN(
            ledc_stop(_pwm.channel_config.speed_mode, _pwm.channel_config.channel, !_flags.light_up_level), false,
            "LEDC stop failed"
        );
#else
        CHECK_NOT_ESP_OK_RETURN(
            ledc_timer_pause(_pwm.timer_config.speed_mode, _pwm.timer_config.timer_num), false,
            "LEDC stop failed"
        );
        _pwm.timer_config.deconfigure = true;
        CHECK_NOT_ESP_OK_RETURN(
            ledc_timer_config(&_pwm.timer_config), false, "LEDC timer delete failed"
            "LEDC timer delete failed"
        );
#endif // ESP_IDF_VERSION
    } else {
        gpio_reset_pin((gpio_num_t)_io_num);
    }

    _flags = {};
    LOGD("Delete device");

    return true;
}

} // namespace esp_panel
