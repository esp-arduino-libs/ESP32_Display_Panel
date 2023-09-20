/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANEL_H
#define ESP_PANEL_H

#include "ESP_IOExpander.h"

#include "bus/all_support_bus.h"
#include "lcd/all_support_lcd.h"
#include "lcd_touch/all_support_lcd_touch.h"
#include "ESP_PanelBus.h"
#include "ESP_PanelLcd.h"
#include "ESP_PanelLcdTouch.h"
#include "ESP_PanelBacklight.h"

class ESP_Panel {
public:
    ESP_Panel();
    ~ESP_Panel();

    void addIOExpander(ESP_IOExpander *expander);
    void init(void);
    void begin(void);
    void del(void);
    ESP_PanelLcd *getLcd(void);
    ESP_PanelLcdTouch *getLcdTouch(void);
    ESP_PanelBacklight *getBacklight(void);
    ESP_IOExpander *getExpander(void);

private:
    ESP_PanelLcd *lcd;
    ESP_PanelLcdTouch *lcd_touch;
    ESP_PanelBacklight *backlight;
    ESP_IOExpander *expander;
};

#endif
