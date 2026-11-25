/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_lcd_panel_vendor.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t esp_lcd_new_panel_co5300(esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *config,
                                   esp_lcd_panel_handle_t *panel);
esp_err_t esp_lcd_panel_co5300_set_brightness(esp_lcd_panel_handle_t panel, uint8_t percent);

#ifdef __cplusplus
}
#endif
