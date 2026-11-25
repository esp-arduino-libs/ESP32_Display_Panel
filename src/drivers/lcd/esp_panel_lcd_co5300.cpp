/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_lcd_conf_internal.h"
#if ESP_PANEL_DRIVERS_LCD_ENABLE_CO5300
#include "esp_panel_lcd_co5300.hpp"
#include "port/esp_lcd_co5300.h"
#include "utils/esp_panel_utils_log.h"

namespace esp_panel::drivers {

LCD_CO5300::~LCD_CO5300()
{
    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete CO5300 failed");
}

bool LCD_CO5300::init()
{
    static const BasicBusSpecification spec = {
        .x_coord_align = 2,
        .y_coord_align = 2,
        .color_bits = (1U << BasicBusSpecification::COLOR_BITS_RGB565_16) |
        (1U << BasicBusSpecification::COLOR_BITS_RGB666_18) |
        (1U << BasicBusSpecification::COLOR_BITS_RGB888_24),
        .functions = (1U << BasicBusSpecification::FUNC_INVERT_COLOR) |
        (1U << BasicBusSpecification::FUNC_SWAP_XY) |
        (1U << BasicBusSpecification::FUNC_MIRROR_X) |
        (1U << BasicBusSpecification::FUNC_MIRROR_Y) |
        (1U << BasicBusSpecification::FUNC_GAP) |
        (1U << BasicBusSpecification::FUNC_DISPLAY_ON_OFF),
    };
    static const BasicBusSpecificationMap bus_specs = {
        {ESP_PANEL_BUS_TYPE_SPI, spec},
        {ESP_PANEL_BUS_TYPE_QSPI, spec},
    };

    ESP_UTILS_CHECK_FALSE_RETURN(processDeviceOnInit(bus_specs), false, "Initialize CO5300 bus and configuration failed");
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_new_panel_co5300(getBus()->getControlPanelHandle(),
                                 getConfig().getDeviceFullConfig(), &refresh_panel), false, "Create CO5300 failed");
    setState(State::INIT);
    return true;
}

bool LCD_CO5300::setBrightness(int percent)
{
    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::BEGIN), false, "CO5300 has not started");
    ESP_UTILS_CHECK_FALSE_RETURN(percent >= 0 && percent <= 100, false, "Invalid brightness");
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_co5300_set_brightness(refresh_panel, percent), false,
                                 "Set CO5300 brightness failed");
    return true;
}

} // namespace esp_panel::drivers
#endif
