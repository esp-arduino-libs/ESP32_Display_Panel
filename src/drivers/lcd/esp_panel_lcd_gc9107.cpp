/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_lcd_conf_internal.h"
#if ESP_PANEL_DRIVERS_LCD_ENABLE_GC9107
#include "esp_panel_lcd_gc9107.hpp"
#include "port/esp_lcd_gc9107.h"
#include "utils/esp_panel_utils_log.h"

namespace esp_panel::drivers {

LCD_GC9107::~LCD_GC9107()
{
    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete GC9107 failed");
}

bool LCD_GC9107::init()
{
    static const BasicBusSpecificationMap bus_specs = {
        {
            ESP_PANEL_BUS_TYPE_SPI, BasicBusSpecification{
                .color_bits = (1U << BasicBusSpecification::COLOR_BITS_RGB565_16) |
                (1U << BasicBusSpecification::COLOR_BITS_RGB666_18),
                .functions = (1U << BasicBusSpecification::FUNC_INVERT_COLOR) |
                (1U << BasicBusSpecification::FUNC_SWAP_XY) |
                (1U << BasicBusSpecification::FUNC_MIRROR_X) |
                (1U << BasicBusSpecification::FUNC_MIRROR_Y) |
                (1U << BasicBusSpecification::FUNC_GAP) |
                (1U << BasicBusSpecification::FUNC_DISPLAY_ON_OFF),
            }
        },
    };

    ESP_UTILS_CHECK_FALSE_RETURN(processDeviceOnInit(bus_specs), false, "Initialize GC9107 bus and configuration failed");
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_new_panel_gc9107(getBus()->getControlPanelHandle(),
                                 getConfig().getDeviceFullConfig(), &refresh_panel), false, "Create GC9107 failed");
    setState(State::INIT);
    return true;
}

} // namespace esp_panel::drivers
#endif
