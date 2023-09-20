/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELLCD_H
#define ESP_PANELLCD_H

#include <stdint.h>

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"

#include "ESP_PanelBus.h"

typedef struct {
    esp_lcd_panel_dev_config_t dev_config;
} ESP_PanelLcdConfig_t;

class ESP_PanelLcd {
public:
    ESP_PanelLcd(ESP_PanelBus *bus, const ESP_PanelLcdConfig_t *config);
    ESP_PanelLcd(ESP_PanelBus *bus);

    void begin(void);
    void reset(void);
    void del(void);
    void drawBitmap(int x_start, int y_start, int x_end, int y_end, const void *color_data);
    void mirror(bool mirror_x, bool mirror_y);
    void swapAxes(bool en);
    void setGap(int x_gap, int y_gap);
    void invertColor(bool en);
    void displayOn(void);
    void displayOff(void);
    esp_lcd_panel_handle_t getHandle(void);
    ESP_PanelBus *getBus(void);
    void setCallback(ESP_PanelBusCallback_t cbs, void *user_ctx);

    virtual void init(void) = 0;
    virtual ~ESP_PanelLcd() = default;

protected:
    ESP_PanelBus *bus;
    ESP_PanelLcdConfig_t config;
    esp_lcd_panel_handle_t handle;
};

#endif
