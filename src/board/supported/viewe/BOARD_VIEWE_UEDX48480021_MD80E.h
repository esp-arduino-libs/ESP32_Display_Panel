/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * @file   BOARD_UEDX48480040E_WB_A.h
 * @brief  Configuration file for Viewe UEDX48480040E-WB-A
 * @author Viewe@VIEWESMART
 * @link   https://viewedisplay.com/product/esp32-4-inch-tft-display-touch-screen-arduino-lvgl/
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure general panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Board name
 */
#define ESP_PANEL_BOARD_NAME                "Viewe:UEDX48480021-MD80E"

/**
 * @brief Panel resolution configuration in pixels
 */
#define ESP_PANEL_BOARD_WIDTH               (480)   // Panel width (horizontal, in pixels)
#define ESP_PANEL_BOARD_HEIGHT              (480)   // Panel height (vertical, in pixels)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the LCD panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief LCD panel configuration flag (0/1)
 *
 * Set to `1` to enable LCD panel support, `0` to disable
 */
#define ESP_PANEL_BOARD_USE_LCD             (1)

#if ESP_PANEL_BOARD_USE_LCD
/**
 * @brief LCD controller selection
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER      GC9503

/**
 * @brief LCD bus type selection
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE        (ESP_PANEL_BUS_TYPE_RGB)

/**
 * @brief LCD bus parameters configuration
 *
 * Configure parameters based on the selected bus type. Parameters for other bus types will be ignored.
 * For detailed parameter explanations, see:
 * https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32s3/api-reference/peripherals/lcd/index.html
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html
 */
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB

    /**
     * @brief RGB bus
     */
    /**
     * Set to 0 if using simple "RGB" interface which does not contain "3-wire SPI" interface.
     */
    #define ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL       (1) // 0/1. Typically set to 1

#if ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL
    /* For control panel (3wire-SPI) */
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_CS               (18)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SCK              (13)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SDA              (12)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_CS_USE_EXPNADER     (0) // Set to 1 if the signal is controlled by an IO expander
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_SCL_USE_EXPNADER    (0) // Set to 1 if the signal is controlled by an IO expander
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_SDA_USE_EXPNADER    (0) // Set to 1 if the signal is controlled by an IO expander
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_MODE                (0) // 0-3, typically set to 0
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_CMD_BYTES           (1) // Typically set to 8
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_PARAM_BYTES         (1) // Typically set to 8
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_USE_DC_BIT          (1) // 0/1. Typically set to 1
#endif // ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL
    /* For refresh panel (RGB) */
    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ          (16 * 1000 * 1000)
                                                            // To increase the upper limit of the PCLK, see: https://docs.espressif.com/projects/esp-faq/en/latest/software-framework/peripherals/lcd.html#how-can-i-increase-the-upper-limit-of-pclk-settings-on-esp32-s3-while-ensuring-normal-rgb-screen-display
    #define ESP_PANEL_BOARD_LCD_RGB_HPW             (8)
    #define ESP_PANEL_BOARD_LCD_RGB_HBP             (20)
    #define ESP_PANEL_BOARD_LCD_RGB_HFP             (40)
    #define ESP_PANEL_BOARD_LCD_RGB_VPW             (8)
    #define ESP_PANEL_BOARD_LCD_RGB_VBP             (20)
    #define ESP_PANEL_BOARD_LCD_RGB_VFP             (50)
    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG (0)     // 0: rising edge, 1: falling edge. Typically set to 0
                                                                                        // The following sheet shows the valid combinations of
                                                                                        // data width and pixel bits:
                                                                                        // ┏---------------------------------┳- -------------------------------┓
    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH      (16)                                // |                16               |               8                 |
    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS      (ESP_PANEL_LCD_COLOR_BITS_RGB565)   // | ESP_PANEL_LCD_COLOR_BITS_RGB565 | ESP_PANEL_LCD_COLOR_BITS_RGB888 |
                                                                                        // ┗---------------------------------┻---------------------------------┛
                                                            // To understand color format of RGB LCD, see: https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/rgb_lcd.html#color-formats
    #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE (ESP_PANEL_BOARD_WIDTH * 10)
                                                            // Bounce buffer size in bytes. It is used to avoid screen drift
                                                            // for ESP32-S3. Typically set to `ESP_PANEL_BOARD_WIDTH * 10`
                                                            // The size should satisfy `size * N = LCD_width * LCD_height`,
                                                            // where N is an even number.
                                                            // For more details, see: https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/FAQ.md#how-to-fix-screen-drift-issue-when-driving-rgb-lcd-with-esp32-s3
    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC        (46)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC        (3)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE           (17)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK         (9)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP         (-1)    // -1 if not used. Typically set to -1

                                                            // The following sheet shows the mapping of ESP GPIOs to
                                                            // LCD data pins with different data width and color format:
                                                            // ┏------┳- ------------┳--------------------------┓
                                                            // | ESP: | 8-bit RGB888 |      16-bit RGB565       |
                                                            // |------|--------------|--------------------------|
                                                            // | LCD: |    RGB888    | RGB565 | RGB666 | RGB888 |
                                                            // ┗------|--------------|--------|--------|--------|
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0        (10)    //        |      D0      |   B0   |  B0-1  |   B0-3 |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1        (11)    //        |      D1      |   B1   |  B2    |   B4   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2        (12)    //        |      D2      |   B2   |  B3    |   B5   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3        (13)    //        |      D3      |   B3   |  B4    |   B6   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4        (14)    //        |      D4      |   B4   |  B5    |   B7   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5        (21)    //        |      D5      |   G0   |  G0    |   G0-2 |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6        (47)     //        |      D6      |   G1   |  G1    |   G3   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7        (48)     //        |      D7      |   G2   |  G2    |   G4   |
#if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8                  //        ┗--------------┫--------|--------|--------|
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8        (45)     //                       |   G3   |  G3    |   G5   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9        (38)     //                       |   G4   |  G4    |   G6   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10       (39)     //                       |   G5   |  G5    |   G7   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11       (40)     //                       |   R0   |  R0-1  |   R0-3 |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12       (41)     //                       |   R1   |  R2    |   R4   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13       (42)     //                       |   R2   |  R3    |   R5   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14       (2)     //                       |   R3   |  R4    |   R6   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15       (1)     //                       |   R4   |  R5    |   R7   |
                                                            //                       ┗--------┻--------┻--------┛
#endif // ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH

#endif // ESP_PANEL_BOARD_LCD_BUS_TYPE

#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD() \
{ \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xF0, {0x55, 0xAA, 0x52, 0x08, 0x00}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xF6, {0x5A, 0x87}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x3F}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xCD, {0x25}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xCD, {0x00}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC9, {0x10}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xF8, {0x8A}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xAC, {0x45}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xA7, {0x47}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xA0, {0xDD}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x87, {0x04, 0x03, 0x66}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x86, {0x99, 0xa3, 0xa3, 0x51}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFA, {0x08, 0x08, 0x08, 0x04}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x9A, {0x8a}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x9B, {0x62}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x82, {0x48, 0x48}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB1, {0x10}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x7A, {0x13, 0x1A}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x7B, {0x13, 0x1A}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x6D, {0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x02, 0x0b, 0x01, 0x00, 0x1f, 0x1e, 0x09, 0x0f, 0x1e, 0x1e, 0x1e, 0x1e, 0x10, 0x0a, 0x1e, 0x1f, 0x00, 0x08, 0x0b, 0x02, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x64, {0x18, 0x07, 0x01, 0xE7, 0x03, 0x03, 0x18, 0x06, 0x01, 0xE6, 0x03, 0x03, 0x7a, 0x7a, 0x7a, 0x7a}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x65, {0x58, 0x26, 0x18, 0x2c, 0x03, 0x03, 0x58, 0x26, 0x18, 0x2c, 0x03, 0x03, 0x7a, 0x7a, 0x7a, 0x7a}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x66, {0x58, 0x26, 0x18, 0x2c, 0x03, 0x03, 0x58, 0x26, 0x18, 0x2c, 0x03, 0x03, 0x7a, 0x7a, 0x7a, 0x7a}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x67, {0x18, 0x05, 0x01, 0xE5, 0x03, 0x03, 0x18, 0x04, 0x01, 0xE4, 0x03, 0x03, 0x7a, 0x7a, 0x7a, 0x7a}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x60, {0x18, 0x09, 0x7A, 0x7A, 0x51, 0xF1, 0x7A, 0x7A}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0x63, {0x51, 0xF1, 0x7A, 0x7A, 0x18, 0x08, 0x7A, 0x7A}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xD1, {0x00, 0x00, 0x00, 0x0E, 0x00, 0x31, 0x00, 0x4E, 0x00, 0x67, 0x00, 0x92, 0x00, 0xB5, 0x00, 0xED, 0x01, 0x1C, 0x01, 0x66, 0x01, 0xA4, 0x02, 0x04, 0x02, 0x53, 0x02, 0x56, 0x02, 0x9F, 0x02, 0xF3, 0x03, 0x29, 0x03, 0x73, 0x03, 0xA1, 0x03, 0xB9, 0x03, 0xC8, 0x03, 0xDB, 0x03, 0xE7, 0x03, 0xF4, 0x03, 0xFB, 0x03, 0XFF}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xD2, {0x00, 0x00, 0x00, 0x0E, 0x00, 0x31, 0x00, 0x4E, 0x00, 0x67, 0x00, 0x92, 0x00, 0xB5, 0x00, 0xED, 0x01, 0x1C, 0x01, 0x66, 0x01, 0xA4, 0x02, 0x04, 0x02, 0x53, 0x02, 0x56, 0x02, 0x9F, 0x02, 0xF3, 0x03, 0x29, 0x03, 0x73, 0x03, 0xA1, 0x03, 0xB9, 0x03, 0xC8, 0x03, 0xDB, 0x03, 0xE7, 0x03, 0xF4, 0x03, 0xFB, 0x03, 0XFF}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xD3, {0x00, 0x00, 0x00, 0x0E, 0x00, 0x31, 0x00, 0x4E, 0x00, 0x67, 0x00, 0x92, 0x00, 0xB5, 0x00, 0xED, 0x01, 0x1C, 0x01, 0x66, 0x01, 0xA4, 0x02, 0x04, 0x02, 0x53, 0x02, 0x56, 0x02, 0x9F, 0x02, 0xF3, 0x03, 0x29, 0x03, 0x73, 0x03, 0xA1, 0x03, 0xB9, 0x03, 0xC8, 0x03, 0xDB, 0x03, 0xE7, 0x03, 0xF4, 0x03, 0xFB, 0x03, 0XFF}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xD4, {0x00, 0x00, 0x00, 0x0E, 0x00, 0x31, 0x00, 0x4E, 0x00, 0x67, 0x00, 0x92, 0x00, 0xB5, 0x00, 0xED, 0x01, 0x1C, 0x01, 0x66, 0x01, 0xA4, 0x02, 0x04, 0x02, 0x53, 0x02, 0x56, 0x02, 0x9F, 0x02, 0xF3, 0x03, 0x29, 0x03, 0x73, 0x03, 0xA1, 0x03, 0xB9, 0x03, 0xC8, 0x03, 0xDB, 0x03, 0xE7, 0x03, 0xF4, 0x03, 0xFB, 0x03, 0XFF}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xD5, {0x00, 0x00, 0x00, 0x0E, 0x00, 0x31, 0x00, 0x4E, 0x00, 0x67, 0x00, 0x92, 0x00, 0xB5, 0x00, 0xED, 0x01, 0x1C, 0x01, 0x66, 0x01, 0xA4, 0x02, 0x04, 0x02, 0x53, 0x02, 0x56, 0x02, 0x9F, 0x02, 0xF3, 0x03, 0x29, 0x03, 0x73, 0x03, 0xA1, 0x03, 0xB9, 0x03, 0xC8, 0x03, 0xDB, 0x03, 0xE7, 0x03, 0xF4, 0x03, 0xFB, 0x03, 0XFF}), \
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xD6, {0x00, 0x00, 0x00, 0x0E, 0x00, 0x31, 0x00, 0x4E, 0x00, 0x67, 0x00, 0x92, 0x00, 0xB5, 0x00, 0xED, 0x01, 0x1C, 0x01, 0x66, 0x01, 0xA4, 0x02, 0x04, 0x02, 0x53, 0x02, 0x56, 0x02, 0x9F, 0x02, 0xF3, 0x03, 0x29, 0x03, 0x73, 0x03, 0xA1, 0x03, 0xB9, 0x03, 0xC8, 0x03, 0xDB, 0x03, 0xE7, 0x03, 0xF4, 0x03, 0xFB, 0x03, 0XFF}), \
    ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x11), \
    ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29), \
}

/**
 * @brief LCD specific flags configuration
 *
 * These flags are specific to the "3-wire SPI + RGB" bus.
 */
#if (ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB) && ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL
/**
 * @brief Enable IO multiplex
 *
 * Set to 1 if the 3-wire SPI pins are sharing other pins of the RGB interface to save GPIOs. Then, the control panel
 * and its pins (except CS signal) will be released after LCD call `init()`. All `*_by_cmd` flags will be invalid.
 */
#define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX       (1) // typically set to 0
/**
 * @brief Mirror by command
 *
 * Set to 1 if the `mirror()` function will be implemented by LCD command. Otherwise, the function will be implemented by
 * software. Only valid when `ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX` is 0.
 */
#define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD             (!ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX)
#endif // ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL

/**
 * @brief LCD color configuration
 */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB888)
                                                        // ESP_PANEL_LCD_COLOR_BITS_RGB565/RGB666/RGB888
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (0)     // 0: RGB, 1: BGR
#define ESP_PANEL_BOARD_LCD_COLOR_INEVRT_BIT    (0)     // 0/1

/**
 * @brief LCD transformation configuration
 */
#define ESP_PANEL_BOARD_LCD_SWAP_XY             (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X            (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y            (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_GAP_X               (0)     // [0, ESP_PANEL_BOARD_WIDTH]
#define ESP_PANEL_BOARD_LCD_GAP_Y               (0)     // [0, ESP_PANEL_BOARD_HEIGHT]

/**
 * @brief LCD reset pin configuration
 */
#define ESP_PANEL_BOARD_LCD_RST_IO              (8)    // Reset pin, -1 if not used
#define ESP_PANEL_BOARD_LCD_RST_LEVEL           (0)     // Reset active level, 0: low, 1: high

#endif // ESP_PANEL_BOARD_USE_LCD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Touch panel configuration flag (0/1)
 *
 * Set to `1` to enable touch panel support, `0` to disable
 */
#define ESP_PANEL_BOARD_USE_TOUCH               (0)

#if ESP_PANEL_BOARD_USE_TOUCH
/**
 * @brief Touch controller selection
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        FT5x06

/**
 * @brief Touch bus type selection
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE          (ESP_PANEL_BUS_TYPE_I2C)

#if (ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C) || \
    (ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI)
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 *
 * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
 * So it is not necessary to set the macro to `1`. For other drivers (like `Wire`), please set the macro to `1`
 * ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST        (0)     // 0/1. Typically set to 0
#endif

/**
 * @brief Touch bus parameters configuration
 */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    /**
     * @brief I2C bus
     */
    /* For general */
    #define ESP_PANEL_BOARD_TOUCH_I2C_HOST_ID           (0)     // Typically set to 0
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ            (400 * 1000)
                                                                // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP        (1)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP        (1)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL            (41)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA            (40)
#endif
    /* For panel */
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS           (0)     // Typically set to 0 to use the default address.
                                                                // - For touchs with only one address, set to 0
                                                                // - For touchs with multiple addresses, set to 0 or
                                                                //   the address. Like GT911, there are two addresses:
                                                                //   0x5D(default) and 0x14

#endif // ESP_PANEL_BOARD_TOUCH_BUS_TYPE

/**
 * @brief Touch panel transformation flags
 */
#define ESP_PANEL_BOARD_TOUCH_SWAP_XY           (0)     // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_X          (0)     // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_Y          (0)     // 0/1

/**
 * @brief Touch panel control pins
 */
#define ESP_PANEL_BOARD_TOUCH_RST_IO            (-1)    // Reset pin, -1 if not used
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL         (0)     // Reset active level, 0: low, 1: high
#define ESP_PANEL_BOARD_TOUCH_INT_IO            (-1)    // Interrupt pin, -1 if not used
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL         (0)     // Interrupt active level, 0: low, 1: high

#endif // ESP_PANEL_BOARD_USE_TOUCH

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Backlight configuration flag (0/1)
 *
 * Set to `1` to enable backlight support, `0` to disable
 */
#define ESP_PANEL_BOARD_USE_BACKLIGHT           (1)

#if ESP_PANEL_BOARD_USE_BACKLIGHT
/**
 * @brief Backlight control type selection
 */
#define ESP_PANEL_BOARD_BACKLIGHT_TYPE          (ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

#if (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO) || \
    (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_EXPANDER) || \
    (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

    /**
     * @brief Backlight control pin configuration
     */
    #define ESP_PANEL_BOARD_BACKLIGHT_IO        (7)    // Output GPIO pin number
    #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL  (0)     // Active level, 0: low, 1: high

#endif // ESP_PANEL_BOARD_BACKLIGHT_TYPE

/**
 * @brief Backlight idle state configuration (0/1)
 *
 * Set to 1 if want to turn off the backlight after initializing. Otherwise, the backlight will be on.
 */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF      (0)

#endif // ESP_PANEL_BOARD_USE_BACKLIGHT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief IO expander configuration flag (0/1)
 *
 * Set to `1` to enable IO expander support, `0` to disable
 */
#define ESP_PANEL_BOARD_USE_EXPANDER            (0)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Please utilize the following macros to execute any additional code if required /////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// File Version ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Do not change the following versions. These version numbers are used to check compatibility between this
 * configuration file and the library. Rules for version numbers:
 * 1. Major version mismatch: Configurations are incompatible, must use library version
 * 2. Minor version mismatch: May be missing new configurations, recommended to update
 * 3. Patch version mismatch: No impact on functionality
 */
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0

// *INDENT-ON*
