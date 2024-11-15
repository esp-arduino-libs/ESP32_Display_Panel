/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// Utils Configurations /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Check result related
 *
 */
/* Set to 1 if assert on error. Otherwise print error message */
#define ESP_PANEL_CHECK_RESULT_ASSERT       (0)         // 0/1

/**
 * LOG related
 *
 */
/* Set to 1 if print log message */
#define ESP_PANEL_ENABLE_LOG                (1)         // 0/1
#if ESP_PANEL_ENABLE_LOG
/* Log format buffer size */
#define ESP_PANEL_LOG_BUFFER_SIZE           (256)
/**
 * Global log level, logs with a level lower than this will not be compiled. Choose one of the following:
 *  - ESP_PANEL_LOG_LEVEL_DEBUG:   Extra information which is not necessary for normal use (values, pointers, sizes, etc)
 *                                 (lowest level)
 *  - ESP_PANEL_LOG_LEVEL_INFO:    Information messages which describe the normal flow of events
 *  - ESP_PANEL_LOG_LEVEL_WARNING: Error conditions from which recovery measures have been taken
 *  - ESP_PANEL_LOG_LEVEL_ERROR:   Critical errors, software module cannot recover on its own (highest level)
 *
 */
#define ESP_PANEL_LOG_GLOBAL_LEVEL          ESP_PANEL_LOG_LEVEL_INFO
#endif // ESP_PANEL_ENABLE_LOG

/**
 * Memory related
 *
 */
/**
 * Memory allocation type, choose one of the following:
 *  - ESP_PANEL_MEM_ALLOC_TYPE_STDLIB:      Use the standard library memory allocation functions (malloc, free)
 *  - ESP_PANEL_MEM_ALLOC_TYPE_ESP:         Use the ESP-IDF memory allocation functions (heap_caps_malloc, heap_caps_free)
 *  - ESP_PANEL_MEM_ALLOC_TYPE_MICROPYTHON: Use the MicroPython memory allocation functions (m_malloc, m_free)
 *  - ESP_PANEL_MEM_ALLOC_TYPE_CUSTOM:      Use custom memory allocation functions (ESP_PANEL_MEM_ALLOC_CUSTOM_MALLOC,
 *                                          ESP_PANEL_MEM_ALLOC_CUSTOM_FREE)
 *
 */
#define ESP_PANEL_MEM_GENERAL_ALLOC_TYPE            ESP_PANEL_MEM_ALLOC_TYPE_STDLIB
#if ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_ESP
#define ESP_PANEL_MEM_GENERAL_ALLOC_ESP_CAPS        (MALLOC_CAP_DEFAULT | MALLOC_CAP_8BIT)
#elif ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_CUSTOM
#define ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_INCLUDE  stdlib.h
#define ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_MALLOC   malloc
#define ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_FREE     free
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Touch Driver Configurations //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Maximum point number */
#define ESP_PANEL_TOUCH_MAX_POINTS          (5)
/* Maximum button number */
#define ESP_PANEL_TOUCH_MAX_BUTTONS         (1)

/**
 * XPT2046 related
 *
 */
#define ESP_PANEL_TOUCH_XPT2046_Z_THRESHOLD             (400)   // Minimum Z pressure threshold
/**
 * Enable Interrupt (PENIRQ) output, also called Full Power Mode.
 * Enable this to configure the XPT2046 to output low on the PENIRQ output if a touch is detected.
 * This mode uses more power when enabled. Note that this signal goes low normally when a read is active.
 */
#define ESP_PANEL_TOUCH_XPT2046_INTERRUPT_MODE          (0)     // 0/1
/**
 * Keep internal Vref enabled.
 * Enable this to keep the internal Vref enabled between conversions. This uses slightly more power,
 * but requires fewer transactions when reading the battery voltage, aux voltage and temperature.
 *
 */
#define ESP_PANEL_TOUCH_XPT2046_VREF_ON_MODE            (0)     // 0/1
/**
 * Convert touch coordinates to screen coordinates.
 * When this option is enabled the raw ADC values will be converted from 0-4096 to 0-{screen width} or 0-{screen height}.
 * When this option is disabled the process_coordinates method will need to be used to convert the raw ADC values into a
 * screen coordinate.
 *
 */
#define ESP_PANEL_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS   (1)     // 0/1
/**
 * Enable data structure locking.
 * By enabling this option the XPT2046 driver will lock the touch position data structures when reading values from the
 * XPT2046 and when reading position data via API.
 * WARNING: enabling this option may result in unintended crashes.
 *
 */
#define ESP_PANEL_TOUCH_XPT2046_ENABLE_LOCKING          (0)     // 0/1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// File Version ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Do not change the following versions, they are used to check if the configurations in this file are compatible with
 * the current version of `ESP_Panel_Conf.h` in the library. The detailed rules are as follows:
 *
 *   1. If the major version is not consistent, then the configurations in this file are incompatible with the library
 *      and must be replaced with the file from the library.
 *   2. If the minor version is not consistent, this file might be missing some new configurations, which will be set to
 *      default values. It is recommended to replace it with the file from the library.
 *   3. Even if the patch version is not consistent, it will not affect normal functionality.
 *
 */
#define ESP_PANEL_CONF_FILE_VERSION_MAJOR 0
#define ESP_PANEL_CONF_FILE_VERSION_MINOR 2
#define ESP_PANEL_CONF_FILE_VERSION_PATCH 0
