/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ESP_PanelLog.h"
#include "ESP_PanelBacklight.h"

static const char *TAG = "ESP_PanelBacklight";

ESP_PanelBacklight::ESP_PanelBacklight(int io_num, bool light_up_level, bool use_pwm):
    _is_initialized(false),
    _use_pwm(use_pwm),
    _light_up_level(light_up_level),
    _io_num(io_num),
    _timer_config(ESP_PANEL_BACKLIGHT_LEDC_TIMER_CONFIG_DEFAULT()),
    _channel_config(ESP_PANEL_BACKLIGHT_LEDC_CHANNEL_CONFIG_DEFAULT(io_num, light_up_level))
{
}

ESP_PanelBacklight::ESP_PanelBacklight(const ledc_timer_config_t &timer_config, const ledc_channel_config_t &channel_config):
    _is_initialized(false),
    _use_pwm(true),
    _timer_config(timer_config),
    _channel_config(channel_config)
{
}

ESP_PanelBacklight::~ESP_PanelBacklight()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (!_is_initialized && !del()) {
        ESP_LOGE(TAG, "Delete device failed");
    }

    ESP_LOGD(TAG, "Destroyed");
}

bool ESP_PanelBacklight::begin(void)
{
    ESP_PANEL_CHECK_FALSE_RET(_io_num >= 0, false, "Invalid IO number");

    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();
    ESP_LOGD(TAG, "begin start");

    if (_use_pwm) {
        ESP_LOGD(TAG, "Use PWM(LEDC) to control");

        ESP_PANEL_CHECK_ERR_RET(ledc_timer_config(&_timer_config), false, "LEDC timer config failed");
        ESP_PANEL_CHECK_ERR_RET(ledc_channel_config(&_channel_config), false, "LEDC channel config failed");
    } else {
        ESP_LOGD(TAG, "Use GPIO to control");

        gpio_config_t io_config = ESP_PANEL_BACKLIGHT_GPIO_CONFIG_DEFAULT(_io_num);
        ESP_PANEL_CHECK_ERR_RET(gpio_config(&io_config), false, "GPIO config failed");
        ESP_PANEL_CHECK_ERR_RET(gpio_set_level((gpio_num_t)_io_num, _light_up_level), false, "GPIO set level failed");
    }

    _is_initialized = true;
    ESP_LOGD(TAG, "begin end");

    return true;
}

bool ESP_PanelBacklight::setBrightness(uint8_t percent)
{
    ESP_PANEL_CHECK_FALSE_RET(_is_initialized, false, "Device has not been initialized");

    ESP_LOGD(TAG, "Set brightness to %d%%", percent);

    percent = percent > 100 ? 100 : percent;
    if (_use_pwm) {
        uint32_t duty_cycle = (BIT(_timer_config.duty_resolution) * percent) / 100;
        ledc_channel_t channel = _channel_config.channel;
        ledc_mode_t mode = _channel_config.speed_mode;

        ESP_PANEL_CHECK_ERR_RET(ledc_set_duty(mode, channel, duty_cycle), false, "LEDC set duty failed");
        ESP_PANEL_CHECK_ERR_RET(ledc_update_duty(mode, channel), false, "LEDC update duty failed");
    } else {
        bool level = (percent > 0) ? _light_up_level : !_light_up_level;

        ESP_PANEL_CHECK_ERR_RET(gpio_set_level((gpio_num_t)_io_num, level), false, "GPIO set level failed");
    }

    return true;
}

bool ESP_PanelBacklight::on(void)
{
    return setBrightness(100);
}

bool ESP_PanelBacklight::off(void)
{
    return setBrightness(0);
}

bool ESP_PanelBacklight::del(void)
{
    ESP_PANEL_CHECK_FALSE_RET(_is_initialized, false, "Device has not been initialized");

    if (_use_pwm) {
        ledc_mode_t mode = _channel_config.speed_mode;
        ledc_channel_t channel = _channel_config.channel;

        ESP_PANEL_CHECK_ERR_RET(ledc_stop(mode, channel, _light_up_level), false, "LEDC stop failed");
    } else {
        gpio_reset_pin((gpio_num_t)_io_num);
    }

    _is_initialized = false;
    ESP_LOGD(TAG, "Delete device");

    return true;
}
