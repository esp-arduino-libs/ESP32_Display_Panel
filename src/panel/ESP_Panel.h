/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "ESP_Panel_Conf_Internal.h"
#include "ESP_Panel_Board_Internal.h"
#include "host/ESP_PanelHost.h"
#include "bus/ESP_PanelBus.h"
#include "lcd/ESP_PanelLcd.h"
#include "touch/ESP_PanelTouch.h"
#include "backlight/ESP_PanelBacklight.h"
#include "ESP_IOExpander_Library.h"

#ifdef ESP_PANEL_USE_BOARD
#include <memory>

/**
 * @brief Panel device class
 *
 * @note This class is mainly aimed at development boards, which integrates other independent drivers such as LCD, Touch, and Backlight.
 * @note For supported development boards, the drivers internally automatically use preset parameters to configure each independent driver.
 * @note For custom development boards, users need to adjust the parameters according to the actual hardware through the `ESP_Panel_Board_Custom.h` file.
 */
class ESP_Panel {
public:
    /**
     * @brief Construct a new panel device
     *
     */
    ESP_Panel();

    /**
     * @brief Destroy the panel device
     *
     */
    ~ESP_Panel();

    /**
     * @brief Configure the IO expander from the outside. This function should be called before `init()`
     *
     * @param expander Pointer of IO expander
     *
     */
    void configExpander(ESP_IOExpander *expander);

    /**
     * @brief Initialize the panel device, the `begin()` function should be called after this function
     *
     * @note This function typically creates objects for the LCD, Touch, Backlight, and other devices based on the configuration. It then initializes the bus host used by each device.
     *
     * @return true if success, otherwise false
     */
    bool init(void);

    /**
     * @brief Startup the panel device. This function should be called after `init()`
     *
     * @note This function typically initializes the LCD, Touch, Backlight, and other devices based on the configuration.
     *
     * @return true
     */
    bool begin(void);

    /**
     * @brief Delete the panel device, release the resources. This function should be called after `init()`
     *
     * @return true if success, otherwise false
     */
    bool del(void);

    /**
     * @brief Here are the functions to get the pointer of each device
     *
     */
    ESP_PanelLcd *getLcd(void);
    ESP_PanelTouch *getTouch(void);
    ESP_PanelBacklight *getBacklight(void);
    ESP_IOExpander *getExpander(void);

    /**
     * @brief Here are the functions to get the some parameters of the devices
     *
     */
    uint16_t getLcdWidth(void)
    {
#ifdef ESP_PANEL_LCD_WIDTH
        return ESP_PANEL_LCD_WIDTH;
#else
        return 0;
#endif
    }
    uint16_t getLcdHeight(void)
    {
#ifdef ESP_PANEL_LCD_HEIGHT
        return ESP_PANEL_LCD_HEIGHT;
#else
        return 0;
#endif
    }

private:
    bool _is_initialed;
    bool _use_external_expander;
    std::shared_ptr<ESP_PanelBus> _lcd_bus_ptr;
    std::shared_ptr<ESP_PanelBus> _touch_bus_ptr;
    std::shared_ptr<ESP_PanelLcd> _lcd_ptr;
    std::shared_ptr<ESP_PanelTouch> _touch_ptr;
    std::shared_ptr<ESP_PanelBacklight> _backlight_ptr;
    std::shared_ptr<ESP_PanelHost> _host_ptr;
    std::shared_ptr<ESP_IOExpander> _expander_ptr;
};

#endif /* ESP_PANEL_USE_BOARD */
