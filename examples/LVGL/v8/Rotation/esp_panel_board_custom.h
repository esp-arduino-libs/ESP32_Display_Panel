/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

/* Set to 1 if using a custom board */
#define ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM  (0)     // 0/1

#if ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure general panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Panel resolution in pixels */
#define ESP_PANEL_BOARD_WIDTH               (320)
#define ESP_PANEL_BOARD_HEIGHT              (240)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the LCD panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using a LCD panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_LCD     (1)     // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_LCD
/**
 * LCD controller name. Choose one of the following:
 *      - AXS15231B
 *      - EK9716B, EK79007
 *      - GC9A01, GC9B71, GC9503
 *      - HX8399
 *      - ILI9341, ILI9881C
 *      - JD9165, JD9365
 *      - NV3022B
 *      - SH8601
 *      - SPD2010
 *      - ST7262, ST7701, ST7703, ST7789, ST7796, ST77903, ST77916, ST77922
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER      ILI9341

/**
 * LCD bus type. Choose one of the following:
 *      - ESP_PANEL_BUS_TYPE_SPI
 *      - ESP_PANEL_BUS_TYPE_QSPI
 *      - ESP_PANEL_BUS_TYPE_RGB (only supported for ESP32-S3)
 *      - ESP_PANEL_BUS_TYPE_MIPI_DSI (only supported for ESP32-P4)
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE        (ESP_PANEL_BUS_TYPE_RGB)

/**
 * LCD bus parameters.
 *
 * There are different parameters for different bus types. Please only configure the parameters for the selected bus type.
 * For the parameters of other bus types, they will be ignored. To understand the parameters, please check
 * https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32s3/api-reference/peripherals/lcd/index.html and
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html for more details.
 *
 */
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

    /**
     * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
     *
     * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
     * So it is not necessary to set the macro to `1`. For other devices, please set the macro to `1` ensure that the
     * host is initialized only once.
     *
     */
    #define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST  (0)     // 0/1. Typically set to 0
    /* For general */
    #define ESP_PANEL_BOARD_LCD_BUS_HOST_ID         (1)     // Typically set to 1
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK          (7)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_MOSI         (6)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_MISO         (-1)    // -1 if not used
#endif // ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    /* For panel */
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS           (5)     // -1 if not used
    #define ESP_PANEL_BOARD_LCD_SPI_IO_DC           (4)
    #define ESP_PANEL_BOARD_LCD_SPI_MODE            (0)     // 0/1/2/3. Typically set to 0
    #define ESP_PANEL_BOARD_LCD_SPI_CLK_HZ          (40 * 1000 * 1000)
                                                            // Should be an integer divisor of 80M, typically set to 40M
    #define ESP_PANEL_BOARD_LCD_SPI_CMD_BITS        (8)     // Typically set to 8
    #define ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS      (8)     // Typically set to 8

#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI

    /**
     * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
     *
     * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
     * So it is not necessary to set the macro to `1`. For other devices, please set the macro to `1` ensure that the
     * host is initialized only once.
     *
     */
    #define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST  (0)     // 0/1. Typically set to 0
    /* For general */
    #define ESP_PANEL_BOARD_LCD_BUS_HOST_ID         (1)     // Typically set to 1
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_LCD_QSPI_IO_SCK         (9)
    #define ESP_PANEL_BOARD_LCD_QSPI_IO_DATA0       (10)
    #define ESP_PANEL_BOARD_LCD_QSPI_IO_DATA1       (11)
    #define ESP_PANEL_BOARD_LCD_QSPI_IO_DATA2       (12)
    #define ESP_PANEL_BOARD_LCD_QSPI_IO_DATA3       (13)
#endif // ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    /* For panel */
    #define ESP_PANEL_BOARD_LCD_QSPI_IO_CS          (5)     // -1 if not used
    #define ESP_PANEL_BOARD_LCD_QSPI_MODE           (0)     // 0/1/2/3, typically set to 0
    #define ESP_PANEL_BOARD_LCD_QSPI_CLK_HZ         (40 * 1000 * 1000)
                                                            // Should be an integer divisor of 80M, typically set to 40M
    #define ESP_PANEL_BOARD_LCD_QSPI_CMD_BITS       (32)    // Typically set to 32
    #define ESP_PANEL_BOARD_LCD_QSPI_PARAM_BITS     (8)     // Typically set to 8

#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB

    /**
     * Set to 1 if not using the 3-wire SPI interface
     */
    #define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST          (0)
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    /* For control panel (3wire-SPI) */
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS                   (0)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK                  (1)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SDA                  (2)
    #define ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER         (0) // Set to 1 if the signal is controlled by an IO expander
    #define ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER        (0) // Set to 1 if the signal is controlled by an IO expander
    #define ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER        (0) // Set to 1 if the signal is controlled by an IO expander
    #define ESP_PANEL_BOARD_LCD_SPI_SCL_ACTIVE_EDGE         (0) // 0: rising edge, 1: falling edge. Typically set to 0
    /* For device */
    #define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX   (0) // Set to 1 if the 3-wire SPI pins are sharing other pins of
                                                                // the RGB interface to save GPIOs. Then, the panel IO and
                                                                // its pins (except CS signal) will be released after LCD
                                                                // call `init()`. All `*_by_cmd` flags will be invalid.
    #define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD         (!ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX)
                                                                // Set to 1 if the `mirror()` function will be implemented by
                                                                // LCD command. Otherwise, the function will be implemented
                                                                // by software. Only valid when
                                                                // `ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX` is 0.
#endif // ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    /* For refresh panel (RGB) */
    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ          (16 * 1000 * 1000)
                                                            // To increase the upper limit of the PCLK, please check
                                                            // https://docs.espressif.com/projects/esp-faq/en/latest/software-framework/peripherals/lcd.html#how-can-i-increase-the-upper-limit-of-pclk-settings-on-esp32-s3-while-ensuring-normal-rgb-screen-display
    #define ESP_PANEL_BOARD_LCD_RGB_HPW             (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HBP             (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HFP             (20)
    #define ESP_PANEL_BOARD_LCD_RGB_VPW             (10)
    #define ESP_PANEL_BOARD_LCD_RGB_VBP             (10)
    #define ESP_PANEL_BOARD_LCD_RGB_VFP             (10)
    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG (0)     // 0: rising edge, 1: falling edge. Typically set to 0

                                                                                        // ┏---------------------------------┳- -------------------------------┓
    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH      (16)                                // |                16               |               8                 |
    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS      (ESP_PANEL_LCD_COLOR_BITS_RGB565)   // | ESP_PANEL_LCD_COLOR_BITS_RGB565 | ESP_PANEL_LCD_COLOR_BITS_RGB888 |
                                                                                        // ┗---------------------------------┻---------------------------------┛
                                                            // To understand color format of RGB LCD, please check
                                                            // https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/rgb_lcd.html#color-formats
    #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE (ESP_PANEL_BOARD_WIDTH * 10)
                                                            // Bounce buffer size in bytes. It is used to avoid screen drift
                                                            // for ESP32-S3. Typically set to `ESP_PANEL_BOARD_WIDTH * 10`
                                                            // The size should satisfy `size * N = LCD_width * LCD_height`,
                                                            // where N is an even number.
                                                            // For more details, please check https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/FAQ.md#how-to-fix-screen-drift-issue-when-driving-rgb-lcd-with-esp32-s3
    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC        (46)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC        (3)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE           (17)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK         (9)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP         (-1)    // -1 if not used. Typically set to -1
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
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6        (47)    //        |      D6      |   G1   |  G1    |   G3   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7        (48)    //        |      D7      |   G2   |  G2    |   G4   |
#if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8                  //        ┗--------------┫        |        |        |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8        (45)    //                       |   G3   |  G3    |   G5   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9        (38)    //                       |   G4   |  G4    |   G6   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10       (39)    //                       |   G5   |  G5    |   G7   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11       (40)    //                       |   R0   |  R0-1  |   R0-3 |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12       (41)    //                       |   R1   |  R2    |   R4   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13       (42)    //                       |   R2   |  R3    |   R5   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14       (2)     //                       |   R3   |  R4    |   R6   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15       (1)     //                       |   R4   |  R5    |   R7   |
                                                            //                       ┗--------┻--------┻--------┛
#endif // ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH

#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_MIPI_DSI

    /* For host */
    #define ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_NUM           (2)     // ESP32-P4 supports 1 or 2 lanes
    #define ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_RATE_MBPS     (1000)  // Single lane bit rate, should check the LCD drive IC
                                                                    // datasheet for the supported lane rate. Different
                                                                    // color format (RGB565/RGB888) may have different
                                                                    // lane bit rate requirements.
                                                                    // ESP32-P4 supports max 1500Mbps
    #define ESP_PANEL_BOARD_LCD_MIPI_DSI_PHY_LDO_ID         (3)     // -1 if not used.
    /* For refresh panel (DPI) */
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_CLK_MHZ            (52)
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_PIXEL_BITS         (ESP_PANEL_LCD_COLOR_BITS_RGB565)
                                                                    // ESP_PANEL_LCD_COLOR_BITS_RGB565/RGB666/RGB888
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_HPW                (10)
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_HBP                (160)
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_HFP                (160)
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_VPW                (1)
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_VBP                (23)
    #define ESP_PANEL_BOARD_LCD_MIPI_DPI_VFP                (12)

#else

    #error "The function is not ready and will be implemented in the future."

#endif /* ESP_PANEL_BOARD_LCD_BUS_TYPE */

/**
 * LCD vendor initialization commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                 ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
/*
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD()                       \
    {                                                               \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},   \
        {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},                     \
        {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},                     \
        {0x29, (uint8_t []){0x00}, 0, 120},                         \
        or
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),                   \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),                   \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),                               \
    }
*/

/* LCD device color */
/* Color depth in bits */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS      (ESP_PANEL_LCD_COLOR_BITS_RGB565)
                                                    // ESP_PANEL_LCD_COLOR_BITS_RGB565/RGB666/RGB888
/* Color RGB element order */
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER (0)     // 0: RGB, 1: BGR

/* LCD pre-process flags */
#define ESP_PANEL_BOARD_LCD_INEVRT_COLOR    (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_SWAP_XY         (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X        (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y        (0)     // 0/1

/* LCD other settings */
/* Reset pin */
#define ESP_PANEL_BOARD_LCD_RST_IO          (-1)    // Reset IO pin num. Set to -1 if not use
#define ESP_PANEL_BOARD_LCD_RST_LEVEL       (0)     // Reset active level. 0: low level, 1: high level

#endif // ESP_PANEL_BOARD_DEFAULT_USE_LCD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using a touch panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_TOUCH   (1)     // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
/**
 * Touch controller name. Choose one of the following:
 *      - AXS15231B
 *      - CST816S
 *      - FT5x06
 *      - GT911, GT1151
 *      - SPD2010
 *      - ST1633, ST7123
 *      - STMPE610
 *      - TT21100
 *      - XPT2046
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER    TT21100

/**
 * Touch bus type. Choose one of the following:
 *      - ESP_PANEL_BUS_TYPE_I2C
 *      - ESP_PANEL_BUS_TYPE_SPI
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE      (ESP_PANEL_BUS_TYPE_I2C)

/**
 * Touch bus parameters.
 *
 */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    /**
     * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
     *
     * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
     * So it is not necessary to set the macro to `1`. For other devices, please set the macro to `1` ensure that the
     * host is initialized only once.
     *
     */
    #define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST    (0)     // 0/1. Typically set to 0
    /* For general */
    #define ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID           (0)     // Typically set to 0
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ            (400 * 1000)
                                                                // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP        (1)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP        (1)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL            (18)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA            (8)
#endif
    /* For panel */
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS           (0)     // Typically set to 0 to use the default address.
                                                                // - For touchs with only one address, set to 0
                                                                // - For touchs with multiple addresses, set to 0 or
                                                                //   the address. Like GT911, there are two addresses:
                                                                // 0x5D(default) and 0x14

#elif ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

    /**
     * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
     *
     * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
     * So it is not necessary to set the macro to `1`. For other devices, please set the macro to `1` ensure that the
     * host is initialized only once.
     *
     */
    #define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST    (0)     // 0/1. Typically set to 0
    /* For general */
    #define ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID           (1)     // Typically set to 1
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK            (7)
    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI           (6)
    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO           (9)
#endif
    /* For panel */
    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_CS             (5)
    #define ESP_PANEL_BOARD_TOUCH_SPI_CLK_HZ            (1 * 1000 * 1000)
                                                                // Should be an integer divisor of 80M, typically set to 1M
#else

    #error "The function is not ready and will be implemented in the future."

#endif // ESP_PANEL_BOARD_TOUCH_BUS_TYPE

/* Touch pre-process flags */
#define ESP_PANEL_BOARD_TOUCH_SWAP_XY       (0)     // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_X      (0)     // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_Y      (0)     // 0/1

/* Touch other settings */
/* Reset pin */
#define ESP_PANEL_BOARD_TOUCH_RST_IO        (-1)    // Reset IO pin num. Set to -1 if not use
                                                    // For GT911, this pin is also used to configure the I2C address
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL     (0)     // Reset active level. 0: low level, 1: high level
/* Interrupt pin */
#define ESP_PANEL_BOARD_TOUCH_INT_IO        (-1)    // Interrupt IO pin num. Set to -1 if not use
                                                    // For GT911, this pin is also used to configure the I2C address
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL     (0)     // Interrupt active level. 0: low level, 1: high level

#endif // ESP_PANEL_BOARD_DEFAULT_USE_TOUCH

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using the backlight */
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT       (1)     // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
/**
 * Backlight control mode. Choose one of the following:
 *      - ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO: Use GPIO switch to control the backlight, only support on/off
 *      - ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC:    Use LEDC PWM to control the backlight, support brightness adjustment
 *      - ESP_PANEL_BACKLIGHT_TYPE_CUSTOM:      Use custom function to control the backlight
 *
 */
#define ESP_PANEL_BOARD_BACKLIGHT_TYPE          (ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

#if (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO) || \
    (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

    /* Output pin */
    #define ESP_PANEL_BOARD_BACKLIGHT_IO        (38)    // Output IO pin num
    #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL  (1)     // Light up level. 0: low level, 1: high level

#elif ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_CUSTOM

    /**
     * @brief Custom backlight control function
     *
     * @param[in] percent   Brightness percentage, 0-100
     * @param[in] user_data User data, default is a pointer of `Board`
     *
     * @return true if successful, otherwise false
     *
     */
    #define ESP_PANEL_BOARD_BACKLIGHT_CUSTOM_FUNCTION( percent, user_data )  \
        {  \
            /* esp_panel::Board *board = static_cast<esp_panel::Board *>(user_data); */ \
            return true; \
        }

#endif // ESP_PANEL_BOARD_BACKLIGHT_TYPE

/* Set to 1 if want to turn off the backlight after initializing. Otherwise, the backlight will be on */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF      (0)     // 0/1

#endif // ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using an IO Expander */
#define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER    (1)     // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER
/**
 * IO expander chip name. Choose one of the following:
 *      - CH422G
 *      - HT8574
 *      - TCA95XX_8BIT
 *      - TCA95XX_16BIT
 */
#define ESP_PANEL_BOARD_EXPANDER_CHIP           TCA95XX_8BIT

/* IO expander parameters */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 *
 * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
 * So it is not necessary to set the macro to `1`. For other devices, please set the macro to `1` ensure that the
 * host is initialized only once.
 *
 */
#define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST     (0)     // 0/1
/* For general */
#define ESP_PANEL_BOARD_EXPANDER_HOST_ID            (0)     // Typically set to 0
/* For host */
#if !ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
#define ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ         (400 * 1000)
                                                            // Typically set to 400K
#define ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP     (1)     // 0/1. Typically set to 1
#define ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP     (1)     // 0/1. Typically set to 1
#define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL         (18)
#define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA         (8)
#endif // ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
/* For device */
#define ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS        (0x20)  // The actual I2C address. Even for the same model of IC,
                                                            // the I2C address may be different, and confirmation based on
                                                            // the actual hardware connection is required
#endif // ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Please utilize the following macros to execute any additional code if required /////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Pre-begin function for the board, will be called in head of the `begin()` function
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Post-begin function for the board, will be called in tail of the `begin()` function
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_POST_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Pre-begin function for the IO expander, will be called before the IO expander beginning
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Post-begin function for the IO expander, will be called after the IO expander beginning
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Pre-begin function for the LCD, will be called before the LCD beginning
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Post-begin function for the LCD, will be called after the LCD beginning
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Pre-begin function for the touch, will be called before the touch beginning
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Post-begin function for the touch, will be called after the touch beginning
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Pre-begin function for the backlight, will be called before the backlight beginning
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

/**
 * @brief Post-begin function for the backlight, will be called the backlight beginning
 *
 * @param[in] p Pointer to the board object
 *
 * @return true if successful, otherwise false
 *
 */
/*
#define ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION( p ) \
    {  \
        esp_panel::Board *board = static_cast<esp_panel::Board *>(p);  \
        return true;    \
    }
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// File Version ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Do not change the following versions, they are used to check if the configurations in this file are compatible with
 * the current version of `esp_panel_board_custom.h` in the library. The detailed rules are as follows:
 *
 *   1. If the major version is not consistent, then the configurations in this file are incompatible with the library
 *      and must be replaced with the file from the library.
 *   2. If the minor version is not consistent, this file might be missing some new configurations, which will be set to
 *      default values. It is recommended to replace it with the file from the library.
 *   3. Even if the patch version is not consistent, it will not affect normal functionality.
 *
 */
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM */

// *INDENT-OFF*