/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ESP_PanelLog.h"
#include "driver/gpio.h"
#include "ESP_PanelTouch.h"

static const char *TAG = "ESP_PanelTouch";

#define CALLBACK_DATA_DEFAULT()             \
    {                                       \
        .tp_ptr = this,                     \
        .user_data = NULL,                  \
    }

ESP_PanelTouchPoint::ESP_PanelTouchPoint(void):
    x(0),
    y(0),
    strength(0)
{
}

ESP_PanelTouchPoint::ESP_PanelTouchPoint(uint16_t x, uint16_t y, uint16_t strength):
    x(x),
    y(y),
    strength(strength)
{
}

bool ESP_PanelTouchPoint::operator==(ESP_PanelTouchPoint p)
{
    return ((p.x == x) && (p.y == y));
}

bool ESP_PanelTouchPoint::operator!=(ESP_PanelTouchPoint p)
{
    return ((p.x != x) || (p.y != y));
}

ESP_PanelTouch::ESP_PanelTouch(ESP_PanelBus *bus, uint16_t width, uint16_t height, int rst_io, int int_io):
    bus(bus),
    config((esp_lcd_touch_config_t)ESP_PANEL_TOUCH_CONFIG_DEFAULT(width, height, rst_io, int_io)),
    handle(NULL),
    _swap_xy(false),
    _mirror_x(false),
    _mirror_y(false),
    _tp_points_num(0),
    _tp_buttons_state{0},
    onTouchInterruptCallback(NULL),
    _isr_sem(NULL),
    callback_data(CALLBACK_DATA_DEFAULT())
{
    if (int_io >= 0) {
        config.interrupt_callback = onTouchInterrupt;
        config.user_data = &callback_data;
    }
}

ESP_PanelTouch::ESP_PanelTouch(ESP_PanelBus *bus, const esp_lcd_touch_config_t &config):
    bus(bus),
    config(config),
    handle(NULL),
    _swap_xy(false),
    _mirror_x(false),
    _mirror_y(false),
    _tp_points_num(0),
    _tp_buttons_state{0},
    onTouchInterruptCallback(NULL),
    _isr_sem(NULL),
    callback_data(CALLBACK_DATA_DEFAULT())
{
    if ((config.int_gpio_num != GPIO_NUM_NC) && (config.interrupt_callback == NULL) && (config.user_data == NULL)) {
        this->config.interrupt_callback = onTouchInterrupt;
        this->config.user_data = &callback_data;
    }
}

void ESP_PanelTouch::configLevels(int reset_level, int interrupt_level)
{
    config.levels.reset = reset_level;
    config.levels.interrupt = interrupt_level;
}

bool ESP_PanelTouch::attachInterruptCallback(std::function<bool (void *)> callback, void *user_data)
{
    ESP_PANEL_CHECK_FALSE_RET((config.interrupt_callback == onTouchInterrupt), false, "Interruption is not enabled");

    onTouchInterruptCallback = callback;
    callback_data.user_data = user_data;

    return true;
}

bool ESP_PanelTouch::init(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (config.interrupt_callback == onTouchInterrupt) {
        _isr_sem = xSemaphoreCreateBinary();
        ESP_PANEL_CHECK_NULL_RET(_isr_sem, false, "Create semaphore failed");

        ESP_LOGD(TAG, "Enable interruption");
    } else {
        ESP_LOGD(TAG, "Disable interruption");
    }

    return true;
}

bool ESP_PanelTouch::del(void)
{
    ESP_PANEL_CHECK_NULL_RET(handle, false, "Invalid handle");
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_touch_del(handle), false, "Delete touch panel failed");

    if (_isr_sem != NULL) {
        vSemaphoreDelete(_isr_sem);
        _isr_sem = NULL;
    }

    ESP_LOGD(TAG, "Touch panel @%p deleted", handle);
    handle = NULL;

    return true;
}

bool ESP_PanelTouch::swapXY(bool en)
{
    ESP_PANEL_CHECK_NULL_RET(handle, false, "Invalid handle");
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_touch_set_swap_xy(handle, en), false, "Swap axes failed");
    _swap_xy = en;

    return true;
}

bool ESP_PanelTouch::mirrorX(bool en)
{
    ESP_PANEL_CHECK_NULL_RET(handle, false, "Invalid handle");
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_touch_set_mirror_x(handle, en), false, "Mirror X failed");
    _mirror_x = en;

    return true;
}

bool ESP_PanelTouch::mirrorY(bool en)
{
    ESP_PANEL_CHECK_NULL_RET(handle, false, "Invalid handle");
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_touch_set_mirror_y(handle, en), false, "Mirror Y failed");
    _mirror_y = en;

    return true;
}

bool ESP_PanelTouch::readRawData(uint8_t max_points_num, int timeout_ms)
{
    ESP_PANEL_CHECK_NULL_RET(handle, false, "Invalid handle");

    uint16_t x[CONFIG_ESP_LCD_TOUCH_MAX_POINTS] = {0};
    uint16_t y[CONFIG_ESP_LCD_TOUCH_MAX_POINTS] = {0};
    uint16_t strength[CONFIG_ESP_LCD_TOUCH_MAX_POINTS] = {0};
    _tp_points_num = 0;
    if (max_points_num > CONFIG_ESP_LCD_TOUCH_MAX_POINTS) {
        max_points_num = CONFIG_ESP_LCD_TOUCH_MAX_POINTS;
        ESP_LOGE(TAG, "The max points number out of range [%d/%d]", max_points_num, CONFIG_ESP_LCD_TOUCH_MAX_POINTS);
    }

    if (_isr_sem != NULL) {
        BaseType_t timeout_ticks = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
        if (xSemaphoreTake(_isr_sem, timeout_ticks) != pdTRUE) {
            ESP_LOGD(TAG, "Touch panel @%p wait for isr timeout", handle);
            return true;
        }
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_touch_read_data(handle), false, "Read data failed");
    esp_lcd_touch_get_coordinates(handle, x, y, strength, &_tp_points_num, max_points_num);

    for (int i = 0; i < _tp_points_num; i++) {
        _tp_points[i].x = x[i];
        _tp_points[i].y = y[i];
        _tp_points[i].strength = strength[i];
        ESP_LOGD(TAG, "Touch panel @%p touched (%d, %d, %d)", handle, _tp_points[i].x, _tp_points[i].y,
                 _tp_points[i].strength);
    }

    return true;
}

int ESP_PanelTouch::getPoints(ESP_PanelTouchPoint points[], uint8_t num)
{
    ESP_PANEL_CHECK_FALSE_RET((num == 0) || (points != NULL), -1, "Invalid points or num");

    int i = 0;
    for (; (i < num) && (i < _tp_points_num); i++) {
        points[i] = _tp_points[i];
    }

    return i;
}

int ESP_PanelTouch::getButtonState(uint8_t n)
{
    uint8_t button_state = 0;
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_touch_get_button_state(handle, n, &button_state), -1, "Get button state failed");

    return button_state;
}

int ESP_PanelTouch::readPoints(ESP_PanelTouchPoint points[], uint8_t num, int timeout_ms)
{
    ESP_PANEL_CHECK_FALSE_RET(readRawData(num, timeout_ms), -1, "Read raw data failed");

    return getPoints(points, num);
}

int ESP_PanelTouch::readButtonState(uint8_t n, int timeout_ms)
{
    ESP_PANEL_CHECK_FALSE_RET(readRawData(0, timeout_ms), -1, "Read raw data failed");

    return getButtonState(n);
}

void ESP_PanelTouch::configResetActiveLevel(uint8_t level)
{
    config.levels.reset = level;
}

void ESP_PanelTouch::configInterruptActiveLevel(uint8_t level)
{
    config.levels.interrupt = level;
}

bool ESP_PanelTouch::isInterruptEnabled(void)
{
    return (config.interrupt_callback == onTouchInterrupt);
}

bool ESP_PanelTouch::getSwapXYFlag(void)
{
    return _swap_xy;
}

bool ESP_PanelTouch::getMirrorXFlag(void)
{
    return _mirror_x;
}

bool ESP_PanelTouch::getMirrorYFlag(void)
{
    return _mirror_y;
}

esp_lcd_touch_handle_t ESP_PanelTouch::getHandle(void)
{
    if (handle == NULL) {
        ESP_PANEL_ENABLE_TAG_DEBUG_LOG();
        ESP_LOGD(TAG, "Get invalid handle");
    }

    return handle;
}

ESP_PanelBus *ESP_PanelTouch::getBus(void)
{
    if (bus == NULL) {
        ESP_PANEL_ENABLE_TAG_DEBUG_LOG();
        ESP_LOGD(TAG, "Get invalid bus");
    }

    return bus;
}

void ESP_PanelTouch::onTouchInterrupt(esp_lcd_touch_handle_t tp)
{
    if ((tp == NULL) || (tp->config.user_data == NULL)) {
        return;
    }

    ESP_PanelTouch *panel = (ESP_PanelTouch *)((ESP_PanelTouchCallbackData_t *)tp->config.user_data)->tp_ptr;
    if (panel == NULL) {
        return;
    }

    BaseType_t need_yield = pdFALSE;
    if (panel->onTouchInterruptCallback != NULL) {
        need_yield = panel->onTouchInterruptCallback(panel->callback_data.user_data) ? pdTRUE : need_yield;
    }
    if (panel->_isr_sem != NULL) {
        xSemaphoreGiveFromISR(panel->_isr_sem, &need_yield);
    }
    if (need_yield == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}
