/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

/* Handle special Kconfig options */
#ifndef ESP_PANEL_KCONFIG_IGNORE
    #include "sdkconfig.h"
    #ifdef CONFIG_ESP_PANEL_CONF_SKIP
        #define ESP_PANEL_CONF_SKIP
    #endif
#endif

/* If "ESP_Panel_Conf.h" is available from here, try to use it later */
#ifdef __has_include
    #if __has_include("ESP_Panel_Conf.h")
        #ifndef ESP_PANEL_CONF_INCLUDE_SIMPLE
            #define ESP_PANEL_CONF_INCLUDE_SIMPLE
        #endif
    #elif __has_include("../../ESP_Panel_Conf.h")
        #ifndef ESP_PANEL_CONF_INCLUDE_OUTSIDE
            #define ESP_PANEL_CONF_INCLUDE_OUTSIDE
        #endif
    #else
        #define ESP_PANEL_CONF_INCLUDE_INSIDE
    #endif
#endif

/* If "ESP_Panel_Conf.h" is not skipped, include it */
#ifndef ESP_PANEL_CONF_SKIP
    #ifdef ESP_PANEL_CONF_PATH                          /* If there is a path defined for "ESP_Panel_Conf.h" use it */
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_CONF_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_CONF_INCLUDE_SIMPLE)        /* Or simply include if "ESP_Panel_Conf.h" is available */
        #include "ESP_Panel_Conf.h"
    #elif defined(ESP_PANEL_CONF_INCLUDE_OUTSIDE)       /* Or include if "../../ESP_Panel_Conf.h" is available */
        #include "../../ESP_Panel_Conf.h"
    #elif defined(ESP_PANEL_CONF_INCLUDE_INSIDE)        /* Or include the default configuration */
        #include "../ESP_Panel_Conf.h"
    #endif
#endif

#ifndef ESP_PANEL_CONF_INCLUDE_INSIDE
/* Supplement macro definitions based on sdkconfig, use default values if not defined */
/*-------------------------------- Debug configurations --------------------------------*/
#ifndef ESP_PANEL_CHECK_RESULT_ASSERT
    #ifdef CONFIG_ESP_PANEL_CHECK_RESULT_ASSERT
        #define ESP_PANEL_CHECK_RESULT_ASSERT CONFIG_ESP_PANEL_CHECK_RESULT_ASSERT
    #else
        #define ESP_PANEL_CHECK_RESULT_ASSERT   (0)
    #endif
#endif
#ifndef ESP_PANEL_ENABLE_LOG
    #ifdef CONFIG_ESP_PANEL_ENABLE_LOG
        #define ESP_PANEL_ENABLE_LOG CONFIG_ESP_PANEL_ENABLE_LOG
    #else
        #define ESP_PANEL_ENABLE_LOG            (0)
    #endif
#endif
/*----------------------------- Touch driver configurations -----------------------------*/
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
#endif

// *INDENT-OFF*
