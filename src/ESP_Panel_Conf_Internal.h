/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>

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
    #endif
#endif

/* If "ESP_Panel_Conf.h" is not skipped, include it */
#ifndef ESP_PANEL_CONF_SKIP
    #ifdef ESP_PANEL_CONF_PATH                           /* If there is a path defined for "ESP_Panel_Conf.h" use it */
        #define __TO_STR_AUX(x) #x
        #define __TO_STR(x) __TO_STR_AUX(x)
        #include __TO_STR(ESP_PANEL_CONF_PATH)
        #undef __TO_STR_AUX
        #undef __TO_STR
    #elif defined(ESP_PANEL_CONF_INCLUDE_SIMPLE)         /* Or simply include if "ESP_Panel_Conf.h" is available */
        #include "ESP_Panel_Conf.h"
    #elif defined(ESP_PANEL_CONF_INCLUDE_OUTSIDE)
        #include "../../ESP_Panel_Conf.h"                /* Or include if "../../ESP_Panel_Conf.h" is available */
    #else
        #define ESP_PANEL_CONF_IGNORE                    /* Or ignore all configurations if none of the above is available */
    #endif
#endif

#ifndef ESP_PANEL_CHECK_RESULT_ASSERT
    #ifdef CONFIG_ESP_PANEL_CHECK_RESULT_ASSERT
        #define ESP_PANEL_CHECK_RESULT_ASSERT    CONFIG_ESP_PANEL_CHECK_RESULT_ASSERT
    #else
        #define ESP_PANEL_CHECK_RESULT_ASSERT    (0)
    #endif
#endif

#ifndef ESP_PANEL_ENABLE_DEBUG_LOG
    #ifdef CONFIG_ESP_PANEL_ENABLE_DEBUG_LOG
        #define ESP_PANEL_ENABLE_DEBUG_LOG    CONFIG_ESP_PANEL_ENABLE_DEBUG_LOG
    #else
        #define ESP_PANEL_ENABLE_DEBUG_LOG    (0)
    #endif
#endif

#ifndef ESP_PANEL_CONF_IGNORE
#include "esp_idf_version.h"
#include "soc/soc_caps.h"
#include "ESP_PanelBus.h"

#ifndef ESP_PANEL_USE_SUPPORTED_BOARD
    #ifdef CONFIG_ESP_PANEL_USE_SUPPORTED_BOARD
        #define ESP_PANEL_USE_SUPPORTED_BOARD   CONFIG_ESP_PANEL_USE_SUPPORTED_BOARD
    #else
        #define ESP_PANEL_USE_SUPPORTED_BOARD   (0)
    #endif
#endif

#if ESP_PANEL_USE_SUPPORTED_BOARD
    #if defined(ESP_PANEL_BOARD_ESP32_C3_LCDKIT) || CONFIG_ESP_PANEL_BOARD_ESP32_C3_LCDKIT
        #include "board/ESP32_C3_LCDKIT.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_BOX) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_BOX
        #include "board/ESP32_S3_BOX.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_BOX_3) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_BOX_3
        #include "board/esp32_s3_box_3.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_BOX_3_BETA) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_BOX_3_BETA
        #include "board/ESP32_S3_BOX_3_BETA.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_BOX_LITE) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_BOX_LITE
        #include "board/ESP32_S3_BOX_LITE.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_EYE) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_EYE
        #include "board/ESP32_S3_EYE.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_KORVO_2) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_KORVO_2
        #include "board/ESP32_S3_KORVO_2.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD
        #include "board/ESP32_S3_LCD_EV_BOARD.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD_2) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD_2
        #include "board/ESP32_S3_LCD_EV_BOARD_2.h"
    #elif defined(ESP_PANEL_BOARD_ESP32_S3_USB_OTG) || CONFIG_ESP_PANEL_BOARD_ESP32_S3_USB_OTG
        #include "board/ESP32_S3_USB_OTG.h"
    #else
        #error "Unkonw board selected, please refer to the README for supported boards."
    #endif
#endif /* ESP_PANEL_USE_SUPPORTED_BOARD */

/*-------------------------------- LCD Related --------------------------------*/
/* Set to 0 if not using LCD */
#ifndef ESP_PANEL_USE_LCD
    #ifdef CONFIG_ESP_PANEL_USE_LCD
        #define ESP_PANEL_USE_LCD   CONFIG_ESP_PANEL_USE_LCD
    #else
        #define ESP_PANEL_USE_LCD   (0)
    #endif
#endif

#if ESP_PANEL_USE_LCD
    /**
     * LCD controller name.
     */
    #ifndef ESP_PANEL_LCD_NAME
        #if defined(ESP_PANEL_LCD_CONTROLLER_ST7262) || CONFIG_ESP_PANEL_LCD_CONTROLLER_ST7262
            #define ESP_PANEL_LCD_NAME  ST7262
        #elif defined(CONFIG_ESP_PANEL_LCD_CONTROLLER_ST7789) || CONFIG_ESP_PANEL_LCD_CONTROLLER_ST7789
            #define ESP_PANEL_LCD_NAME  ST7789
        #elif defined(CONFIG_ESP_PANEL_LCD_CONTROLLER_GC9503) || CONFIG_ESP_PANEL_LCD_CONTROLLER_GC9503
            #define ESP_PANEL_LCD_NAME  GC9503
        #elif defined(CONFIG_ESP_PANEL_LCD_CONTROLLER_GC9A01) || CONFIG_ESP_PANEL_LCD_CONTROLLER_GC9A01
            #define ESP_PANEL_LCD_NAME  GC9A01
        #else
            #error "Unkonw LCD controller selected, please refer to the README for supported controllers."
        #endif
    #endif

    /* LCD resolution in pixels */
    #ifndef ESP_PANEL_LCD_H_RES
        #ifdef CONFIG_ESP_PANEL_LCD_H_RES
            #define ESP_PANEL_LCD_H_RES CONFIG_ESP_PANEL_LCD_H_RES
        #else
            #error "LCD horizontal resolution is not defined."
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_V_RES
        #ifdef CONFIG_ESP_PANEL_LCD_V_RES
            #define ESP_PANEL_LCD_V_RES CONFIG_ESP_PANEL_LCD_V_RES
        #else
            #error "LCD vertical resolution is not defined."
        #endif
    #endif

    /* LCD Bus Settings */
    /**
     * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
     * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
     */
    #ifndef ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
        #ifdef CONFIG_ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
            #define ESP_PANEL_LCD_BUS_SKIP_INIT_HOST    CONFIG_ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
        #else
            #define ESP_PANEL_LCD_BUS_SKIP_INIT_HOST    (0)
        #endif
    #endif
    /* LCD bus type. */
    #ifndef ESP_PANEL_LCD_BUS_TYPE
        #ifdef CONFIG_ESP_PANEL_LCD_BUS_TYPE
            #define ESP_PANEL_LCD_BUS_TYPE  CONFIG_ESP_PANEL_LCD_BUS_TYPE
        #else
            #error "LCD bus type is not defined."
        #endif
    #endif
    /* LCD bus parameters. */
    #if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
        #define ESP_PANEL_LCD_BUS_NAME      I2C
        #error "LCD I2C bus is not available and will be implemented in the future."
    #elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
        #define ESP_PANEL_LCD_BUS_NAME      SPI

        #ifndef ESP_PANEL_LCD_BUS_HOST_ID
            #ifdef CONFIG_ESP_PANEL_LCD_BUS_HOST_ID
                #define ESP_PANEL_LCD_BUS_HOST_ID   CONFIG_ESP_PANEL_LCD_BUS_HOST_ID
            #else
                #error "LCD bus host id is not defined."
            #endif
        #endif
        #include "hal/spi_types.h"
        #define ESP_PANEL_LCD_BUS_HOST      ((spi_host_device_t)ESP_PANEL_LCD_BUS_HOST_ID)

        #ifndef ESP_PANEL_LCD_SPI_IO_CS
            #ifdef CONFIG_ESP_PANEL_LCD_SPI_IO_CS
                #define ESP_PANEL_LCD_SPI_IO_CS     CONFIG_ESP_PANEL_LCD_SPI_IO_CS
            #else
                #error "LCD SPI CS pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_SPI_IO_DC
            #ifdef CONFIG_ESP_PANEL_LCD_SPI_IO_DC
                #define ESP_PANEL_LCD_SPI_IO_DC     CONFIG_ESP_PANEL_LCD_SPI_IO_DC
            #else
                #error "LCD SPI DC pin is not defined."
            #endif
        #endif

        #if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
            #ifndef ESP_PANEL_LCD_SPI_MODE
                #ifdef CONFIG_ESP_PANEL_LCD_SPI_MODE
                    #define ESP_PANEL_LCD_SPI_MODE  CONFIG_ESP_PANEL_LCD_SPI_MODE
                #else
                    #error "LCD SPI mode is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_SPI_CLK_HZ
                #ifdef CONFIG_ESP_PANEL_LCD_SPI_CLK_HZ
                    #define ESP_PANEL_LCD_SPI_CLK_HZ    CONFIG_ESP_PANEL_LCD_SPI_CLK_HZ
                #else
                    #error "LCD SPI clock is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ
                #ifdef CONFIG_ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ
                    #define ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ    CONFIG_ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ
                #else
                    #error "LCD SPI transaction queue size is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_SPI_CMD_BITS
                #ifdef CONFIG_ESP_PANEL_LCD_SPI_CMD_BITS
                    #define ESP_PANEL_LCD_SPI_CMD_BITS  CONFIG_ESP_PANEL_LCD_SPI_CMD_BITS
                #else
                    #error "LCD SPI command bits is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_SPI_PARAM_BITS
                #ifdef CONFIG_ESP_PANEL_LCD_SPI_PARAM_BITS
                    #define ESP_PANEL_LCD_SPI_PARAM_BITS    CONFIG_ESP_PANEL_LCD_SPI_PARAM_BITS
                #else
                    #error "LCD SPI parameter bits is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_SPI_IO_SCK
                #ifdef CONFIG_ESP_PANEL_LCD_SPI_IO_SCK
                    #define ESP_PANEL_LCD_SPI_IO_SCK    CONFIG_ESP_PANEL_LCD_SPI_IO_SCK
                #else
                    #error "LCD SPI SCK pin is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_SPI_IO_MOSI
                #ifdef CONFIG_ESP_PANEL_LCD_SPI_IO_MOSI
                    #define ESP_PANEL_LCD_SPI_IO_MOSI   CONFIG_ESP_PANEL_LCD_SPI_IO_MOSI
                #else
                    #error "LCD SPI MOSI pin is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_SPI_IO_MISO
                #ifdef CONFIG_ESP_PANEL_LCD_SPI_IO_MISO
                    #define ESP_PANEL_LCD_SPI_IO_MISO   CONFIG_ESP_PANEL_LCD_SPI_IO_MISO
                #else
                    #define ESP_PANEL_LCD_SPI_IO_MISO   (-1)
                #endif
            #endif
        #endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */
    #elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_I80
        #define ESP_PANEL_LCD_BUS_NAME      I80
        #error "LCD I80 bus is not available and will be implemented in the future."
    #elif ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
        #ifndef SOC_LCD_RGB_SUPPORTED
            #error "LCD RGB is only supported for ESP32-S3, please select the correct board."
        #endif

        #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
            #error "LCD RGB bus here is only supported in arduino-esp32 v3.x.x (ESP-IDF v5.1), please update the SDK."
        #endif

        #define ESP_PANEL_LCD_BUS_NAME      RGB
        #define ESP_PANEL_LCD_BUS_HOST      (-1)

        #ifndef ESP_PANEL_LCD_RGB_CLK_HZ
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_CLK_HZ
                #define ESP_PANEL_LCD_RGB_CLK_HZ    CONFIG_ESP_PANEL_LCD_RGB_CLK_HZ
            #else
                #error "LCD RGB clock is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_HPW
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_HPW
                #define ESP_PANEL_LCD_RGB_HPW   CONFIG_ESP_PANEL_LCD_RGB_HPW
            #else
                #error "LCD RGB HPW is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_HBP
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_HBP
                #define ESP_PANEL_LCD_RGB_HBP   CONFIG_ESP_PANEL_LCD_RGB_HBP
            #else
                #error "LCD RGB HBP is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_HFP
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_HFP
                #define ESP_PANEL_LCD_RGB_HFP   CONFIG_ESP_PANEL_LCD_RGB_HFP
            #else
                #error "LCD RGB HFP is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_VPW
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_VPW
                #define ESP_PANEL_LCD_RGB_VPW   CONFIG_ESP_PANEL_LCD_RGB_VPW
            #else
                #error "LCD RGB VPW is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_VBP
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_VBP
                #define ESP_PANEL_LCD_RGB_VBP   CONFIG_ESP_PANEL_LCD_RGB_VBP
            #else
                #error "LCD RGB VBP is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_VFP
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_VFP
                #define ESP_PANEL_LCD_RGB_VFP   CONFIG_ESP_PANEL_LCD_RGB_VFP
            #else
                #error "LCD RGB VFP is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_PCLK_ACTIVE_NEG
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_PCLK_ACTIVE_NEG
                #define ESP_PANEL_LCD_RGB_PCLK_ACTIVE_NEG   CONFIG_ESP_PANEL_LCD_RGB_PCLK_ACTIVE_NEG
            #else
                #error "LCD RGB PCLK active level is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_DATA_WIDTH
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_DATA_WIDTH
                #define ESP_PANEL_LCD_RGB_DATA_WIDTH    CONFIG_ESP_PANEL_LCD_RGB_DATA_WIDTH
            #else
                #error "LCD RGB data width is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_FRAME_BUF_NUM
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_FRAME_BUF_NUM
                #define ESP_PANEL_LCD_RGB_FRAME_BUF_NUM    CONFIG_ESP_PANEL_LCD_RGB_FRAME_BUF_NUM
            #else
                #define ESP_PANEL_LCD_RGB_FRAME_BUF_NUM    (1)
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE
                #define ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE    CONFIG_ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE
            #else
                #define ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE   (ESP_PANEL_LCD_H_RES * 10)
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_HSYNC
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_HSYNC
                #define ESP_PANEL_LCD_RGB_IO_HSYNC  CONFIG_ESP_PANEL_LCD_RGB_IO_HSYNC
            #else
                #error "LCD RGB HSYNC pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_VSYNC
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_VSYNC
                #define ESP_PANEL_LCD_RGB_IO_VSYNC  CONFIG_ESP_PANEL_LCD_RGB_IO_VSYNC
            #else
                #error "LCD RGB VSYNC pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DE
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DE
                #define ESP_PANEL_LCD_RGB_IO_DE     CONFIG_ESP_PANEL_LCD_RGB_IO_DE
            #else
                #error "LCD RGB DE pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_PCLK
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_PCLK
                #define ESP_PANEL_LCD_RGB_IO_PCLK   CONFIG_ESP_PANEL_LCD_RGB_IO_PCLK
            #else
                #error "LCD RGB PCLK pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DISP
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DISP
                #define ESP_PANEL_LCD_RGB_IO_DISP   CONFIG_ESP_PANEL_LCD_RGB_IO_DISP
            #else
                #define ESP_PANEL_LCD_RGB_IO_DISP   (-1)
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA0
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA0
                #define ESP_PANEL_LCD_RGB_IO_DATA0      CONFIG_ESP_PANEL_LCD_RGB_IO_DATA0
            #else
                #error "LCD RGB DATA0 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_PIXEL_BITS
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_PIXEL_BITS
                #define ESP_PANEL_LCD_RGB_PIXEL_BITS    CONFIG_ESP_PANEL_LCD_RGB_PIXEL_BITS
            #else
                #error "LCD RGB pixel bits is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA1
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA1
                #define ESP_PANEL_LCD_RGB_IO_DATA1  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA1
            #else
                #error "LCD RGB DATA1 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA2
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA2
                #define ESP_PANEL_LCD_RGB_IO_DATA2  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA2
            #else
                #error "LCD RGB DATA2 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA3
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA3
                #define ESP_PANEL_LCD_RGB_IO_DATA3  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA3
            #else
                #error "LCD RGB DATA3 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA4
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA4
                #define ESP_PANEL_LCD_RGB_IO_DATA4  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA4
            #else
                #error "LCD RGB DATA4 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA5
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA5
                #define ESP_PANEL_LCD_RGB_IO_DATA5  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA5
            #else
                #error "LCD RGB DATA5 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA6
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA6
                #define ESP_PANEL_LCD_RGB_IO_DATA6  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA6
            #else
                #error "LCD RGB DATA6 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA7
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA7
                #define ESP_PANEL_LCD_RGB_IO_DATA7  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA7
            #else
                #error "LCD RGB DATA7 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA8
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA8
                #define ESP_PANEL_LCD_RGB_IO_DATA8  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA8
            #elif ESP_PANEL_LCD_RGB_DATA_WIDTH = 8
                #define ESP_PANEL_LCD_RGB_IO_DATA8  (-1)
            #else
                #error "LCD RGB DATA8 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA9
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA9
                #define ESP_PANEL_LCD_RGB_IO_DATA9  CONFIG_ESP_PANEL_LCD_RGB_IO_DATA9
            #elif ESP_PANEL_LCD_RGB_DATA_WIDTH = 8
                #define ESP_PANEL_LCD_RGB_IO_DATA9  (-1)
            #else
                #error "LCD RGB DATA9 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA10
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA10
                #define ESP_PANEL_LCD_RGB_IO_DATA10 CONFIG_ESP_PANEL_LCD_RGB_IO_DATA10
            #elif ESP_PANEL_LCD_RGB_DATA_WIDTH = 8
                #define ESP_PANE_LCD_RGB_IO_DATA10  (-1)
            #else
                #error "LCD RGB DATA10 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA11
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA11
                #define ESP_PANEL_LCD_RGB_IO_DATA11 CONFIG_ESP_PANEL_LCD_RGB_IO_DATA11
            #elif ESP_PANEL_LCD_RGB_DATA_WIDTH = 8
                #define ESP_PANEL_LCD_RGB_IO_DATA11  (-1)
            #else
                #error "LCD RGB DATA11 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA12
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA12
                #define ESP_PANEL_LCD_RGB_IO_DATA12 CONFIG_ESP_PANEL_LCD_RGB_IO_DATA12
            #elif ESP_PANEL_LCD_RGB_DATA_WIDTH = 8
                #define ESP_PANEL_LCD_RGB_IO_DATA12  (-1)
            #else
                #error "LCD RGB DATA12 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA13
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA13
                #define ESP_PANEL_LCD_RGB_IO_DATA13 CONFIG_ESP_PANEL_LCD_RGB_IO_DATA13
            #elif ESP_PANEL_LCD_RGB_DATA_WIDTH = 8
                #define ESP_PANEL_LCD_RGB_IO_DATA13  (-1)
            #else
                #error "LCD RGB DATA13 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA14
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA14
                #define ESP_PANEL_LCD_RGB_IO_DATA14 CONFIG_ESP_PANEL_LCD_RGB_IO_DATA14
            #elif ESP_PANEL_LCD_RGB_DATA_WIDTH = 8
                #define ESP_PANEL_LCD_RGB_IO_DATA14  (-1)
            #else
                #error "LCD RGB DATA14 pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_LCD_RGB_IO_DATA15
            #ifdef CONFIG_ESP_PANEL_LCD_RGB_IO_DATA15
                #define ESP_PANEL_LCD_RGB_IO_DATA15 CONFIG_ESP_PANEL_LCD_RGB_IO_DATA15
            #elif ESP_PANEL_LCD_RGB_DATA_WIDTH = 8
                #define ESP_PANEL_LCD_RGB_IO_DATA15  (-1)
            #else
                #error "LCD RGB DATA15 pin is not defined."
            #endif
        #endif

        #if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
            #ifndef ESP_PANEL_LCD_3WIRE_SPI_SCL_ACTIVE_EDGE
                #ifdef ESP_PANEL_LCD_3WIRE_SPI_SCL_ACTIVE_EDGE
                    #define ESP_PANEL_LCD_3WIRE_SPI_SCL_ACTIVE_EDGE   CONFIG_ESP_PANEL_LCD_3WIRE_SPI_SCL_ACTIVE_EDGE
                #else
                    #error "LCD 3-wire SPI SCL active edge is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_3WIRE_SPI_AUTO_DEL_PANEL_IO
                #ifdef ESP_PANEL_LCD_3WIRE_SPI_AUTO_DEL_PANEL_IO
                    #define ESP_PANEL_LCD_3WIRE_SPI_AUTO_DEL_PANEL_IO   CONFIG_ESP_PANEL_LCD_3WIRE_SPI_AUTO_DEL_PANEL_IO
                #else
                    #error "LCD 3-wire SPI auto delete panel io is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_3WIRE_SPI_CS_USE_EXPNADER
                #ifdef CONFIG_ESP_PANEL_LCD_3WIRE_SPI_CS_USE_EXPNADER
                    #define ESP_PANEL_LCD_3WIRE_SPI_CS_USE_EXPNADER  CONFIG_ESP_PANEL_LCD_3WIRE_SPI_CS_USE_EXPNADER
                #else
                    #define ESP_PANEL_LCD_3WIRE_SPI_CS_USE_EXPNADER  (0)
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_3WIRE_SPI_SCL_USE_EXPNADER
                #ifdef CONFIG_ESP_PANEL_LCD_3WIRE_SPI_SCL_USE_EXPNADER
                    #define ESP_PANEL_LCD_3WIRE_SPI_SCL_USE_EXPNADER CONFIG_ESP_PANEL_LCD_3WIRE_SPI_SCL_USE_EXPNADER
                #else
                    #define ESP_PANEL_LCD_3WIRE_SPI_SCL_USE_EXPNADER (0)
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_3WIRE_SPI_SDA_USE_EXPNADER
                #ifdef CONFIG_ESP_PANEL_LCD_3WIRE_SPI_SDA_USE_EXPNADER
                    #define ESP_PANEL_LCD_3WIRE_SPI_SDA_USE_EXPNADER CONFIG_ESP_PANEL_LCD_3WIRE_SPI_SDA_USE_EXPNADER
                #else
                    #define ESP_PANEL_LCD_3WIRE_SPI_SDA_USE_EXPNADER (0)
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_3WIRE_SPI_IO_CS
                #ifdef CONFIG_ESP_PANEL_LCD_3WIRE_SPI_IO_CS
                    #define ESP_PANEL_LCD_3WIRE_SPI_IO_CS CONFIG_ESP_PANEL_LCD_3WIRE_SPI_IO_CS
                #else
                    #error "LCD SPI CS pin is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_3WIRE_SPI_IO_SCL
                #ifdef CONFIG_ESP_PANEL_LCD_3WIRE_SPI_IO_SCL
                    #define ESP_PANEL_LCD_3WIRE_SPI_IO_SCL    CONFIG_ESP_PANEL_LCD_3WIRE_SPI_IO_SCL
                #else
                    #error "LCD SPI SCL pin is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_3WIRE_SPI_IO_SDA
                #ifdef CONFIG_ESP_PANEL_LCD_3WIRE_SPI_IO_SDA
                    #define ESP_PANEL_LCD_3WIRE_SPI_IO_SDA    CONFIG_ESP_PANEL_LCD_3WIRE_SPI_IO_SDA
                #else
                    #error "LCD SPI SDA pin is not defined."
                #endif
            #endif
        #endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */
    #else
        #error "Unkonw LCD bus type selected, please refer to the README for supported bus types."
    #endif /* ESP_PANEL_LCD_BUS_TYPE */

    /* ESP_PANEL_LCD_INIT_CMD_SIZE */
    #ifndef ESP_PANEL_LCD_INIT_CMD_SIZE
        #define ESP_PANEL_LCD_INIT_CMD_SIZE     (0)
    #endif

    /* ESP_PANEL_LCD_INIT_CMD */
    #ifndef ESP_PANEL_LCD_INIT_CMD
        #define ESP_PANEL_LCD_INIT_CMD          (NULL)
        #if ESP_PANEL_LCD_INIT_CMD_SIZE > 0
            #undef ESP_PANEL_LCD_INIT_CMD_SIZE
            #define ESP_PANEL_LCD_INIT_CMD_SIZE     (0)
        #endif
    #endif

    /* LCD Color Settings */
    /* LCD color depth in bits */
    #ifndef ESP_PANEL_LCD_COLOR_BITS
        #ifdef CONFIG_ESP_PANEL_LCD_COLOR_BITS
            #define ESP_PANEL_LCD_COLOR_BITS    CONFIG_ESP_PANEL_LCD_COLOR_BITS
        #else
            #define ESP_PANEL_LCD_COLOR_BITS    (16)
        #endif
    #endif
   /* LCD Color Space. */
    #ifndef ESP_PANEL_LCD_RGB_ORDER
        #ifdef CONFIG_ESP_PANEL_LCD_RGB_ORDER
            #define ESP_PANEL_LCD_RGB_ORDER   CONFIG_ESP_PANEL_LCD_RGB_ORDER
        #else
            #define ESP_PANEL_LCD_RGB_ORDER     (0)
        #endif
    #endif

    /* LCD Transformation Flags */
    #ifndef ESP_PANEL_LCD_SWAP_XY
        #ifdef CONFIG_ESP_PANEL_LCD_SWAP_XY
            #define ESP_PANEL_LCD_SWAP_XY  CONFIG_ESP_PANEL_LCD_SWAP_XY
        #else
            #define ESP_PANEL_LCD_SWAP_XY  (0)
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_MIRROR_X
        #ifdef CONFIG_ESP_PANEL_LCD_MIRROR_X
            #define ESP_PANEL_LCD_MIRROR_X  CONFIG_ESP_PANEL_LCD_MIRROR_X
        #else
            #define ESP_PANEL_LCD_MIRROR_X  (0)
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_MIRROR_Y
        #ifdef CONFIG_ESP_PANEL_LCD_MIRROR_Y
            #define ESP_PANEL_LCD_MIRROR_Y  CONFIG_ESP_PANEL_LCD_MIRROR_Y
        #else
            #define ESP_PANEL_LCD_MIRROR_Y  (0)
        #endif
    #endif

    /* LCD Other Settings */
    #ifndef ESP_PANEL_LCD_IO_RST
        #ifdef CONFIG_ESP_PANEL_LCD_IO_RST
            #define ESP_PANEL_LCD_IO_RST    CONFIG_ESP_PANEL_LCD_IO_RST
        #else
            #define ESP_PANEL_LCD_IO_RST    (-1)
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_RST_LEVEL
        #ifdef CONFIG_ESP_PANEL_LCD_RST_LEVEL
            #define ESP_PANEL_LCD_RST_LEVEL CONFIG_ESP_PANEL_LCD_RST_LEVEL
        #else
            #define ESP_PANEL_LCD_RST_LEVEL (0)
        #endif
    #endif
#endif /* ESP_PANEL_USE_LCD */

/*-------------------------------- LCD Touch Related --------------------------------*/
/* Set to 0 if not using LCD touch */
#ifndef ESP_PANEL_USE_LCD_TOUCH
    #ifdef CONFIG_ESP_PANEL_USE_LCD_TOUCH
        #define ESP_PANEL_USE_LCD_TOUCH    CONFIG_ESP_PANEL_USE_LCD_TOUCH
    #else
        #define ESP_PANEL_USE_LCD_TOUCH    (0)
    #endif
#endif

#if ESP_PANEL_USE_LCD_TOUCH
    /* LCD touch IC name. */
    #ifndef ESP_PANEL_LCD_TOUCH_NAME
        #if defined(ESP_PANEL_LCD_TOUCH_CONTROLLER_CST816S) || CONFIG_ESP_PANEL_LCD_TOUCH_CONTROLLER_CST816S
            #define ESP_PANEL_LCD_TOUCH_NAME  CST816S
        #elif defined(ESP_PANEL_LCD_TOUCH_CONTROLLER_FT5X06) || CONFIG_ESP_PANEL_LCD_TOUCH_CONTROLLER_FT5X06
            #define ESP_PANEL_LCD_TOUCH_NAME  FT5x06
        #elif defined(ESP_PANEL_LCD_TOUCH_CONTROLLER_GT1151) || CONFIG_ESP_PANEL_LCD_TOUCH_CONTROLLER_GT1151
            #define ESP_PANEL_LCD_TOUCH_NAME  GT1151
        #elif defined(ESP_PANEL_LCD_TOUCH_CONTROLLER_GT911) || CONFIG_ESP_PANEL_LCD_TOUCH_CONTROLLER_GT911
            #define ESP_PANEL_LCD_TOUCH_NAME  GT911
        #elif defined(ESP_PANEL_LCD_TOUCH_CONTROLLER_TT21100) || CONFIG_ESP_PANEL_LCD_TOUCH_CONTROLLER_TT21100
            #define ESP_PANEL_LCD_TOUCH_NAME  TT21100
        #elif defined(ESP_PANEL_LCD_TOUCH_CONTROLLER_STMPE610) || CONFIG_ESP_PANEL_LCD_TOUCH_CONTROLLER_STMPE610
            #define ESP_PANEL_LCD_TOUCH_NAME  STMPE610
        #else
            #error "Unkonw LCD Touch controller selected, please refer to the README for supported controllers."
        #endif
    #endif

    /* LCD touch resolution in pixels */
    #ifndef ESP_PANEL_LCD_TOUCH_H_RES
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_H_RES
            #define ESP_PANEL_LCD_TOUCH_H_RES CONFIG_ESP_PANEL_LCD_TOUCH_H_RES
        #elif defined(ESP_PANEL_LCD_H_RES)
            #define ESP_PANEL_LCD_TOUCH_H_RES ESP_PANEL_LCD_H_RES
        #elif defined(CONFIG_ESP_PANEL_LCD_H_RES)
            #define ESP_PANEL_LCD_TOUCH_H_RES CONFIG_ESP_PANEL_LCD_H_RES
        #else
            #error "LCD touch horizontal resolution is not defined."
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_TOUCH_V_RES
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_V_RES
            #define ESP_PANEL_LCD_TOUCH_V_RES CONFIG_ESP_PANEL_LCD_TOUCH_V_RES
        #elif defined(ESP_PANEL_LCD_V_RES)
            #define ESP_PANEL_LCD_TOUCH_V_RES ESP_PANEL_LCD_V_RES
        #elif defined(CONFIG_ESP_PANEL_LCD_V_RES)
            #define ESP_PANEL_LCD_TOUCH_V_RES CONFIG_ESP_PANEL_LCD_V_RES
        #else
            #error "LCD touch vertical resolution is not defined."
        #endif
    #endif

    /* LCD Touch Bus Settings */
    /**
     * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
     * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
     */
    #ifndef ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
            #define ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST    CONFIG_ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
        #else
            #define ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST    (0)
        #endif
    #endif
    /* LCD bus type. */
    #ifndef ESP_PANEL_LCD_TOUCH_BUS_TYPE
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_BUS_TYPE
            #define ESP_PANEL_LCD_TOUCH_BUS_TYPE  CONFIG_ESP_PANEL_LCD_TOUCH_BUS_TYPE
        #else
            #error "LCD bus type is not defined."
        #endif
    #endif
    /* LCD bus parameters. */
    #if ESP_PANEL_LCD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
        #define ESP_PANEL_LCD_TOUCH_BUS_NAME    I2C

        #ifndef ESP_PANEL_LCD_TOUCH_BUS_HOST_ID
            #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_BUS_HOST_ID
                #define ESP_PANEL_LCD_TOUCH_BUS_HOST_ID    CONFIG_ESP_PANEL_LCD_TOUCH_BUS_HOST_ID
            #else
                #define ESP_PANEL_LCD_TOUCH_BUS_HOST_ID    (0)
            #endif
        #endif
        #include "hal/i2c_types.h"
        #define ESP_PANEL_LCD_TOUCH_BUS_HOST      ((i2c_port_t)ESP_PANEL_LCD_TOUCH_BUS_HOST_ID)

        #if !ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
            #ifndef ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ
                #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ
                    #define ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ  CONFIG_ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ
                #else
                    #define ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ  (400 * 1000)
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP
                #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP
                    #define ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP  CONFIG_ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP
                #else
                    #define ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP  (0)
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP
                #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP
                    #define ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP  CONFIG_ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP
                #else
                    #define ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP  (0)
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_TOUCH_I2C_IO_SCL
                #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_I2C_IO_SCL
                    #define ESP_PANEL_LCD_TOUCH_I2C_IO_SCL  CONFIG_ESP_PANEL_LCD_TOUCH_I2C_IO_SCL
                #else
                    #error "LCD touch I2C SCL pin is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_TOUCH_I2C_IO_SDA
                #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_I2C_IO_SDA
                    #define ESP_PANEL_LCD_TOUCH_I2C_IO_SDA  CONFIG_ESP_PANEL_LCD_TOUCH_I2C_IO_SDA
                #else
                    #error "LCD touch I2C SDA pin is not defined."
                #endif
            #endif
        #endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */
    #elif ESP_PANEL_LCD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
        #define ESP_PANEL_LCD_TOUCH_BUS_NAME    SPI

        #ifndef ESP_PANEL_LCD_TOUCH_BUS_HOST_ID
            #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_BUS_HOST_ID
                #define ESP_PANEL_LCD_TOUCH_BUS_HOST_ID    CONFIG_ESP_PANEL_LCD_TOUCH_BUS_HOST_ID
            #else
                #define ESP_PANEL_LCD_TOUCH_BUS_HOST_ID    (1)
            #endif
        #endif
        #define ESP_PANEL_LCD_TOUCH_BUS_HOST      ((spi_host_device_t)ESP_PANEL_LCD_TOUCH_BUS_HOST_ID)

        #ifndef ESP_PANEL_LCD_TOUCH_SPI_IO_CS
            #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_SPI_IO_CS
                #define ESP_PANEL_LCD_TOUCH_SPI_IO_CS CONFIG_ESP_PANEL_LCD_TOUCH_SPI_IO_CS
            #else
                #error "LCD touch SPI CS pin is not defined."
            #endif
        #endif
        #if !ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST
            #ifndef ESP_PANEL_LCD_TOUCH_SPI_IO_SCK
                #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_SPI_IO_SCK
                    #define ESP_PANEL_LCD_TOUCH_SPI_IO_SCK    CONFIG_ESP_PANEL_LCD_TOUCH_SPI_IO_SCK
                #else
                    #error "LCD touch SPI SCK pin is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_TOUCH_SPI_IO_MOSI
                #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_SPI_IO_MOSI
                    #define ESP_PANEL_LCD_TOUCH_SPI_IO_MOSI   CONFIG_ESP_PANEL_LCD_TOUCH_SPI_IO_MOSI
                #else
                    #error "LCD touch SPI MOSI pin is not defined."
                #endif
            #endif

            #ifndef ESP_PANEL_LCD_TOUCH_SPI_IO_MISO
                #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_SPI_IO_MISO
                    #define ESP_PANEL_LCD_TOUCH_SPI_IO_MISO   CONFIG_ESP_PANEL_LCD_TOUCH_SPI_IO_MISO
                #else
                    #error "LCD touch SPI MISO pin is not defined."
                #endif
            #endif
        #endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */
    #else
        #error "Unkonw LCD touch bus type selected, please refer to the README for supported bus types."
    #endif /* ESP_PANEL_LCD_TOUCH_BUS_TYPE */

    /* LCD Touch Transformation Flags */
    #ifndef ESP_PANEL_LCD_TOUCH_SWAP_XY
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_SWAP_XY
            #define ESP_PANEL_LCD_TOUCH_SWAP_XY    CONFIG_ESP_PANEL_LCD_TOUCH_SWAP_XY
        #else
            #define ESP_PANEL_LCD_TOUCH_SWAP_XY    (0)
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_TOUCH_MIRROR_X
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_MIRROR_X
            #define ESP_PANEL_LCD_TOUCH_MIRROR_X    CONFIG_ESP_PANEL_LCD_TOUCH_MIRROR_X
        #else
            #define ESP_PANEL_LCD_TOUCH_MIRROR_X    (0)
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_TOUCH_MIRROR_Y
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_MIRROR_Y
            #define ESP_PANEL_LCD_TOUCH_MIRROR_Y    CONFIG_ESP_PANEL_LCD_TOUCH_MIRROR_Y
        #else
            #define ESP_PANEL_LCD_TOUCH_MIRROR_Y    (0)
        #endif
    #endif

    /* LCD Touch Other Settings */
    #ifndef ESP_PANEL_LCD_TOUCH_IO_RST
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_IO_RST
            #define ESP_PANEL_LCD_TOUCH_IO_RST  CONFIG_ESP_PANEL_LCD_TOUCH_IO_RST
        #else
            #define ESP_PANEL_LCD_TOUCH_IO_RST  (-1)
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_TOUCH_IO_INT
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_IO_INT
            #define ESP_PANEL_LCD_TOUCH_IO_INT  CONFIG_ESP_PANEL_LCD_TOUCH_IO_INT
        #else
            #define ESP_PANEL_LCD_TOUCH_IO_INT  (-1)
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_TOUCH_RST_LEVEL
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_RST_LEVEL
            #define ESP_PANEL_LCD_TOUCH_RST_LEVEL CONFIG_ESP_PANEL_LCD_TOUCH_RST_LEVEL
        #else
            #define ESP_PANEL_LCD_TOUCH_RST_LEVEL (0)
        #endif
    #endif
    #ifndef ESP_PANEL_LCD_TOUCH_INT_LEVEL
        #ifdef CONFIG_ESP_PANEL_LCD_TOUCH_INT_LEVEL
            #define ESP_PANEL_LCD_TOUCH_INT_LEVEL CONFIG_ESP_PANEL_LCD_TOUCH_INT_LEVEL
        #else
            #define ESP_PANEL_LCD_TOUCH_INT_LEVEL (0)
        #endif
    #endif
#endif /* ESP_PANEL_USE_LCD_TOUCH */

/*-------------------------------- Backlight Related --------------------------------*/
#ifndef ESP_PANEL_USE_BL
    #ifdef CONFIG_ESP_PANEL_USE_BL
        #define ESP_PANEL_USE_BL    CONFIG_ESP_PANEL_USE_BL
    #else
        #define ESP_PANEL_USE_BL    (0)
    #endif
#endif

#if ESP_PANEL_USE_BL
    /* IO num of backlight pin */
    #ifndef ESP_PANEL_LCD_IO_BL
        #ifdef CONFIG_ESP_PANEL_LCD_IO_BL
            #define ESP_PANEL_LCD_IO_BL CONFIG_ESP_PANEL_LCD_IO_BL
        #else
            #error "Backlight pin is not defined."
        #endif
    #endif

    /* If the backlight is on when high level, set to 1; otherwise to 0 */
    #if ESP_PANEL_LCD_IO_BL >= 0
        #ifndef ESP_PANEL_LCD_BL_LEVEL
            #ifdef CONFIG_ESP_PANEL_LCD_BL_LEVEL
                #define ESP_PANEL_LCD_BL_LEVEL  CONFIG_ESP_PANEL_LCD_BL_LEVEL
            #else
                #define ESP_PANEL_LCD_BL_LEVEL  (0)
            #endif
        #endif
    #endif

    /* Set to 1 if use PWM for backlight brightness control. */
    #ifndef ESP_PANEL_LCD_BL_USE_PWM
        #ifdef CONFIG_ESP_PANEL_LCD_BL_USE_PWM
            #define ESP_PANEL_LCD_BL_USE_PWM    CONFIG_ESP_PANEL_LCD_BL_USE_PWM
        #else
            #define ESP_PANEL_LCD_BL_USE_PWM    (0)
        #endif
    #endif

    #if ESP_PANEL_LCD_BL_USE_PWM
        /**
 *  Backlight LEDC Parameters.
 *
 *  Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/ledc.html for details.
 */
        #ifndef ESP_PANEL_LCD_BL_PWM_TIMER
            #ifdef CONFIG_ESP_PANEL_LCD_BL_PWM_TIMER
                #define ESP_PANEL_LCD_BL_PWM_TIMER  CONFIG_ESP_PANEL_LCD_BL_PWM_TIMER
            #else
                #define ESP_PANEL_LCD_BL_PWM_TIMER  (0)
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_BL_PWM_CHANNEL
            #ifdef CONFIG_ESP_PANEL_LCD_BL_PWM_CHANNEL
                #define ESP_PANEL_LCD_BL_PWM_CHANNEL    CONFIG_ESP_PANEL_LCD_BL_PWM_CHANNEL
            #else
                #define ESP_PANEL_LCD_BL_PWM_CHANNEL    (0)
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_BL_PWM_RESOLUTION
            #ifdef CONFIG_ESP_PANEL_LCD_BL_PWM_RESOLUTION
                #define ESP_PANEL_LCD_BL_PWM_RESOLUTION CONFIG_ESP_PANEL_LCD_BL_PWM_RESOLUTION
            #else
                #define ESP_PANEL_LCD_BL_PWM_RESOLUTION (10)
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_BL_PWM_FREQ_HZ
            #ifdef CONFIG_ESP_PANEL_LCD_BL_PWM_FREQ_HZ
                #define ESP_PANEL_LCD_BL_PWM_FREQ_HZ    CONFIG_ESP_PANEL_LCD_BL_PWM_FREQ_HZ
            #else
                #define ESP_PANEL_LCD_BL_PWM_FREQ_HZ    (5000)
            #endif
        #endif
    #endif /* ESP_PANEL_LCD_BL_USE_PWM */
#endif /* ESP_PANEL_USE_BL */

/*-------------------------------- IO Expander Related --------------------------------*/
/* Set to 0 if not using IO expander */
#ifndef ESP_PANEL_USE_EXPANDER
    #ifdef CONFIG_ESP_PANEL_USE_EXPANDER
        #define ESP_PANEL_USE_EXPANDER    CONFIG_ESP_PANEL_USE_EXPANDER
    #else
        #define ESP_PANEL_USE_EXPANDER    (0)
    #endif
#endif

#if ESP_PANEL_USE_EXPANDER
    /* IO expander IC name. */
    #ifndef ESP_PANEL_EXPANDER_NAME
        #if defined(ESP_PANEL_EXPANDER_CONTROLLER_CST816S) || CONFIG_ESP_PANEL_EXPANDER_CONTROLLER_CST816S
            #define ESP_PANEL_EXPANDER_NAME  CST816S
        #elif defined(ESP_PANEL_EXPANDER_CONTROLLER_FT5X06) || CONFIG_ESP_PANEL_EXPANDER_CONTROLLER_FT5X06
            #define ESP_PANEL_EXPANDER_NAME  FT5x06
        #elif defined(ESP_PANEL_EXPANDER_CONTROLLER_GT1151) || CONFIG_ESP_PANEL_EXPANDER_CONTROLLER_GT1151
            #define ESP_PANEL_EXPANDER_NAME  GT1151
        #else
            #error "Unkonw LCD Touch controller selected, please refer to the README for supported controllers."
        #endif
    #endif

    /* LCD Touch Bus Settings */
    /**
     * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
     * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
     */
    #ifndef ESP_PANEL_EXPANDER_SKIP_INIT_HOST
        #ifdef CONFIG_ESP_PANEL_EXPANDER_SKIP_INIT_HOST
            #define ESP_PANEL_EXPANDER_SKIP_INIT_HOST    CONFIG_ESP_PANEL_EXPANDER_SKIP_INIT_HOST
        #else
            #define ESP_PANEL_EXPANDER_SKIP_INIT_HOST    (0)
        #endif
    #endif

    #ifndef ESP_PANEL_EXPANDER_HOST_ID
        #ifdef CONFIG_ESP_PANEL_EXPANDER_HOST_ID
            #define ESP_PANEL_EXPANDER_HOST_ID    CONFIG_ESP_PANEL_EXPANDER_HOST_ID
        #else
            #define ESP_PANEL_EXPANDER_HOST_ID    (0)
        #endif
    #endif

    #include "hal/i2c_types.h"
    #define ESP_PANEL_EXPANDER_HOST      ((i2c_port_t)ESP_PANEL_EXPANDER_HOST_ID)

    #if !ESP_PANEL_EXPANDER_SKIP_INIT_HOST
        #ifndef ESP_PANEL_EXPANDER_I2C_CLK_HZ
            #ifdef CONFIG_ESP_PANEL_EXPANDER_I2C_CLK_HZ
                #define ESP_PANEL_EXPANDER_I2C_CLK_HZ  CONFIG_ESP_PANEL_EXPANDER_I2C_CLK_HZ
            #else
                #define ESP_PANEL_EXPANDER_I2C_CLK_HZ  (400 * 1000)
            #endif
        #endif

        #ifndef ESP_PANEL_EXPANDER_I2C_SCL_PULLUP
            #ifdef CONFIG_ESP_PANEL_EXPANDER_I2C_SCL_PULLUP
                #define ESP_PANEL_EXPANDER_I2C_SCL_PULLUP  CONFIG_ESP_PANEL_EXPANDER_I2C_SCL_PULLUP
            #else
                #define ESP_PANEL_EXPANDER_I2C_SCL_PULLUP  (0)
            #endif
        #endif

        #ifndef ESP_PANEL_EXPANDER_I2C_SDA_PULLUP
            #ifdef CONFIG_ESP_PANEL_EXPANDER_I2C_SDA_PULLUP
                #define ESP_PANEL_EXPANDER_I2C_SDA_PULLUP  CONFIG_ESP_PANEL_EXPANDER_I2C_SDA_PULLUP
            #else
                #define ESP_PANEL_EXPANDER_I2C_SDA_PULLUP  (0)
            #endif
        #endif

        #ifndef ESP_PANEL_EXPANDER_I2C_IO_SCL
            #ifdef CONFIG_ESP_PANEL_EXPANDER_I2C_IO_SCL
                #define ESP_PANEL_EXPANDER_I2C_IO_SCL  CONFIG_ESP_PANEL_EXPANDER_I2C_IO_SCL
            #else
                #error "IO expander I2C SCL pin is not defined."
            #endif
        #endif

        #ifndef ESP_PANEL_EXPANDER_I2C_IO_SDA
            #ifdef CONFIG_ESP_PANEL_EXPANDER_I2C_IO_SDA
                #define ESP_PANEL_EXPANDER_I2C_IO_SDA  CONFIG_ESP_PANEL_EXPANDER_I2C_IO_SDA
            #else
                #error "IO expander I2C SDA pin is not defined."
            #endif
        #endif
    #endif /* ESP_PANEL_EXPANDER_SKIP_INIT_HOST */
#endif /* ESP_PANEL_USE_EXPANDER */
// *INDENT-OFF*

#endif /* ESP_PANEL_CONF_IGNORE */
