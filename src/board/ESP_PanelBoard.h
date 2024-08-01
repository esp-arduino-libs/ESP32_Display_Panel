/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

// Check if multiple boards are enabled
#if defined(BOARD_ESP32_C3_LCDKIT) + defined(BOARD_ESP32_S3_BOX) + defined(BOARD_ESP32_S3_BOX_3) + \
    defined(BOARD_ESP32_S3_BOX_3_BETA) + defined(BOARD_ESP32_S3_BOX_LITE) + defined(BOARD_ESP32_S3_EYE) + \
    defined(BOARD_ESP32_S3_KORVO_2) + defined(BOARD_ESP32_S3_LCD_EV_BOARD) + \
    defined(BOARD_ESP32_S3_LCD_EV_BOARD_V1_5) +  defined(BOARD_ESP32_S3_LCD_EV_BOARD_2) + \
    defined(BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5) + defined(BOARD_ESP32_S3_USB_OTG) +  defined(BOARD_ELECROW_CROWPANEL_7_0) + \
    defined(BOARD_M5STACK_M5CORE2) + defined(BOARD_M5STACK_M5DIAL) + defined(BOARD_M5STACK_M5CORES3) + \
    defined(BOARD_ESP32_4848S040C_I_Y_3) + defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_4_3) \
    > 1
    #error "Multiple boards enabled! Please check file `ESP_Panel_Board_Supported.h` and make sure only one board is enabled."
#endif

// Include board specific header file
/* Espressif */
#if defined(BOARD_ESP32_C3_LCDKIT) || CONFIG_BOARD_ESP32_C3_LCDKIT
    #include "board/espressif/ESP32_C3_LCDKIT.h"
#elif defined(BOARD_ESP32_S3_BOX) || CONFIG_BOARD_ESP32_S3_BOX
    #include "board/espressif/ESP32_S3_BOX.h"
#elif defined(BOARD_ESP32_S3_BOX_3) || CONFIG_BOARD_ESP32_S3_BOX_3
    #include "board/espressif/ESP32_S3_BOX_3.h"
#elif defined(BOARD_ESP32_S3_BOX_3_BETA) || CONFIG_BOARD_ESP32_S3_BOX_3_BETA
    #include "board/espressif/ESP32_S3_BOX_3_BETA.h"
#elif defined(BOARD_ESP32_S3_BOX_LITE) || CONFIG_BOARD_ESP32_S3_BOX_LITE
    #include "board/espressif/ESP32_S3_BOX_LITE.h"
#elif defined(BOARD_ESP32_S3_EYE) || CONFIG_BOARD_ESP32_S3_EYE
    #include "board/espressif/ESP32_S3_EYE.h"
#elif defined(BOARD_ESP32_S3_KORVO_2) || CONFIG_BOARD_ESP32_S3_KORVO_2
    #include "board/espressif/ESP32_S3_KORVO_2.h"
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD) || CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD
    #include "board/espressif/ESP32_S3_LCD_EV_BOARD.h"
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_V1_5) || CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_V1_5
    #include "board/espressif/ESP32_S3_LCD_EV_BOARD_V1_5.h"
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_2) || CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_2
    #include "board/espressif/ESP32_S3_LCD_EV_BOARD_2.h"
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5) || CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5
    #include "board/espressif/ESP32_S3_LCD_EV_BOARD_2_V1_5.h"
#elif defined(BOARD_ESP32_S3_USB_OTG) || CONFIG_BOARD_ESP32_S3_USB_OTG
    #include "board/espressif/ESP32_S3_USB_OTG.h"
/* Elecrow */
#elif defined(BOARD_ELECROW_CROWPANEL_7_0) || CONFIG_BOARD_ELECROW_CROWPANEL_7_0
    #include "board/elecrow/CROWPANEL_7_0.h"
/* M5Stack */
#elif defined(BOARD_M5STACK_M5CORE2) || CONFIG_BOARD_M5STACK_M5CORE2
    #include "board/m5stack/M5CORE2.h"
#elif defined(BOARD_M5STACK_M5DIAL) || CONFIG_BOARD_M5STACK_M5DIAL
    #include "board/m5stack/M5DIAL.h"
#elif defined(BOARD_M5STACK_M5CORES3) || CONFIG_BOARD_M5STACK_M5CORES3
    #include "board/m5stack/M5CORES3.h"
/* Jingcai */
#elif defined(BOARD_ESP32_4848S040C_I_Y_3) || CONFIG_BOARD_ESP32_4848S040C_I_Y_3
    #include "board/jingcai/ESP32_4848S040C_I_Y_3.h"
/* Waveshare */
#elif defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_4_3) || CONFIG_BOARD_WAVESHARE_ESP32_S3_Touch_LCD_4_3
    #include "board/waveshare/ESP32_S3_Touch_LCD_4.3.h"
#else
    #error "Unkonw board selected! Please check file `ESP_Panel_Board_Supported.h` and make sure only one board is enabled."
#endif

// *INDENT-OFF*
