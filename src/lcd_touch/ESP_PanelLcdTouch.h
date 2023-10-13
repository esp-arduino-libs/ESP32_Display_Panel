/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELLCDTOUCH_H
#define ESP_PANELLCDTOUCH_H

#include <stdint.h>

#include "lcd_touch/base/esp_lcd_touch.h"
#include "../bus/ESP_PanelBus.h"

class TouchPoint {
public:
    TouchPoint(void);
    TouchPoint(uint16_t x, uint16_t y, uint16_t z);

    bool operator==(TouchPoint p);
    bool operator!=(TouchPoint p);

    uint16_t x;
    uint16_t y;
    uint16_t z;
};

class ESP_PanelLcdTouch {
public:
    ESP_PanelLcdTouch(ESP_PanelBus *bus, const esp_lcd_touch_config_t *config);
    ESP_PanelLcdTouch(ESP_PanelBus *bus, uint16_t width, uint16_t height);
    virtual ~ESP_PanelLcdTouch() = default;

    void del(void);
    void readData(void);
    bool getTouchState(void);
    TouchPoint getPoint(uint8_t n = 0);
    bool getButtonState(uint8_t n = 0);
    void swapAxes(bool en);
    void mirrorX(bool en);
    void mirrorY(bool en);
    esp_lcd_touch_handle_t getHandle(void);
    ESP_PanelBus *getBus(void);

    virtual void begin(void) = 0;

protected:
    ESP_PanelBus *bus;
    esp_lcd_touch_config_t config;
    esp_lcd_touch_handle_t handle;

private:
    uint8_t touch_state;
    uint8_t num_points;
    uint16_t x[LCD_TOUCH_MAX_POINTS];
    uint16_t y[LCD_TOUCH_MAX_POINTS];
    uint16_t z[LCD_TOUCH_MAX_POINTS];
};

#endif
