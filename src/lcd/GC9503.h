/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "ESP_PanelLcd.h"
#include "base/esp_lcd_custom_types.h"
#include "base/esp_lcd_gc9503.h"

/**
 * @brief GC9503 LCD device object class
 *
 * @note  This class is a derived class of `ESP_PanelLcd`, user can use it directly
 */
class ESP_PanelLcd_GC9503: public ESP_PanelLcd {
public:
    /**
     * @brief Construct a new LCD device in a simple way, the `init()` function should be called after this function
     *
     * @note  This function uses some default values to config the LCD device, please use `config*()` functions to
     *        change them
     * @note  Vendor specific initialization can be different between manufacturers, should consult the LCD supplier
     *        for initialization sequence code, and use `configVendorCommands()` to configure
     *
     * @param bus           Pointer of panel bus
     * @param color_bits    Bits per pixel (16/18/24)
     * @param rst_io        Reset pin, set to -1 if no use
     */
    ESP_PanelLcd_GC9503(ESP_PanelBus *bus, uint8_t color_bits, int rst_io = -1);

    /**
     * @brief Construct a new LCD in a complex way, the `init()` function should be called after this function
     *
     * @param bus           Pointer of panel bus
     * @param panel_config  LCD device configuration
     */
    ESP_PanelLcd_GC9503(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config);

    /**
     * @brief Destroy the LCD device
     *
     */
    ~ESP_PanelLcd_GC9503() override;

    /**
     * @brief Initialize the LCD device, the `begin()` function should be called after this function
     *
     * @note  This function typically calls `esp_lcd_new_panel_*()` to create the LCD panel handle
     *
     * @return true if success, otherwise false
     */
    bool init(void) override;
};
#endif
