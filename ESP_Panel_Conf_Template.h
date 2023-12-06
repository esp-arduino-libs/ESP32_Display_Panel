/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*
/* Set to 1 if assert on error. Otherwise print error message and jump */
#define ESP_PANEL_CHECK_RESULT_ASSERT       (0)         // 0/1

/* Set to 1 if print debug message */
#define ESP_PANEL_ENABLE_DEBUG_LOG          (0)         // 0/1

/* Set to 0 if use a custom board */
#define ESP_PANEL_USE_SUPPORTED_BOARD       (1)         // 0/1

#if ESP_PANEL_USE_SUPPORTED_BOARD
/*
 * Supported Boards:
 *  - ESP32-C3-LCDkit: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32c3/esp32-c3-lcdkit/index.html
 *  - ESP32-S3-Box: https://github.com/espressif/esp-box/tree/master
 *  - ESP32-S3-Box-3: https://github.com/espressif/esp-box/tree/master
 *  - ESP32-S3-Box-3(beta): https://github.com/espressif/esp-box/tree/c4c954888e11250423f083df0067d99e22d59fbe
 *  - ESP32-S3-Box-Lite: https://github.com/espressif/esp-box/tree/master
 *  - ESP32-S3-EYE: https://github.com/espressif/esp-who/blob/master/docs/en/get-started/ESP32-S3-EYE_Getting_Started_Guide.md
 *  - ESP32-S3-Korvo-2: https://docs.espressif.com/projects/esp-adf/en/latest/design-guide/dev-boards/user-guide-esp32-s3-korvo-2.html
 *  - ESP32-S3-LCD-EV-Board: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/index.html
 *  - ESP32-S3-LCD-EV-Board-2: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/index.html
 *  - ESP32-S3-USB-OTG: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-usb-otg/index.html
 *
 * Uncomment one of the following macros to select an supported development board.
 *
 */
// #define ESP_PANEL_BOARD_ESP32_C3_LCDKIT
// #define ESP_PANEL_BOARD_ESP32_S3_BOX
// #define ESP_PANEL_BOARD_ESP32_S3_BOX_3
// #define ESP_PANEL_BOARD_ESP32_S3_BOX_3_BETA
// #define ESP_PANEL_BOARD_ESP32_S3_BOX_LITE
// #define ESP_PANEL_BOARD_ESP32_S3_EYE
// #define ESP_PANEL_BOARD_ESP32_S3_KORVO_2
// #define ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD
// #define ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD_2
// #define ESP_PANEL_BOARD_ESP32_S3_USB_OTG

#else
/*===================================== LCD Related =====================================*/
/* Set to 0 if not using LCD */
#define ESP_PANEL_USE_LCD           (0)     // 0/1
#if ESP_PANEL_USE_LCD
/**
 * LCD IC name. Choose one of the following:
 *      - ILI9341
 *      - GC9503, GC9A01
 *      - ST7262, ST7789, ST7796
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
#define ESP_PANEL_LCD_BUS_SKIP_INIT_HOST        (0)     // 0/1
/**
 * LCD bus type. Choose one of the following:
 *      - 0: I2C (not supported yet)
 *      - 1: SPI
 *      - 2: RGB (only supported for ESP32-S3)
 *      - 3: I80 (not supported yet)
 *      - 4: QSPI (not supported yet)
 */
#define ESP_PANEL_LCD_BUS_TYPE      (1)
/**
 * LCD bus parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html
 * for details.
 *
 */
#if ESP_PANEL_LCD_BUS_TYPE == 0         // I2C related parameters

#error "This function is not implemented and will be implemented in the future."

#elif ESP_PANEL_LCD_BUS_TYPE == 1       // SPI panel IO related parameters
    #define ESP_PANEL_LCD_BUS_HOST_ID           (1)
    #define ESP_PANEL_LCD_SPI_IO_CS             (5)
    #define ESP_PANEL_LCD_SPI_IO_DC             (4)
    #define ESP_PANEL_LCD_SPI_MODE              (0)     // 0/1/2/3
    #define ESP_PANEL_LCD_SPI_CLK_HZ            (40 * 1000 * 1000)
    #define ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ    (10)
    #define ESP_PANEL_LCD_SPI_CMD_BITS          (8)     // 8/16/32
    #define ESP_PANEL_LCD_SPI_PARAM_BITS        (8)     // 8/16/32
#if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST   // SPI host related parameters
    #define ESP_PANEL_LCD_SPI_IO_SCK            (7)
    #define ESP_PANEL_LCD_SPI_IO_MOSI           (6)
    #define ESP_PANEL_LCD_SPI_IO_MISO           (-1)
#endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */

#elif ESP_PANEL_LCD_BUS_TYPE == 2       // I80 related parameters

#error "This function is not implemented and will be implemented in the future."

#elif ESP_PANEL_LCD_BUS_TYPE == 3       // RGB related parameters
    #define ESP_PANEL_LCD_RGB_CLK_HZ            (16 * 1000 * 1000)
    #define ESP_PANEL_LCD_RGB_HPW               (10)
    #define ESP_PANEL_LCD_RGB_HBP               (10)
    #define ESP_PANEL_LCD_RGB_HFP               (20)
    #define ESP_PANEL_LCD_RGB_VPW               (10)
    #define ESP_PANEL_LCD_RGB_VBP               (10)
    #define ESP_PANEL_LCD_RGB_VFP               (10)
    #define ESP_PANEL_LCD_RGB_PCLK_ACTIVE_NEG   (0)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_LCD_RGB_DATA_WIDTH        (16)    //  8 | 16
    #define ESP_PANEL_LCD_RGB_PIXEL_BITS        (16)    // 24 | 16
    #define ESP_PANEL_LCD_RGB_FRAME_BUF_NUM     (1)     // 1/2/3
    #define ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE   (ESP_PANEL_LCD_H_RES * 10)  // Bounce buffer size in bytes. Set to 0 if disable bounce buffer.
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
#if ESP_PANEL_LCD_RGB_DATA_WIDTH > 8
    #define ESP_PANEL_LCD_RGB_IO_DATA8          (45)
    #define ESP_PANEL_LCD_RGB_IO_DATA9          (38)
    #define ESP_PANEL_LCD_RGB_IO_DATA10         (39)
    #define ESP_PANEL_LCD_RGB_IO_DATA11         (40)
    #define ESP_PANEL_LCD_RGB_IO_DATA12         (41)
    #define ESP_PANEL_LCD_RGB_IO_DATA13         (42)
    #define ESP_PANEL_LCD_RGB_IO_DATA14         (2)
    #define ESP_PANEL_LCD_RGB_IO_DATA15         (1)
#endif /* ESP_PANEL_LCD_RGB_DATA_WIDTH */

#if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST       // 3-wire SPI panel IO related parameters
    #define ESP_PANEL_LCD_3WIRE_SPI_SCL_ACTIVE_EDGE     (0)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_LCD_3WIRE_SPI_AUTO_DEL_PANEL_IO   (0)     // Delete the panel IO instance automatically if set to 1.
                                                                // If the 3-wire SPI pins are sharing other pins of the RGB interface to save GPIOs,
                                                                // Please set it to 1 to release the panel IO and its pins (except CS signal).
    #define ESP_PANEL_LCD_3WIRE_SPI_CS_USE_EXPNADER     (0)     // 0/1
    #define ESP_PANEL_LCD_3WIRE_SPI_SCL_USE_EXPNADER    (0)     // 0/1
    #define ESP_PANEL_LCD_3WIRE_SPI_SDA_USE_EXPNADER    (0)     // 0/1
    #define ESP_PANEL_LCD_3WIRE_SPI_IO_CS               (0)
    #define ESP_PANEL_LCD_3WIRE_SPI_IO_SCL              (1)
    #define ESP_PANEL_LCD_3WIRE_SPI_IO_SDA              (2)
#endif /* ESP_PANEL_LCD_BUS_SKIP_INIT_HOST */

#endif /* ESP_PANEL_LCD_BUS_TYPE */

/**
 * LCD initialization commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment the following macro definitions and change them in the same format
 * if needed. Otherwise, the LCD driver will use the default initialization sequence code.
 *
 */
// #define ESP_PANEL_LCD_INIT_CMD_SIZE          (3)     // Number of commands
// //      {cmd, { data }, data_size, delay_ms}
// #define ESP_PANEL_LCD_INIT_CMD                                      \
//     {                                                               \
//         {0xf0, (uint8_t []){0x55, 0xaa, 0x52, 0x08, 0x00}, 5, 0},   \
//         {0xf6, (uint8_t []){0x5a, 0x87}, 2, 0},                     \
//         {0xc1, (uint8_t []){0x3f}, 1, 0},                           \
//     }

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_LCD_COLOR_BITS    (16)        // 8/16/18/24
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_LCD_RGB_ORDER     (0)
#define ESP_PANEL_LCD_INEVRT_COLOR  (0)         // 0/1

/* LCD Transformation Flags */
#define ESP_PANEL_LCD_SWAP_XY       (0)         // 0/1
#define ESP_PANEL_LCD_MIRROR_X      (0)         // 0/1
#define ESP_PANEL_LCD_MIRROR_Y      (0)         // 0/1

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_LCD_IO_RST        (-1)
#define ESP_PANEL_LCD_RST_LEVEL     (0)         // 0/1

#endif /* ESP_PANEL_USE_LCD */

/*===================================== LCD Touch Related =====================================*/
/* Set to 0 if not using LCD touch */
#define ESP_PANEL_USE_LCD_TOUCH     (0)         // 0/1
#if ESP_PANEL_USE_LCD_TOUCH
/**
 * LCD Touch IC name. Choose one of the following:
 *      - CST816S
 *      - FT5x06
 *      - GT1151, GT911
 *      - TT21100
 *      - STMPE610
 */
#define ESP_PANEL_LCD_TOUCH_NAME    TT21100

/* LCD Touch resolution in pixels */
#define ESP_PANEL_LCD_TOUCH_H_RES   (ESP_PANEL_LCD_H_RES)
#define ESP_PANEL_LCD_TOUCH_V_RES   (ESP_PANEL_LCD_V_RES)

/* LCD Touch Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST  (0)     // 0/1
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
#if ESP_PANEL_LCD_TOUCH_BUS_TYPE == 0           // I2C related parameters
    #define ESP_PANEL_LCD_TOUCH_BUS_HOST_ID     (0)
#if !ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST     // I2C host
    #define ESP_PANEL_LCD_TOUCH_I2C_CLK_HZ      (400 * 1000)
    #define ESP_PANEL_LCD_TOUCH_I2C_SCL_PULLUP  (0)     // 0/1
    #define ESP_PANEL_LCD_TOUCH_I2C_SDA_PULLUP  (0)     // 0/1
    #define ESP_PANEL_LCD_TOUCH_I2C_IO_SCL      (18)
    #define ESP_PANEL_LCD_TOUCH_I2C_IO_SDA      (8)
#endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */
#elif ESP_PANEL_LCD_TOUCH_BUS_TYPE == 1         // SPI related parameters
    #define ESP_PANEL_LCD_TOUCH_BUS_HOST_ID     (1)
    #define ESP_PANEL_LCD_TOUCH_SPI_IO_CS       (5)
#if !ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST     // SPI host
    #define ESP_PANEL_LCD_TOUCH_SPI_IO_SCK      (7)
    #define ESP_PANEL_LCD_TOUCH_SPI_IO_MOSI     (6)
    #define ESP_PANEL_LCD_TOUCH_SPI_IO_MISO     (4)
#endif /* ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST */
#endif /* ESP_PANEL_LCD_TOUCH_BUS_TYPE */

/* LCD Touch Transformation Flags */
#define ESP_PANEL_LCD_TOUCH_SWAP_XY         (0)         // 0/1
#define ESP_PANEL_LCD_TOUCH_MIRROR_X        (0)         // 0/1
#define ESP_PANEL_LCD_TOUCH_MIRROR_Y        (0)         // 0/1

/* LCD Touch Other Settings */
#define ESP_PANEL_LCD_TOUCH_IO_RST          (-1)
#define ESP_PANEL_LCD_TOUCH_IO_INT          (-1)
#define ESP_PANEL_LCD_TOUCH_RST_LEVEL       (0)         // 0/1
#define ESP_PANEL_LCD_TOUCH_INT_LEVEL       (0)         // 0/1

#endif /* ESP_PANEL_USE_LCD_TOUCH */

/*===================================== Backlight Related =====================================*/
#define ESP_PANEL_USE_BL                    (0)         // 0/1
#if ESP_PANEL_USE_BL
/* IO num of backlight pin */
#define ESP_PANEL_LCD_IO_BL                 (45)

/* If the backlight is on when high level, set to 1; otherwise to 0 */
#define ESP_PANEL_LCD_BL_ON_LEVEL           (1)         // 0/1

/* Set to 1 if use PWM for backlight brightness control. */
#define ESP_PANEL_LCD_BL_USE_PWM            (0)         // 0/1
#if ESP_PANEL_LCD_BL_USE_PWM
/**
 *  Backlight LEDC Parameters.
 *
 *  Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/ledc.html for details.
 */
    #define ESP_PANEL_LCD_BL_PWM_TIMER      (0)
    #define ESP_PANEL_LCD_BL_PWM_CHANNEL    (0)
    #define ESP_PANEL_LCD_BL_PWM_RESOLUTION (10)
    #define ESP_PANEL_LCD_BL_PWM_FREQ_HZ    (5000)
#endif /* ESP_PANEL_LCD_BL_USE_PWM */
#endif /* ESP_PANEL_USE_BL */

/*===================================== IO Expander Related =====================================*/
/* Set to 0 if not using IO Expander */
#define ESP_PANEL_USE_EXPANDER              (0)         // 0/1
#if ESP_PANEL_USE_EXPANDER
/**
 * IO expander IC name. Choose one of the following:
 *      - HT8574
 *      - TCA95xx_8bit
 *      - TCA95xx_16bit
 */
#define ESP_PANEL_EXPANDER_NAME                 TCA95xx_8bit

/* IO expander & host Settings */
// Device parameters
#define ESP_PANEL_EXPANDER_ADDRESS              (0x20)

//  If set to 1, the driver will skip to initialize the corresponding host. Users need to initialize the host in advance.
#define ESP_PANEL_EXPANDER_SKIP_INIT_HOST       (0)     // 0/1
// Host parameters
#if !ESP_PANEL_EXPANDER_SKIP_INIT_HOST
    #define ESP_PANEL_EXPANDER_HOST_ID          (0)
    #define ESP_PANEL_EXPANDER_I2C_CLK_HZ       (400 * 1000)
    #define ESP_PANEL_EXPANDER_I2C_SCL_PULLUP   (0)     // 0/1
    #define ESP_PANEL_EXPANDER_I2C_SDA_PULLUP   (0)     // 0/1
    #define ESP_PANEL_EXPANDER_I2C_IO_SCL       (18)
    #define ESP_PANEL_EXPANDER_I2C_IO_SDA       (8)
#endif
#endif /* ESP_PANEL_USE_EXPANDER */

#endif /* ESP_PANEL_USE_SUPPORTED_BOARD */
// *INDENT-OFF*

#endif
