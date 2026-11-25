/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "esp_idf_version.h"

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 4, 0)
#error "ESP32_Display_Panel 2.x requires ESP-IDF 5.4 or Arduino-ESP32 3.2.0 or later."
#endif

/* Base */
#include "esp_panel_types.h"
#include "esp_panel_versions.h"
#include "esp_panel_conf_internal.h"

/* Utils */
#include "utils/esp_panel_utils_cxx.hpp"

/* Drivers */
#include "drivers/bus/esp_panel_bus_factory.hpp"
#include "drivers/lcd/esp_panel_lcd_factory.hpp"
#include "drivers/touch/esp_panel_touch_factory.hpp"
#include "drivers/backlight/esp_panel_backlight_factory.hpp"
#include "drivers/io_expander/esp_panel_io_expander_factory.hpp"

/* Board */
#include "board/esp_panel_board.hpp"
