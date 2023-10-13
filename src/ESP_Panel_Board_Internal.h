/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

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
/* Jingcai */
#elif defined(BOARD_ESP32_4848S040C_I_Y_3) || CONFIG_BOARD_ESP32_4848S040C_I_Y_3
    #include "board/jingcai/ESP32_4848S040C_I_Y_3.h"
#else
    #error "Unkonw board selected, please refer to the README for supported boards."
#endif
