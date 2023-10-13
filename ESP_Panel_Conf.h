/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/**
 * Debug configurations
 *
 */
/* Set to 1 if assert on error. Otherwise print error message */
#define ESP_PANEL_CHECK_RESULT_ASSERT       (0)         // 0/1

/* Set to 1 if print log message for debug */
#define ESP_PANEL_ENABLE_LOG                (0)         // 0/1

/**
 * Touch driver configurations
 *
 */
/* Maximum point number */
#define ESP_PANEL_TOUCH_MAX_POINTS          (5)

/* Maximum button number */
#define ESP_PANEL_TOUCH_MAX_BUTTONS         (1)
