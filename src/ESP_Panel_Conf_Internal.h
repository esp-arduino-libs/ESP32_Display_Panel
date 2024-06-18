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
    #ifdef CONFIG_ESP_PANEL_CONF_FILE_SKIP
        #define ESP_PANEL_CONF_FILE_SKIP
    #endif
#endif

#ifndef ESP_PANEL_CONF_FILE_SKIP
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
#else
#endif

/* If "ESP_Panel_Conf.h" is not skipped, include it */
#ifndef ESP_PANEL_CONF_FILE_SKIP
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
    /**
     * There are two purposes to include the this file:
     *  1. Convert configuration items starting with `CONFIG_` to the required configuration items.
     *  2. Define default values for configuration items that are not defined to keep compatibility.
     *
     */
    #include "ESP_Panel_Conf_Kconfig.h"
#endif

// *INDENT-OFF*
