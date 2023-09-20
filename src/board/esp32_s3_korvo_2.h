/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP32_S3_KORVO_2_H
#define ESP32_S3_KORVO_2_H

/* For more information about it, please visit here: https://docs.espressif.com/projects/esp-adf/en/latest/design-guide/dev-boards/user-guide-esp32-s3-korvo-2.html */

/*-------------------------------- LCD Related --------------------------------*/
/* Set to 0 if not using LCD */
#define ESP_PANEL_USE_LCD           (1)
/**
 * LCD controller name.
 */
#define ESP_PANEL_LCD_NAME          ST7789

/* LCD resolution in pixels */
#define ESP_PANEL_LCD_H_RES         (320)
#define ESP_PANEL_LCD_V_RES         (240)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_BUS_SKIP_INIT_HOST        (0)
/**
 * LCD bus type. Choose one of the following:
 *      - 0: I2C (not supported yet)
 *      - 1: SPI
 *      - 2: I80 (not supported yet)
 *      - 3: RGB
 */
#define ESP_PANEL_LCD_BUS_TYPE      (1)
/**
 * LCD bus parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html for details.
 */
#define ESP_PANEL_LCD_BUS_HOST_ID           (1)
#define ESP_PANEL_LCD_SPI_IO_CS             (-1)
#define ESP_PANEL_LCD_SPI_IO_DC             (2)
#define ESP_PANEL_LCD_SPI_MODE              (0)
#define ESP_PANEL_LCD_SPI_CLK_HZ            (40 * 1000 * 1000)
#define ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ    (10)
#define ESP_PANEL_LCD_SPI_CMD_BITS          (8)
#define ESP_PANEL_LCD_SPI_PARAM_BITS        (8)
#define ESP_PANEL_LCD_SPI_IO_SCK            (1)
#define ESP_PANEL_LCD_SPI_IO_MOSI           (0)
#define ESP_PANEL_LCD_SPI_IO_MISO           (-1)

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_LCD_COLOR_BITS    (16)
/*
 * LCD Color Space. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_LCD_COLOR_SPACE   (1)
#define ESP_PANEL_LCD_INEVRT_COLOR  (0)

/* LCD Transformation Flags */
#define ESP_PANEL_LCD_SWAP_XY       (0)
#define ESP_PANEL_LCD_MIRROR_X      (1)
#define ESP_PANEL_LCD_MIRROR_Y      (1)

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_LCD_IO_RST        (-1)

/*-------------------------------- LCD Touch Related --------------------------------*/
/* Set to 0 if not using LCD touch */
#define ESP_PANEL_USE_LCD_TOUCH     (1)
/**
 * LCD Touch IC name.
 */
#define ESP_PANEL_LCD_TOUCH_NAME            TT21100

/* LCD Touch resolution in pixels */
#define ESP_PANEL_LCD_TOUCH_H_RES           (ESP_PANEL_LCD_H_RES)
#define ESP_PANEL_LCD_TOUCH_V_RES           (ESP_PANEL_LCD_V_RES)

/* LCD Touch Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST  (1)
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
#define ESP_PANEL_LCD_TOUCH_I2C_IO_SDA      (17)

/* LCD Touch Transformation Flags */
#define ESP_PANEL_LCD_TOUCH_SWAP_XY         (0)
#define ESP_PANEL_LCD_TOUCH_MIRROR_X        (1)
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
