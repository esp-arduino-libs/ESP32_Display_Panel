/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * @file   BOARD_UEDX24320024E_WB_A.h
 * @brief  Configuration file for Viewe UEDX24320024E-WB-A
 * @author Viewe@VIEWESMART
 * @link   https://viewedisplay.com/product/esp32-2-4-inch-240x320-rgb-ips-tft-display-touch-screen-arduino-lvgl-wifi-ble-uart-smart-module/
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure general panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Board name
 */
#define ESP_PANEL_BOARD_NAME                "Viewe:SMARTRING"

/**
 * @brief Panel resolution configuration in pixels
 */
#define ESP_PANEL_BOARD_WIDTH               (472)   // Panel width (horizontal, in pixels)
#define ESP_PANEL_BOARD_HEIGHT              (466)   // Panel height (vertical, in pixels)

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
#define ESP_PANEL_BOARD_LCD_CONTROLLER      SH8601

/**
 * @brief LCD bus type selection
 *
 * Supported bus types:
 * - `ESP_PANEL_BUS_TYPE_SPI`
 * - `ESP_PANEL_BUS_TYPE_QSPI`
 * - `ESP_PANEL_BUS_TYPE_RGB` (ESP32-S3 only)
 * - `ESP_PANEL_BUS_TYPE_MIPI_DSI` (ESP32-P4 only)
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE        (ESP_PANEL_BUS_TYPE_QSPI)

#if (ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI) || \
    (ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI)
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 *
 * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
 * So it is not necessary to set the macro to `1`. For other drivers (like `Wire`), please set the macro to `1`
 * ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST      (0)     // 0/1. Typically set to 0
#endif

/**
 * @brief LCD bus parameters configuration
 *
 * Configure parameters based on the selected bus type. Parameters for other bus types will be ignored.
 * For detailed parameter explanations, see:
 * https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32s3/api-reference/peripherals/lcd/index.html
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html
 */
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI

/**
 * @brief QSPI bus
 */
/* For general */
#define ESP_PANEL_BOARD_LCD_QSPI_HOST_ID        (1)     // Typically set to 1
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
/* For host */
#define ESP_PANEL_BOARD_LCD_QSPI_IO_SCK         (13)
#define ESP_PANEL_BOARD_LCD_QSPI_IO_DATA0       (12)
#define ESP_PANEL_BOARD_LCD_QSPI_IO_DATA1       (8)
#define ESP_PANEL_BOARD_LCD_QSPI_IO_DATA2       (14)
#define ESP_PANEL_BOARD_LCD_QSPI_IO_DATA3       (9)
#endif // ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
/* For panel */
#define ESP_PANEL_BOARD_LCD_QSPI_IO_CS          (7)     // -1 if not used
#define ESP_PANEL_BOARD_LCD_QSPI_MODE           (0)     // 0-3, typically set to 0
#define ESP_PANEL_BOARD_LCD_QSPI_CLK_HZ         (40 * 1000 * 1000)
                                                        // Should be an integer divisor of 80M, typically set to 40M
#define ESP_PANEL_BOARD_LCD_QSPI_CMD_BITS       (32)    // Typically set to 32
#define ESP_PANEL_BOARD_LCD_QSPI_PARAM_BITS     (8)     // Typically set to 8

#endif // ESP_PANEL_BOARD_LCD_BUS_TYPE

/**
 * @brief LCD vendor initialization commands
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * The initialization sequence can be specified in two formats:
 * 1. Raw format:
 *    {command, (uint8_t []){data0, data1, ...}, data_size, delay_ms}
 * 2. Helper macros:
 *    - ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, {data0, data1, ...})
 *    - ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD() \
{                                                                          \
    {0xFE, (uint8_t []){0x00}, 0, 0},                                      \
    {0xC4, (uint8_t []){0x80}, 1, 0},                                      \
    {0x3A, (uint8_t []){0x55}, 1, 0},                                      \
    {0x35, (uint8_t []){0x00}, 0, 10},                                     \
    {0x53, (uint8_t []){0x20}, 1, 10},                                     \
    {0x51, (uint8_t []){0xFF}, 1, 10},                                     \
    {0x63, (uint8_t []){0xFF}, 1, 10},                                     \
    {0x2A, (uint8_t []){0x00,0x06,0x01,0xDD}, 4, 0},                       \
    {0x2B, (uint8_t []){0x00,0x00,0x01,0xD1}, 4, 0},                       \
    {0x11, (uint8_t []){0x00}, 0, 60},                                     \
    {0x29, (uint8_t []){0x00}, 0, 0},                                      \
}
/**
 * @brief LCD color configuration
 */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB565)
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
#define ESP_PANEL_BOARD_LCD_RST_IO              (11)    // Reset pin, -1 if not used
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
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        CST816S

/**
 * @brief Touch bus type selection
 * - `ESP_PANEL_BUS_TYPE_SPI`
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
    #define ESP_PANEL_BOARD_TOUCH_I2C_HOST_ID           (1)     // Typically set to 0
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ            (400 * 1000)
                                                                // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP        (1)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP        (1)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL            (45)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA            (41)
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
#define ESP_PANEL_BOARD_TOUCH_RST_IO            (46)    // Reset pin, -1 if not used
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL         (0)     // Reset active level, 0: low, 1: high
#define ESP_PANEL_BOARD_TOUCH_INT_IO            (42)    // Interrupt pin, -1 if not used
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
#define ESP_PANEL_BOARD_USE_BACKLIGHT           (0)

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
    #define ESP_PANEL_BOARD_BACKLIGHT_IO        (17)    // Output GPIO pin number
    #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL  (1)     // Active level, 0: low, 1: high

#endif // ESP_PANEL_BOARD_BACKLIGHT_TYPE

/**
 * @brief Backlight idle state configuration (0/1)
 *
 * Set to 1 if want to turn off the backlight after initializing. Otherwise, the backlight will be on.
 */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF      (1)

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
/**
 * @brief Pre-begin function for board initialization
 *
 * @param[in] p Pointer to the board object
 * @return true on success, false on failure
 */
#define ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION(p) \
    {  \
        constexpr gpio_num_t BL = static_cast<gpio_num_t>(40); \
        gpio_set_direction(BL, GPIO_MODE_OUTPUT); \
        gpio_set_level(BL, 1); \
        return true;    \
    }

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
