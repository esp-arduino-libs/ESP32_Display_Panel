/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the LCD panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an LCD panel */
#define ESP_PANEL_USE_LCD           (1)     // 0/1

#if ESP_PANEL_USE_LCD
/**
 * LCD Controller Name.
 */
#define ESP_PANEL_LCD_NAME          ST7262

/* LCD resolution in pixels */
#define ESP_PANEL_LCD_WIDTH         (800)
#define ESP_PANEL_LCD_HEIGHT        (480)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_BUS_SKIP_INIT_HOST    (1)     // 0/1
/**
 * LCD Bus Type.
 */
#define ESP_PANEL_LCD_BUS_TYPE      (ESP_PANEL_BUS_TYPE_RGB)
/**
 * LCD Bus Parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html and
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html for more details.
 *
 */
#if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB

    #define ESP_PANEL_LCD_RGB_CLK_HZ            (16 * 1000 * 1000)
    #define ESP_PANEL_LCD_RGB_HPW               (40)
    #define ESP_PANEL_LCD_RGB_HBP               (40)
    #define ESP_PANEL_LCD_RGB_HFP               (40)
    #define ESP_PANEL_LCD_RGB_VPW               (23)
    #define ESP_PANEL_LCD_RGB_VBP               (32)
    #define ESP_PANEL_LCD_RGB_VFP               (13)
    #define ESP_PANEL_LCD_RGB_PCLK_ACTIVE_NEG   (1)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_LCD_RGB_DATA_WIDTH        (16)    //  8 | 16
    #define ESP_PANEL_LCD_RGB_PIXEL_BITS        (16)    // 24 | 16
    #define ESP_PANEL_LCD_RGB_FRAME_BUF_NUM     (1)     // 1/2/3
    #define ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE   (ESP_PANEL_LCD_WIDTH * 10)
                                                        // Bounce buffer size in bytes. This function is used to avoid screen drift.
                                                        // To enable the bounce buffer, set it to a non-zero value.
                                                        // Typically set to `ESP_PANEL_LCD_WIDTH * 10`
    #define ESP_PANEL_LCD_RGB_IO_HSYNC          (46)
    #define ESP_PANEL_LCD_RGB_IO_VSYNC          (3)
    #define ESP_PANEL_LCD_RGB_IO_DE             (17)    // -1 if not used
    #define ESP_PANEL_LCD_RGB_IO_PCLK           (9)
    #define ESP_PANEL_LCD_RGB_IO_DISP           (-1)    // -1 if not used
    #define ESP_PANEL_LCD_RGB_IO_DATA0          (10)
    #define ESP_PANEL_LCD_RGB_IO_DATA1          (11)
    #define ESP_PANEL_LCD_RGB_IO_DATA2          (12)
    #define ESP_PANEL_LCD_RGB_IO_DATA3          (13)
    #define ESP_PANEL_LCD_RGB_IO_DATA4          (14)
    #define ESP_PANEL_LCD_RGB_IO_DATA5          (21)
    #define ESP_PANEL_LCD_RGB_IO_DATA6          (8)
    #define ESP_PANEL_LCD_RGB_IO_DATA7          (18)
#if ESP_PANEL_LCD_RGB_DATA_WIDTH > 8
    #define ESP_PANEL_LCD_RGB_IO_DATA8          (45)
    #define ESP_PANEL_LCD_RGB_IO_DATA9          (38)
    #define ESP_PANEL_LCD_RGB_IO_DATA10         (39)
    #define ESP_PANEL_LCD_RGB_IO_DATA11         (40)
    #define ESP_PANEL_LCD_RGB_IO_DATA12         (41)
    #define ESP_PANEL_LCD_RGB_IO_DATA13         (42)
    #define ESP_PANEL_LCD_RGB_IO_DATA14         (2)
    #define ESP_PANEL_LCD_RGB_IO_DATA15         (1)
#endif

#endif /* ESP_PANEL_LCD_BUS_TYPE */

/**
 * LCD Venbdor Initialization Commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formater: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
// #define ESP_PANEL_LCD_VENDOR_INIT_CMD                                          \
//     {                                                                          \
//         {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},              \
//         {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},                                \
//         {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},                                \
//         {0x29, (uint8_t []){0x00}, 0, 120},                                    \
//         or                                                                     \
//         ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}), \
//         ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),                   \
//         ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),                   \
//         ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),                               \
//     }

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_LCD_COLOR_BITS    (24)        // 8/16/18/24
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_LCD_BGR_ORDER     (0)         // 0/1
#define ESP_PANEL_LCD_INEVRT_COLOR  (0)         // 0/1

/* LCD Transformation Flags */
#define ESP_PANEL_LCD_SWAP_XY       (0)         // 0/1
#define ESP_PANEL_LCD_MIRROR_X      (0)         // 0/1
#define ESP_PANEL_LCD_MIRROR_Y      (0)         // 0/1

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_LCD_IO_RST        (-1)
#define ESP_PANEL_LCD_RST_LEVEL     (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_USE_LCD */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an touch panel */
#define ESP_PANEL_USE_TOUCH         (1)         // 0/1
#if ESP_PANEL_USE_TOUCH
/**
 * Touch controller name.
 */
#define ESP_PANEL_TOUCH_NAME        GT1151

/* Touch resolution in pixels */
#define ESP_PANEL_TOUCH_H_RES       (ESP_PANEL_LCD_WIDTH)   // Typically set to the same value as the width of LCD
#define ESP_PANEL_TOUCH_V_RES       (ESP_PANEL_LCD_HEIGHT)  // Typically set to the same value as the height of LCD

/* Touch Panel Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_TOUCH_BUS_SKIP_INIT_HOST      (0)     // 0/1
/**
 * Touch panel bus type.
 */
#define ESP_PANEL_TOUCH_BUS_TYPE            (ESP_PANEL_BUS_TYPE_I2C)
/* Touch panel bus parameters */
#if ESP_PANEL_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    #define ESP_PANEL_TOUCH_BUS_HOST_ID     (0)     // Typically set to 0
    #define ESP_PANEL_TOUCH_I2C_ADDRESS     (0)     // Typically set to 0 to use default address
#if !ESP_PANEL_TOUCH_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_TOUCH_I2C_CLK_HZ      (400 * 1000)
                                                    // Typically set to 400K
    #define ESP_PANEL_TOUCH_I2C_SCL_PULLUP  (0)     // 0/1
    #define ESP_PANEL_TOUCH_I2C_SDA_PULLUP  (0)     // 0/1
    #define ESP_PANEL_TOUCH_I2C_IO_SCL      (48)
    #define ESP_PANEL_TOUCH_I2C_IO_SDA      (47)
#endif

#endif

/* Touch Transformation Flags */
#define ESP_PANEL_TOUCH_SWAP_XY         (0)         // 0/1
#define ESP_PANEL_TOUCH_MIRROR_X        (0)         // 0/1
#define ESP_PANEL_TOUCH_MIRROR_Y        (0)         // 0/1

/* Touch Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_TOUCH_IO_RST          (-1)
#define ESP_PANEL_TOUCH_RST_LEVEL       (0)         // 0: low level, 1: high level
/* IO num of INT pin, set to -1 if not use */
#define ESP_PANEL_TOUCH_IO_INT          (-1)
#define ESP_PANEL_TOUCH_INT_LEVEL       (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_USE_TOUCH */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ESP_PANEL_USE_BACKLIGHT         (0)         // 0/1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using IO Expander */
#define ESP_PANEL_USE_EXPANDER          (0)         // 0/1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please utilize the following macros to execute any additional code if required. //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define ESP_PANEL_BEGIN_START_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_EXPANDER_START_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_EXPANDER_END_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_LCD_START_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_LCD_END_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_TOUCH_START_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_TOUCH_END_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_BACKLIGHT_START_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_BACKLIGHT_END_FUNCTION( panel )
// #define ESP_PANEL_BEGIN_END_FUNCTION( panel )

// *INDENT-OFF*
