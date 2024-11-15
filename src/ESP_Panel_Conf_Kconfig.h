/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Error Check Configurations //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ESP_PANEL_CHECK_RESULT_ASSERT
    #ifdef CONFIG_ESP_PANEL_CHECK_RESULT_ASSERT
        #define ESP_PANEL_CHECK_RESULT_ASSERT   CONFIG_ESP_PANEL_CHECK_RESULT_ASSERT
    #else
        #define ESP_PANEL_CHECK_RESULT_ASSERT   (0)
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// LOG Configurations //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ESP_PANEL_ENABLE_LOG
    #ifdef CONFIG_ESP_PANEL_ENABLE_LOG
        #define ESP_PANEL_ENABLE_LOG            CONFIG_ESP_PANEL_ENABLE_LOG
    #else
        #define ESP_PANEL_ENABLE_LOG            (0)
    #endif
#endif

#ifndef ESP_PANEL_LOG_BUFFER_SIZE
    #ifdef CONFIG_ESP_PANEL_LOG_BUFFER_SIZE
        #define ESP_PANEL_LOG_BUFFER_SIZE       CONFIG_ESP_PANEL_LOG_BUFFER_SIZE
    #else
        #define ESP_PANEL_LOG_BUFFER_SIZE       (256)
    #endif
#endif

#ifndef ESP_PANEL_LOG_GLOBAL_LEVEL
    #ifdef CONFIG_ESP_PANEL_LOG_GLOBAL_LEVEL
        #define ESP_PANEL_LOG_GLOBAL_LEVEL      CONFIG_ESP_PANEL_LOG_GLOBAL_LEVEL
    #else
        #define ESP_PANEL_LOG_GLOBAL_LEVEL      ESP_PANEL_LOG_LEVEL_INFO
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// Memory Configurations /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ESP_PANEL_MEM_GENERAL_ALLOC_TYPE
    #ifdef CONFIG_ESP_PANEL_MEM_GENERAL_ALLOC_TYPE
        #define ESP_PANEL_MEM_GENERAL_ALLOC_TYPE        CONFIG_ESP_PANEL_MEM_GENERAL_ALLOC_TYPE
    #else
        #define ESP_PANEL_MEM_GENERAL_ALLOC_TYPE        (ESP_PANEL_MEM_GENERAL_ALLOC_TYPE_STDLIB)
    #endif
#endif


#if ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_ESP
    #ifndef ESP_PANEL_MEM_GENERAL_ALLOC_ESP_CAPS
        #ifdef CONFIG_ESP_PANEL_MEM_GENERAL_ALLOC_ESP_CAPS
            #define ESP_PANEL_MEM_GENERAL_ALLOC_ESP_CAPS    CONFIG_ESP_PANEL_MEM_GENERAL_ALLOC_ESP_CAPS
        #else
            #error "`ESP_PANEL_MEM_GENERAL_ALLOC_ESP_CAPS` must be defined when `ESP_PANEL_MEM_GENERAL_ALLOC_TYPE` is set to `ESP_PANEL_MEM_ALLOC_TYPE_ESP`"
        #endif
    #endif
#elif ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_CUSTOM
    #ifndef ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_INCLUDE
        #ifdef CONFIG_ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_INCLUDE
            #define ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_INCLUDE  CONFIG_ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_INCLUDE
        #else
            #define ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_INCLUDE "stdlib.h"
        #endif
    #endif

    #ifndef ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_MALLOC
        #error "`ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_MALLOC` must be defined when `ESP_PANEL_MEM_GENERAL_ALLOC_TYPE` is set to `ESP_PANEL_MEM_GENERAL_ALLOC_TYPE_CUSTOM`"
    #endif

    #ifndef ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_FREE
        #error "`ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_FREE` must be defined when `ESP_PANEL_MEM_GENERAL_ALLOC_TYPE` is set to `ESP_PANEL_MEM_GENERAL_ALLOC_TYPE_CUSTOM`"
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Touch Driver Configurations //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ESP_PANEL_TOUCH_MAX_POINTS
#ifdef CONFIG_ESP_PANEL_TOUCH_MAX_POINTS
#define ESP_PANEL_TOUCH_MAX_POINTS CONFIG_ESP_PANEL_TOUCH_MAX_POINTS
#else
#define ESP_PANEL_TOUCH_MAX_POINTS      (5)
#endif
#endif
#ifndef ESP_PANEL_TOUCH_MAX_BUTTONS
#ifdef CONFIG_ESP_PANEL_TOUCH_MAX_BUTTONS
#define ESP_PANEL_TOUCH_MAX_BUTTONS CONFIG_ESP_PANEL_TOUCH_MAX_BUTTONS
#else
#define ESP_PANEL_TOUCH_MAX_BUTTONS     (1)
#endif
#endif
#ifndef ESP_PANEL_TOUCH_XPT2046_Z_THRESHOLD
#ifdef CONFIG_ESP_PANEL_TOUCH_XPT2046_Z_THRESHOLD
#define ESP_PANEL_TOUCH_XPT2046_Z_THRESHOLD CONFIG_ESP_PANEL_TOUCH_XPT2046_Z_THRESHOLD
#else
#define ESP_PANEL_TOUCH_XPT2046_Z_THRESHOLD     (400)
#endif
#endif
#ifndef ESP_PANEL_TOUCH_XPT2046_INTERRUPT_MODE
#ifdef CONFIG_ESP_PANEL_TOUCH_XPT2046_INTERRUPT_MODE
#define ESP_PANEL_TOUCH_XPT2046_INTERRUPT_MODE CONFIG_ESP_PANEL_TOUCH_XPT2046_INTERRUPT_MODE
#else
#define ESP_PANEL_TOUCH_XPT2046_INTERRUPT_MODE  (0)
#endif
#endif
#ifndef ESP_PANEL_TOUCH_XPT2046_VREF_ON_MODE
#ifdef CONFIG_ESP_PANEL_TOUCH_XPT2046_VREF_ON_MODE
#define ESP_PANEL_TOUCH_XPT2046_VREF_ON_MODE CONFIG_ESP_PANEL_TOUCH_XPT2046_VREF_ON_MODE
#else
#define ESP_PANEL_TOUCH_XPT2046_VREF_ON_MODE    (0)
#endif
#endif
#ifndef ESP_PANEL_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS
#ifdef CONFIG_ESP_PANEL_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS
#define ESP_PANEL_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS CONFIG_ESP_PANEL_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS
#else
#define ESP_PANEL_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS   (1)
#endif
#endif
#ifndef ESP_PANEL_TOUCH_XPT2046_ENABLE_LOCKING
#ifdef CONFIG_ESP_PANEL_TOUCH_XPT2046_ENABLE_LOCKING
#define ESP_PANEL_TOUCH_XPT2046_ENABLE_LOCKING CONFIG_ESP_PANEL_TOUCH_XPT2046_ENABLE_LOCKING
#else
#define ESP_PANEL_TOUCH_XPT2046_ENABLE_LOCKING  (1)
#endif
#endif

// *INDENT-OFF*
