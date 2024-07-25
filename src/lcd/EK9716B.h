/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "ESP_PanelLcd.h"

/**
 * @brief EK9716B LCD device object class
 *
 * @note  This class is a derived class of `ESP_PanelLcd`, user can use it directly
 */
class ESP_PanelLcd_EK9716B: public ESP_PanelLcd {
public:
    /**
     * @brief Construct a new LCD device in a simple way, the `init()` function should be called after this function
     *
     * @note  This function uses some default values to config the LCD device, please use `config*()` functions to
     *        change them
     *
     * @param bus           Pointer of panel bus
     * @param color_bits    Bits per pixel (24)
     * @param rst_io        Reset pin, set to `-1` if no use
     */
    ESP_PanelLcd_EK9716B(ESP_PanelBus *bus, uint8_t color_bits, int rst_io = -1);

    /**
     * @brief Construct a new LCD device in a complex way, the `init()` function should be called after this function
     *
     * @param bus           Pointer of panel bus
     * @param panel_config  LCD device configuration
     */
    ESP_PanelLcd_EK9716B(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config);

    /**
     * @brief Destroy the LCD device
     *
     */
    ~ESP_PanelLcd_EK9716B() override;

    /**
     * @brief Initialize the LCD device, the `begin()` function should be called after this function
     *
     * @note  This function typically calls `esp_lcd_new_panel_*()` to create the LCD panel handle
     *
     * @return true if success, otherwise false
     */
    bool init(void) override;

    /**
     * @brief Reset the LCD. If the `rst_io` is not set, this function will do reset by software instead of hardware
     *
     * @note  This function should be called after `init()`
     *
     * @return true if success, otherwise false
     */
    bool reset(void);
};

#endif /* SOC_LCD_RGB_SUPPORTED */
