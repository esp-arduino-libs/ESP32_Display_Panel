/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_lcd_panel_vendor.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t esp_lcd_new_panel_gc9d01(esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *config,
                                   esp_lcd_panel_handle_t *panel);

#ifdef __cplusplus
}
#endif
