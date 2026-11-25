/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_touch_conf_internal.h"
#if ESP_PANEL_DRIVERS_TOUCH_ENABLE_CST9220
#include "esp_panel_touch_cst9220.hpp"
#include "utils/esp_panel_utils_log.h"

namespace esp_panel::drivers {

TouchCST9220::~TouchCST9220()
{
    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete CST9220 failed");
}

bool TouchCST9220::begin()
{
    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::BEGIN), false, "CST9220 already started");
    ESP_UTILS_CHECK_FALSE_RETURN(getBus() && getBus()->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_I2C,
                                 false, "CST9220 requires an I2C bus");
    if (!isOverState(State::INIT)) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Initialize CST9220 bus failed");
    }

    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_touch_new_i2c_cst9220(getBus()->getControlPanelHandle(),
                                 getConfig().getDeviceFullConfig(), &touch_panel), false, "Create CST9220 failed");
    setState(State::BEGIN);
    return true;
}

} // namespace esp_panel::drivers
#endif
