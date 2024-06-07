/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/* Set to 1 if using a supported board */
#define ESP_PANEL_USE_SUPPORTED_BOARD       (0)         // 0/1

#if ESP_PANEL_USE_SUPPORTED_BOARD
/**
 * Uncomment one of the following macros to select an supported development board. If multiple macros are uncommented
 * at the same time, an error will be prompted during compilation.
 *
 */

/*
 * Espressif Supported Boards (https://www.espressif.com/en/products/devkits):
 *
 *  - ESP32-C3-LCDkit: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32c3/esp32-c3-lcdkit/index.html
 *  - ESP32-S3-Box: https://github.com/espressif/esp-box/tree/master
 *  - ESP32-S3-Box-3: https://github.com/espressif/esp-box/tree/master
 *  - ESP32-S3-Box-3(beta): https://github.com/espressif/esp-box/tree/c4c954888e11250423f083df0067d99e22d59fbe
 *  - ESP32-S3-Box-Lite: https://github.com/espressif/esp-box/tree/master
 *  - ESP32-S3-EYE: https://github.com/espressif/esp-who/blob/master/docs/en/get-started/ESP32-S3-EYE_Getting_Started_Guide.md
 *  - ESP32-S3-Korvo-2: https://docs.espressif.com/projects/esp-adf/en/latest/design-guide/dev-boards/user-guide-esp32-s3-korvo-2.html
 *  - ESP32-S3-LCD-EV-Board: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/user_guide_v1.4.html
 *  - ESP32-S3-LCD-EV-Board(v1.5): https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/user_guide.html
 *  - ESP32-S3-LCD-EV-Board-2: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/user_guide_v1.4.html
 *  - ESP32-S3-LCD-EV-Board-2(v1.5): https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/user_guide.html
 *  - ESP32-S3-USB-OTG: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-usb-otg/index.html
 *
 */
// #define BOARD_ESP32_C3_LCDKIT
// #define BOARD_ESP32_S3_BOX
// #define BOARD_ESP32_S3_BOX_3
// #define BOARD_ESP32_S3_BOX_3_BETA
// #define BOARD_ESP32_S3_BOX_LITE
// #define BOARD_ESP32_S3_EYE
// #define BOARD_ESP32_S3_KORVO_2
// #define BOARD_ESP32_S3_LCD_EV_BOARD
// #define BOARD_ESP32_S3_LCD_EV_BOARD_V1_5
// #define BOARD_ESP32_S3_LCD_EV_BOARD_2
// #define BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5
// #define BOARD_ESP32_S3_USB_OTG

/*
 * M5Stack (https://m5stack.com/):
 *
 *  - M5STACK_M5CORE2: https://docs.m5stack.com/zh_CN/core/core2
 *  - M5STACK_M5DIAL: https://docs.m5stack.com/zh_CN/core/M5Dial
 *  - M5STACK_M5CORES3: https://docs.m5stack.com/zh_CN/core/CoreS3
 */
// #define BOARD_M5STACK_M5CORE2
// #define BOARD_M5STACK_M5DIAL
// #define BOARD_M5STACK_M5CORES3

/*
 * Shenzhen Jingcai Intelligent Supported Boards (https://www.displaysmodule.com/):
 *
 *  - ESP32-4848S040C_I_Y_3:
 *      - https://www.displaysmodule.com/sale-41828962-experience-the-power-of-the-esp32-display-module-sku-esp32-4848s040c-i-y-3.html
 *      - http://pan.jczn1688.com/directlink/1/ESP32%20module/4.0inch_ESP32-4848S040.zip
 *
 */
// #define BOARD_ESP32_4848S040C_I_Y_3

/**
 * Do not change the following versions, they are used to check if the configurations in this file are compatible with
 * the current version of `ESP_Panel_Board_Supported.h` in the library. The detailed rules are as follows:
 *
 *   1. If the major version is not consistent, then the configurations in this file are incompatible with the library
 *      and must be replaced with the file from the library.
 *   2. If the minor version is not consistent, this file might be missing some new configurations, which will be set to
 *      default values. It is recommended to replace it with the file from the library.
 *   3. If the patch version is not consistent, it will not affect normal functionality.
 *
 */
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR 0
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR 2
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH 0

#endif
