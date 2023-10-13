/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELBACKLIGHT_H
#define ESP_PANELBACKLIGHT_H

#include <stdint.h>

#include "driver/ledc.h"

typedef struct {
    int io_num;
    /* PWM config */
    struct {
        ledc_timer_t timer;
        ledc_channel_t channel;
        ledc_timer_bit_t resolution;
        uint32_t freq_hz;
    } pwm;
    struct {
        uint32_t use_pwm: 1;
        uint32_t light_on_level: 1;
    } flags;
} ESP_PanelBacklightConfig_t;

class ESP_PanelBacklight {
public:
    ESP_PanelBacklight(const ESP_PanelBacklightConfig_t *config);
    ESP_PanelBacklight(int io_num, int light_on_level);
    ~ESP_PanelBacklight();

    void init(void);
    void setBrightness(uint8_t percent);
    void on(void);
    void off(void);
    void del(void);

private:
    ESP_PanelBacklightConfig_t config;
    bool is_initialized;
};

#endif
