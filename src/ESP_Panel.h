/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "ESP_Panel_Library.h"

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
    uint16_t getLcdWidth(void);
    uint16_t getLcdHeight(void);

private:
    bool _is_initialed;
    bool _use_external_expander;
    std::shared_ptr<ESP_PanelBus> _lcd_bus_ptr;
    std::shared_ptr<ESP_PanelBus> _touch_bus_ptr;
    std::shared_ptr<ESP_PanelLcd> _lcd_ptr;
    std::shared_ptr<ESP_PanelTouch> _touch_ptr;
    std::shared_ptr<ESP_PanelBacklight> _backlight_ptr;
    std::shared_ptr<ESP_IOExpander> _expander_ptr;
};

inline uint16_t ESP_Panel::getLcdWidth(void)
{
#ifdef ESP_PANEL_LCD_WIDTH
    return ESP_PANEL_LCD_WIDTH;
#else
    return 0;
#endif
}

inline uint16_t ESP_Panel::getLcdHeight(void)
{
#ifdef ESP_PANEL_LCD_HEIGHT
    return ESP_PANEL_LCD_HEIGHT;
#else
    return 0;
#endif
}

#endif /* ESP_PANEL_USE_BOARD */
