/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP32_S3_EYE_H
#define ESP32_S3_EYE_H

/* For more information about the board, please visit here: https://github.com/espressif/esp-who/blob/master/docs/en/get-started/ESP32-S3-EYE_Getting_Started_Guide.md */

/*-------------------------------- LCD Related --------------------------------*/
/* Set to 0 if not using LCD */
#define ESP_PANEL_USE_LCD           (1)
/**
 * LCD controller name.
 */
#define ESP_PANEL_LCD_NAME          ST7789

/* LCD resolution in pixels */
#define ESP_PANEL_LCD_H_RES         (240)
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
#define ESP_PANEL_LCD_SPI_IO_CS             (44)
#define ESP_PANEL_LCD_SPI_IO_DC             (43)
#define ESP_PANEL_LCD_SPI_MODE              (0)
#define ESP_PANEL_LCD_SPI_CLK_HZ            (40 * 1000 * 1000)
#define ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ    (10)
#define ESP_PANEL_LCD_SPI_CMD_BITS          (8)
#define ESP_PANEL_LCD_SPI_PARAM_BITS        (8)
#define ESP_PANEL_LCD_SPI_IO_SCK            (21)
#define ESP_PANEL_LCD_SPI_IO_MOSI           (47)
#define ESP_PANEL_LCD_SPI_IO_MISO           (-1)

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_LCD_COLOR_BITS    (16)
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_LCD_RGB_ORDER     (0)
#define ESP_PANEL_LCD_INEVRT_COLOR  (1)

/* LCD Transformation Flags */
#define ESP_PANEL_LCD_SWAP_XY       (0)
#define ESP_PANEL_LCD_MIRROR_X      (0)
#define ESP_PANEL_LCD_MIRROR_Y      (0)

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_LCD_IO_RST        (-1)

/*-------------------------------- LCD Touch Related --------------------------------*/
/* Set to 0 if not using LCD touch */
#define ESP_PANEL_USE_LCD_TOUCH     (0)

/*-------------------------------- Backlight Related --------------------------------*/
#define ESP_PANEL_USE_BL                    (1)

/* IO num of backlight pin */
#define ESP_PANEL_LCD_IO_BL                 (48)

/* If the backlight is on when high level, set to 1; otherwise to 0 */
#define ESP_PANEL_LCD_BL_ON_LEVEL           (0)

/* Set to 1 if use PWM for backlight brightness control. */
#define ESP_PANEL_LCD_BL_USE_PWM            (1)

/**
 *  Backlight LEDC Parameters.
 *
 *  Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/ledc.html for details.
 */
#define ESP_PANEL_LCD_BL_PWM_TIMER          (0)
#define ESP_PANEL_LCD_BL_PWM_CHANNEL        (0)
#define ESP_PANEL_LCD_BL_PWM_RESOLUTION     (10)
#define ESP_PANEL_LCD_BL_PWM_FREQ_HZ        (5000)

/*-------------------------------- Others --------------------------------*/
/* Assert on error. Otherwise return error code */
#define ESP_PANEL_CHECK_RESULT_ASSERT       (0)
#endif
