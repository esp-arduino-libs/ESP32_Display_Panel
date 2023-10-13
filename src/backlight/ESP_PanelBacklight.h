/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include "driver/ledc.h"

/**
 * @brief Backlight LEDC default configuration macros
 *
 */
#define ESP_PANEL_BACKLIGHT_LEDC_TIMER_CONFIG_DEFAULT() \
    {                                                   \
        .speed_mode = LEDC_LOW_SPEED_MODE,              \
        .duty_resolution = LEDC_TIMER_10_BIT,           \
        .timer_num = LEDC_TIMER_0,                      \
        .freq_hz = 5000,                                \
        .clk_cfg = LEDC_AUTO_CLK,                       \
    }
#define ESP_PANEL_BACKLIGHT_LEDC_CHANNEL_CONFIG_DEFAULT(io_num, on_level) \
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
#define ESP_PANEL_BACKLIGHT_GPIO_CONFIG_DEFAULT(io_num) \
    {                                                   \
        .pin_bit_mask = BIT64(io_num),                  \
        .mode = GPIO_MODE_OUTPUT,                       \
        .pull_up_en = GPIO_PULLUP_DISABLE,              \
        .pull_down_en = GPIO_PULLDOWN_DISABLE,          \
        .intr_type = GPIO_INTR_DISABLE,                 \
    }

/**
 * @brief The backlight device class
 *
 */
class ESP_PanelBacklight {
public:
    /**
     * @brief Construct a new backlight device in a simple way, the `begin()` function should be called after this
     *
     * @note  This function uses some default values to config the backlight device, please use `config*()` functions to
     *        change them
     *
     * @param io_num         GPIO number
     * @param light_up_level Level when light up
     * @param use_pwm        Use PWM(LEDC) if set to true
     */
    ESP_PanelBacklight(int io_num, bool light_up_level, bool use_pwm);

    /**
     * @brief Construct a new backlight device with using PWM(LEDC) in a complex way, the `begin()` function should be
     *        called after this
     *
     * @param timer_config   LEDC timer configuration
     * @param channel_config LEDC channel configuration
     */
    ESP_PanelBacklight(const ledc_timer_config_t &timer_config, const ledc_channel_config_t &channel_config);

    /**
     * @brief Destroy the backlight device
     *
     */
    ~ESP_PanelBacklight();

    /**
     * @brief Startup the backlight device
     *
     * @return true if success, otherwise false
     */
    bool begin(void);

    /**
     * @brief Set the brightness of the backlight by percent
     *
     * @note  This function should be called after `begin()`
     * @note  When not using PWM, calling this function only controls the backlight switch and cannot adjust
     *        the brightness
     *
     * @param percent The brightness percent, 0-100
     *
     * @return true if success, otherwise false
     */
    bool setBrightness(uint8_t percent);

    /**
     * @brief Turn on the backlight
     *
     * @note  This function should be called after `begin()`
     * @note  This function is same as `setBrightness(100)`
     *
     * @return true if success, otherwise false
     */
    bool on(void);

    /**
     * @brief Turn off the backlight
     *
     * @note  This function should be called after `begin()`
     * @note  This function is same as `setBrightness(0)`
     *
     * @return true if success, otherwise false
     */
    bool off(void);

    /**
     * @brief Delete the backlight device, release the resources
     *
     * @note  This function should be called after `begin()`
     *
     * @return true if success, otherwise false
     */
    bool del(void);

private:
    bool _is_initialized;
    bool _use_pwm;
    bool _light_up_level;
    uint8_t _io_num;
    ledc_timer_config_t _timer_config;
    ledc_channel_config_t _channel_config;
};
