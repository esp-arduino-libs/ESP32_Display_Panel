/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <functional>
#include "driver/ledc.h"

namespace esp_panel {

/**
 * @brief The backlight device class
 *
 */
class Backlight {
public:
    using SetBrightnessCallback = std::function <bool(uint8_t, void *)>;

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
    Backlight(int io_num, bool light_up_level, bool use_pwm);

    /**
     * @brief Construct a new backlight device with the specified callback function to set the brightness
     *
     * @param callback  The callback function to set the brightness
     * @param user_data The user data passed to the callback function
     */
    Backlight(SetBrightnessCallback callback, void *user_data);

    /**
     * @brief Destroy the backlight device
     *
     */
    ~Backlight();

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
    bool checkBegun(void)
    {
        return _flags.is_begun;
    }

    struct {
        uint8_t is_begun: 1;
        uint8_t use_pwm: 1;
        uint8_t use_callback: 1;
        uint8_t light_up_level: 1;
    } _flags;
    union {
        uint8_t _io_num;
        struct {
            ledc_timer_config_t timer_config;
            ledc_channel_config_t channel_config;
        } _pwm;
        struct {
            SetBrightnessCallback cbx_function;
            void *user_data;
        } _callback;
    };
};

} // namespace esp_panel
