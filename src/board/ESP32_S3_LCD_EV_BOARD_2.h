/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP32_S3_LCD_EV_BOARD_2_H
#define ESP32_S3_LCD_EV_BOARD_2_H

/* For more information about the board, please visit here: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/index.html */

/*-------------------------------- LCD Related --------------------------------*/
/* Set to 0 if not using LCD */
#define ESP_PANEL_USE_LCD           (1)
/**
 * LCD controller name.
 */
#define ESP_PANEL_LCD_NAME          ST7262

/* LCD resolution in pixels */
#define ESP_PANEL_LCD_H_RES         (800)
#define ESP_PANEL_LCD_V_RES         (480)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_BUS_SKIP_INIT_HOST        (1)
/**
 * LCD bus type. Choose one of the following:
 *      - 0: I2C (not supported yet)
 *      - 1: SPI
 *      - 2: I80 (not supported yet)
 *      - 3: RGB
 */
#define ESP_PANEL_LCD_BUS_TYPE      (3)
/**
 * LCD bus parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html for details.
 */
#define ESP_PANEL_LCD_RGB_CLK_HZ            (16 * 1000 * 1000)
#define ESP_PANEL_LCD_RGB_HPW               (40)
#define ESP_PANEL_LCD_RGB_HBP               (40)
#define ESP_PANEL_LCD_RGB_HFP               (48)
#define ESP_PANEL_LCD_RGB_VPW               (23)
#define ESP_PANEL_LCD_RGB_VBP               (32)
#define ESP_PANEL_LCD_RGB_VFP               (13)
#define ESP_PANEL_LCD_RGB_PCLK_ACTIVE_NEG   (1)
#define ESP_PANEL_LCD_RGB_DATA_WIDTH        (16)
#define ESP_PANEL_LCD_RGB_PIXEL_BITS        (16)
#define ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE   (ESP_PANEL_LCD_H_RES * 10)
#define ESP_PANEL_LCD_RGB_IO_HSYNC          (46)
#define ESP_PANEL_LCD_RGB_IO_VSYNC          (3)
#define ESP_PANEL_LCD_RGB_IO_DE             (17)
#define ESP_PANEL_LCD_RGB_IO_PCLK           (9)
#define ESP_PANEL_LCD_RGB_IO_DISP           (-1)
#define ESP_PANEL_LCD_RGB_IO_DATA0          (10)
#define ESP_PANEL_LCD_RGB_IO_DATA1          (11)
#define ESP_PANEL_LCD_RGB_IO_DATA2          (12)
#define ESP_PANEL_LCD_RGB_IO_DATA3          (13)
#define ESP_PANEL_LCD_RGB_IO_DATA4          (14)
#define ESP_PANEL_LCD_RGB_IO_DATA5          (21)
#define ESP_PANEL_LCD_RGB_IO_DATA6          (47)
#define ESP_PANEL_LCD_RGB_IO_DATA7          (48)
#define ESP_PANEL_LCD_RGB_IO_DATA8          (45)
#define ESP_PANEL_LCD_RGB_IO_DATA9          (38)
#define ESP_PANEL_LCD_RGB_IO_DATA10         (39)
#define ESP_PANEL_LCD_RGB_IO_DATA11         (40)
#define ESP_PANEL_LCD_RGB_IO_DATA12         (41)
#define ESP_PANEL_LCD_RGB_IO_DATA13         (42)
#define ESP_PANEL_LCD_RGB_IO_DATA14         (2)
#define ESP_PANEL_LCD_RGB_IO_DATA15         (1)

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_LCD_COLOR_BITS    (24)
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_LCD_RGB_ORDER     (0)
#define ESP_PANEL_LCD_INEVRT_COLOR  (0)

/* LCD Transformation Flags */
#define ESP_PANEL_LCD_SWAP_XY       (0)
#define ESP_PANEL_LCD_MIRROR_X      (0)
#define ESP_PANEL_LCD_MIRROR_Y      (0)

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_LCD_IO_RST        (-1)

/*-------------------------------- LCD Touch Related --------------------------------*/
/* Set to 0 if not using LCD touch */
#define ESP_PANEL_USE_LCD_TOUCH     (1)
/**
 * LCD Touch IC name.
 */
#define ESP_PANEL_LCD_TOUCH_NAME            GT1151

/* LCD Touch resolution in pixels */
#define ESP_PANEL_LCD_TOUCH_H_RES           (ESP_PANEL_LCD_H_RES)
#define ESP_PANEL_LCD_TOUCH_V_RES           (ESP_PANEL_LCD_V_RES)

/* LCD Touch Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST  (0)
/**
 * LCD touch bus type. Choose one of the following:
 *      - 0: I2C
 *      - 1: SPI
 */
#define ESP_PANEL_LCD_TOUCH_BUS_TYPE            (0)
/**
 * LCD touch bus parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html for details.
 */
#define ESP_PANEL_LCD_TOUCH_BUS_HOST_ID     (0)
#define ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ      (400 * 1000)
#define ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP  (0)
#define ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP  (0)
#define ESP_PANEL_LCD_TOUCH_I2C_IO_SCL      (18)
#define ESP_PANEL_LCD_TOUCH_I2C_IO_SDA      (8)

/* LCD Touch Transformation Flags */
#define ESP_PANEL_LCD_TOUCH_SWAP_XY         (0)
#define ESP_PANEL_LCD_TOUCH_MIRROR_X        (0)
#define ESP_PANEL_LCD_TOUCH_MIRROR_Y        (0)

/* LCD Touch Other Settings */
#define ESP_PANEL_LCD_TOUCH_IO_RST          (-1)
#define ESP_PANEL_LCD_TOUCH_IO_INT          (-1)

/*-------------------------------- Backlight Related --------------------------------*/
#define ESP_PANEL_USE_BL                    (0)

/*-------------------------------- Others --------------------------------*/
/* Assert on error. Otherwise return error code */
#define ESP_PANEL_CHECK_RESULT_ASSERT       (0)
#endif
