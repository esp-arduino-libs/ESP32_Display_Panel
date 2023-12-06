/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "ESP_IOExpander_Library.h"
#include "ESP_PanelBacklight.h"
#include "ESP_PanelBus.h"
#include "ESP_PanelLcd.h"
#include "ESP_PanelLcdTouch.h"
#include "ESP_Panel_Conf_Internal.h"

#ifndef ESP_PANEL_CONF_IGNORE
class ESP_Panel {
public:
    ESP_Panel();
    ~ESP_Panel();

    void init(void);
    void begin(void);
    void del(void);

#if ESP_PANEL_USE_LCD
    ESP_PanelLcd *getLcd(void);
#endif

#if ESP_PANEL_USE_LCD_TOUCH
    ESP_PanelLcdTouch *getLcdTouch(void);
#endif

#if ESP_PANEL_USE_BL
    ESP_PanelBacklight *getBacklight(void);
#endif

#if ESP_PANEL_USE_EXPANDER
    bool addIOExpander(ESP_IOExpander *expander);
    ESP_IOExpander *getExpander(void);
#endif

private:
    void runExtraBoardInit(void);

#if ESP_PANEL_USE_LCD
    ESP_PanelLcd *lcd;
#endif

#if ESP_PANEL_USE_LCD_TOUCH
    ESP_PanelLcdTouch *lcd_touch;
#endif

#if ESP_PANEL_USE_BL
    ESP_PanelBacklight *backlight;
#endif

#if ESP_PANEL_USE_EXPANDER
    ESP_IOExpander *expander;
#endif
};

#endif
