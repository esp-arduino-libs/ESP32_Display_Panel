/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "driver/gpio.h"

#include "private/CheckResult.h"
#include "ESP_PanelLcdTouch.h"

static const char *TAG = "LcdTouch";

#define LCD_TOUCH_CONFIG_DEFAULT(width, height) \
    {                                           \
        .x_max = width,                         \
        .y_max = height,                        \
        .rst_gpio_num = GPIO_NUM_NC,            \
        .int_gpio_num = GPIO_NUM_NC,            \
        .flags = {                              \
            .swap_xy = 0,                       \
            .mirror_x = 0,                      \
            .mirror_y = 0,                      \
        },                                      \
    }

TouchPoint::TouchPoint(void):
    x(0),
    y(0),
    z(0)
{
}

TouchPoint::TouchPoint(uint16_t x, uint16_t y, uint16_t z):
    x(x),
    y(y),
    z(z)
{
}

bool TouchPoint::operator==(TouchPoint p)
{
    return ((p.x == x) && (p.y == y) && (p.z == z));
}

bool TouchPoint::operator!=(TouchPoint p)
{
    return ((p.x != x) || (p.y != y) || (p.z != z));
}

ESP_PanelLcdTouch::ESP_PanelLcdTouch(ESP_PanelBus *bus, const esp_lcd_touch_config_t *config):
    bus(bus),
    config(*config),
    handle(NULL),
    touch_state(false)
{
}

ESP_PanelLcdTouch::ESP_PanelLcdTouch(ESP_PanelBus *bus, uint16_t width, uint16_t height):
    bus(bus),
    config((esp_lcd_touch_config_t)LCD_TOUCH_CONFIG_DEFAULT(width, height)),
    handle(NULL),
    touch_state(false)
{
}

void ESP_PanelLcdTouch::del(void)
{
    CHECK_ERROR_RETURN(esp_lcd_touch_del(handle));
    handle = NULL;
}

void ESP_PanelLcdTouch::readData(void)
{
    CHECK_ERROR_RETURN(esp_lcd_touch_read_data(handle));
    touch_state = esp_lcd_touch_get_coordinates(handle, x, y, z, &num_points, LCD_TOUCH_MAX_POINTS);
}

bool ESP_PanelLcdTouch::getTouchState(void)
{
    bool ret = false;
    CHECK_NULL_GOTO(handle, err);

    ret = touch_state;
    touch_state = false;
    return ret;

err:
    return false;
}

TouchPoint ESP_PanelLcdTouch::getPoint(uint8_t n)
{
    TouchPoint point(0, 0, 0);
    CHECK_FALSE_GOTO(n < num_points, err);

    point.x = x[n];
    point.y = y[n];
    point.z = z[n];

err:
    return point;
}

bool ESP_PanelLcdTouch::getButtonState(uint8_t n)
{
    uint8_t button_state[LCD_TOUCH_MAX_BUTTONS] = {0};
    CHECK_ERROR_GOTO(esp_lcd_touch_get_button_state(handle, n, &button_state[n]), err);
    return button_state[n];

err:
    return false;
}

void ESP_PanelLcdTouch::swapAxes(bool en)
{
    CHECK_ERROR_RETURN(esp_lcd_touch_set_swap_xy(handle, en));
}

void ESP_PanelLcdTouch::mirrorX(bool en)
{
    CHECK_ERROR_RETURN(esp_lcd_touch_set_mirror_x(handle, en));
}

void ESP_PanelLcdTouch::mirrorY(bool en)
{
    CHECK_ERROR_RETURN(esp_lcd_touch_set_mirror_y(handle, en));
}

esp_lcd_touch_handle_t ESP_PanelLcdTouch::getHandle(void)
{
    CHECK_NULL_GOTO(handle, err);
    return handle;

err:
    return NULL;
}

ESP_PanelBus *ESP_PanelLcdTouch::getBus(void)
{
    CHECK_NULL_GOTO(bus, err);
    return bus;

err:
    return NULL;
}
