/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_idf_version.h"

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
#define ESP_PANEL_LCD_DPI_COLOR_CONFIG(format) .in_color_format = (format),
#define ESP_PANEL_LCD_DPI_LEGACY_DMA_CONFIG
#else
#define ESP_PANEL_LCD_DPI_COLOR_CONFIG(format) .pixel_format = (format),
#define ESP_PANEL_LCD_DPI_LEGACY_DMA_CONFIG .flags = { .use_dma2d = true },
#endif
