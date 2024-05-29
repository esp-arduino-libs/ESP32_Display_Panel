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

#include "ESP_PanelTypes.h"

/* If "ESP_Panel_*_Board.h" are available from here, try to use them later */
#ifdef __has_include
    #if __has_include("ESP_Panel_Board_Supported.h")
        #ifndef ESP_PANEL_SUPPORTED_BOARD_INCLUDE_SIMPLE
            #define ESP_PANEL_SUPPORTED_BOARD_INCLUDE_SIMPLE
        #endif
    #elif __has_include("../../ESP_Panel_Board_Supported.h")
        #ifndef ESP_PANEL_SUPPORTED_BOARD_INCLUDE_OUTSIDE
            #define ESP_PANEL_SUPPORTED_BOARD_INCLUDE_OUTSIDE
        #endif
    #endif
    #if __has_include("ESP_Panel_Board_Custom.h")
        #ifndef ESP_PANEL_CUSTOM_BOARD_INCLUDE_SIMPLE
            #define ESP_PANEL_CUSTOM_BOARD_INCLUDE_SIMPLE
        #endif
    #elif __has_include("../../ESP_Panel_Board_Custom.h")
        #ifndef ESP_PANEL_CUSTOM_BOARD_INCLUDE_OUTSIDE
            #define ESP_PANEL_CUSTOM_BOARD_INCLUDE_OUTSIDE
        #endif
    #endif
#endif

/* If "ESP_Panel_Board_*.h" are not skipped, include them */
#ifndef ESP_PANEL_CONF_SKIP
    #ifdef ESP_PANEL_SUPPORTED_BOARD_PATH                       /* If there is a path defined for "ESP_Panel_Board_Supported.h", use it */
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_SUPPORTED_BOARD_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_SUPPORTED_BOARD_INCLUDE_SIMPLE)     /* Or simply include if "ESP_Panel_Board_Supported.h" is available */
        #include "ESP_Panel_Board_Supported.h"
    #elif defined(ESP_PANEL_SUPPORTED_BOARD_INCLUDE_OUTSIDE)    /* Or include if "../../ESP_Panel_Board_Supported.h" is available */
        #include "../../ESP_Panel_Board_Supported.h"
    #endif
    #ifdef ESP_PANEL_CUSTOM_BOARD_PATH                          /* If there is a path defined for "ESP_Panel_Board_Custom.h" use it */
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_CUSTOM_BOARD_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_CUSTOM_BOARD_INCLUDE_SIMPLE)        /* Or simply include if "ESP_Panel_Board_Custom.h" is available */
        #include "ESP_Panel_Board_Custom.h"
    #elif defined(ESP_PANEL_CUSTOM_BOARD_INCLUDE_OUTSIDE)       /* Or include if "../../ESP_Panel_Board_Custom.h" is available */
        #include "../../ESP_Panel_Board_Custom.h"
    #endif
#endif

/* Check if select both custom and supported board */
#if (CONFIG_ESP_PANEL_USE_SUPPORTED_BOARD || ESP_PANEL_USE_SUPPORTED_BOARD) && \
    (CONFIG_ESP_PANEL_USE_CUSTOM_BOARD || ESP_PANEL_USE_CUSTOM_BOARD)
    #error "Please select either a custom or a supported development board, cannot enable both simultaneously"
#endif

/* Check if use board */
#if CONFIG_ESP_PANEL_USE_SUPPORTED_BOARD || ESP_PANEL_USE_SUPPORTED_BOARD || \
    CONFIG_ESP_PANEL_USE_CUSTOM_BOARD || ESP_PANEL_USE_CUSTOM_BOARD
    #define ESP_PANEL_USE_BOARD
#endif

#ifdef ESP_PANEL_USE_BOARD
/* For using a supported board */
#if CONFIG_ESP_PANEL_USE_SUPPORTED_BOARD || ESP_PANEL_USE_SUPPORTED_BOARD
// Include the supported board header file
#include "board/ESP_PanelBoard.h"
#endif

/* Define some special macros for devices */
/*-------------------------------- LCD Related --------------------------------*/
#if ESP_PANEL_USE_LCD
    #if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

        #include "hal/spi_types.h"
        #define ESP_PANEL_LCD_BUS_NAME      SPI
        #define ESP_PANEL_LCD_BUS_HOST      ((spi_host_device_t)ESP_PANEL_LCD_BUS_HOST_ID)

    #elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI

        #include "hal/spi_types.h"
        #define ESP_PANEL_LCD_BUS_NAME      QSPI
        #define ESP_PANEL_LCD_BUS_HOST      ((spi_host_device_t)ESP_PANEL_LCD_BUS_HOST_ID)

    #elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB

        #ifndef SOC_LCD_RGB_SUPPORTED
            #error "LCD RGB is only supported for ESP32-S3, please select the correct board."
        #endif
        #define ESP_PANEL_LCD_BUS_NAME      RGB
        #define ESP_PANEL_LCD_BUS_HOST      (-1)

    #else

        #error "Unkonw LCD panel bus type selected, please refer to the README for supported bus types"

    #endif /* ESP_PANEL_LCD_BUS_TYPE */
#endif /* ESP_PANEL_USE_LCD */
/*-------------------------------- Touch Related --------------------------------*/
#if ESP_PANEL_USE_TOUCH
    #if ESP_PANEL_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

        #include "hal/i2c_types.h"
        #define ESP_PANEL_TOUCH_BUS_NAME    I2C
        #define ESP_PANEL_TOUCH_BUS_HOST      ((i2c_port_t)ESP_PANEL_TOUCH_BUS_HOST_ID)

    #elif ESP_PANEL_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

        #include "hal/spi_types.h"
        #define ESP_PANEL_TOUCH_BUS_NAME    SPI
        #define ESP_PANEL_TOUCH_BUS_HOST    ((spi_host_device_t)ESP_PANEL_TOUCH_BUS_HOST_ID)

    #else

        #error "Unkonw Touch bus type selected, please refer to the README for supported bus types."

    #endif /* ESP_PANEL_TOUCH_BUS_TYPE */
#endif /* ESP_PANEL_USE_TOUCH */
/*-------------------------------- IO Expander Related --------------------------------*/
#if ESP_PANEL_USE_EXPANDER

    #include "hal/i2c_types.h"
    #define ESP_PANEL_EXPANDER_HOST      ((i2c_port_t)ESP_PANEL_EXPANDER_HOST_ID)

#endif /* ESP_PANEL_USE_EXPANDER */
#endif /* ESP_PANEL_USE_BOARD */

// *INDENT-OFF*
