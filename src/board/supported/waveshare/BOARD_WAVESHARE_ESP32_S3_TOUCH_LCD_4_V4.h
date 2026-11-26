/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * @file   BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_V4.h
 * @brief  Configuration file for Waveshare ESP32-S3-Touch-LCD-4 (4" square, 480x480) - V4 hardware
 * @note   V4 hardware only. The V4 board uses a CH32V003 microcontroller as its I2C IO expander,
 *         driven by the WAVESHARE_LCD_4_CH32V003 expander class. (V3 hardware uses a different
 *         expander and is not covered by this file.)
 * @link   https://www.waveshare.com/esp32-s3-touch-lcd-4.htm
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure general panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Board name
 */
#define ESP_PANEL_BOARD_NAME                "Waveshare:ESP32-S3-Touch-LCD-4-V4"

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
 */
#define ESP_PANEL_BOARD_USE_LCD             (1)

#if ESP_PANEL_BOARD_USE_LCD
/**
 * @brief LCD controller selection
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER      ST7701

/**
 * @brief LCD bus type selection
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE        (ESP_PANEL_BUS_TYPE_RGB)

#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB

    /**
     * @brief RGB bus
     */
    #define ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL       (1) // 0/1. Typically set to 1

#if ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL
    /* For control panel (3-wire SPI) - signals are on MCU GPIOs, not the IO expander */
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_CS               (42)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SCK              (2)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SDA              (1)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_CS_USE_EXPNADER     (0) // CS is MCU GPIO, not expander
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_SCL_USE_EXPNADER    (0)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_SDA_USE_EXPNADER    (0)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_MODE                (0) // 0-3, typically set to 0
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_CMD_BYTES           (1)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_PARAM_BYTES         (1)
    #define ESP_PANEL_BOARD_LCD_RGB_SPI_USE_DC_BIT          (1) // 0/1. Typically set to 1
#endif // ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL
    /* For refresh panel (RGB) */
    // RGB timing from the confirmed-working reference project.
    // PCLK_ACTIVE_NEG=0: panel samples on rising edge (critical - wrong polarity = corrupt pixels).
    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ          (16 * 1000 * 1000)
    #define ESP_PANEL_BOARD_LCD_RGB_HPW             (8)
    #define ESP_PANEL_BOARD_LCD_RGB_HBP             (40)
    #define ESP_PANEL_BOARD_LCD_RGB_HFP             (20)
    #define ESP_PANEL_BOARD_LCD_RGB_VPW             (8)
    #define ESP_PANEL_BOARD_LCD_RGB_VBP             (20)
    #define ESP_PANEL_BOARD_LCD_RGB_VFP             (10)
    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG (0)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH      (16)
    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS      (ESP_PANEL_LCD_COLOR_BITS_RGB565)
    #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE (ESP_PANEL_BOARD_WIDTH * 10)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC        (38)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC        (39)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE           (40)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK         (41)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP         (-1)    // -1 if not used

    // Data bus: B0-B4, G0-G5, R0-R4
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0        (5)     // B0
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1        (45)    // B1
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2        (48)    // B2
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3        (47)    // B3
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4        (21)    // B4
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5        (14)    // G0
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6        (13)    // G1
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7        (12)    // G2
#if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8        (11)    // G3
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9        (10)    // G4
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10       (9)     // G5
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11       (46)    // R0
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12       (3)     // R1
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13       (8)     // R2
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14       (18)    // R3
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15       (17)    // R4
#endif // ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH

#endif // ESP_PANEL_BOARD_LCD_BUS_TYPE

/**
 * @brief LCD specific flags configuration ("3-wire SPI + RGB" bus)
 */
#if (ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB) && ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL
#define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX       (0) // typically set to 0
#define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD             (!ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX)
#endif // ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL

/**
 * @brief LCD vendor initialization commands
 *
 * Vendor init sequence for Waveshare ESP32-S3-Touch-LCD-4 (ST7701, 480x480).
 * Format: {cmd, {data bytes}, data_len, delay_ms}
 */
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD() \
    { \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0}, \
        {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0}, \
        {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0}, \
        {0xC2, (uint8_t []){0x31, 0x05}, 2, 0}, \
        {0xCD, (uint8_t []){0x08}, 1, 0}, \
        {0xB0, (uint8_t []){0x00, 0x11, 0x18, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22, 0x04, 0x12, 0x0F, 0xAA, 0x31, \
                            0x18}, 16, 0}, \
        {0xB1, (uint8_t []){0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x08, 0x08, 0x08, 0x22, 0x04, 0x11, 0x11, 0xA9, 0x32, \
                            0x18}, 16, 0}, \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x11}, 5, 0}, \
        {0xB0, (uint8_t []){0x60}, 1, 0}, \
        {0xB1, (uint8_t []){0x32}, 1, 0}, \
        {0xB2, (uint8_t []){0x07}, 1, 0}, \
        {0xB3, (uint8_t []){0x80}, 1, 0}, \
        {0xB5, (uint8_t []){0x49}, 1, 0}, \
        {0xB7, (uint8_t []){0x85}, 1, 0}, \
        {0xB8, (uint8_t []){0x21}, 1, 0}, \
        {0xC1, (uint8_t []){0x78}, 1, 0}, \
        {0xC2, (uint8_t []){0x78}, 1, 0}, \
        {0xE0, (uint8_t []){0x00, 0x1B, 0x02}, 3, 0}, \
        {0xE1, (uint8_t []){0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44, 0x44}, 11, 0}, \
        {0xE2, (uint8_t []){0x11, 0x11, 0x44, 0x44, 0xED, 0xA0, 0x00, 0x00, 0xEC, 0xA0, 0x00, 0x00}, 12, 0}, \
        {0xE3, (uint8_t []){0x00, 0x00, 0x11, 0x11}, 4, 0}, \
        {0xE4, (uint8_t []){0x44, 0x44}, 2, 0}, \
        {0xE5, (uint8_t []){0x0A, 0xE9, 0xD8, 0xA0, 0x0C, 0xEB, 0xD8, 0xA0, 0x0E, 0xED, 0xD8, 0xA0, 0x10, 0xEF, 0xD8, \
                            0xA0}, 16, 0}, \
        {0xE6, (uint8_t []){0x00, 0x00, 0x11, 0x11}, 4, 0}, \
        {0xE7, (uint8_t []){0x44, 0x44}, 2, 0}, \
        {0xE8, (uint8_t []){0x09, 0xE8, 0xD8, 0xA0, 0x0B, 0xEA, 0xD8, 0xA0, 0x0D, 0xEC, 0xD8, 0xA0, 0x0F, 0xEE, 0xD8, \
                            0xA0}, 16, 0}, \
        {0xEB, (uint8_t []){0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40}, 7, 0}, \
        {0xEC, (uint8_t []){0x3C, 0x00}, 2, 0}, \
        {0xED, (uint8_t []){0xAB, 0x89, 0x76, 0x54, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x45, 0x67, 0x98, \
                            0xBA}, 16, 0}, \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x13}, 5, 0}, \
        {0xE5, (uint8_t []){0xE4}, 1, 0}, \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x00}, 5, 0}, \
        {0x21, (uint8_t []){0x00}, 0, 0}, \
        {0x3A, (uint8_t []){0x60}, 1, 0}, \
        {0x11, (uint8_t []){0x00}, 0, 120}, \
        {0x29, (uint8_t []){0x00}, 0, 0}, \
    }

/**
 * @brief LCD color configuration
 */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB666)
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (0)     // 0: RGB, 1: BGR
#define ESP_PANEL_BOARD_LCD_COLOR_INEVRT_BIT    (0)     // 0/1

/**
 * @brief LCD transformation configuration
 */
#define ESP_PANEL_BOARD_LCD_SWAP_XY             (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X            (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y            (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_GAP_X               (0)
#define ESP_PANEL_BOARD_LCD_GAP_Y               (0)

/**
 * @brief LCD reset pin configuration (reset is handled via the IO expander, see pre-begin hook)
 */
#define ESP_PANEL_BOARD_LCD_RST_IO              (-1)    // Reset pin, -1 if not used
#define ESP_PANEL_BOARD_LCD_RST_LEVEL           (0)     // Reset active level, 0: low, 1: high

#endif // ESP_PANEL_BOARD_USE_LCD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Touch panel configuration flag (0/1)
 */
#define ESP_PANEL_BOARD_USE_TOUCH               (1)

#if ESP_PANEL_BOARD_USE_TOUCH
/**
 * @brief Touch controller selection
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        GT911

/**
 * @brief Touch bus type selection
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE          (ESP_PANEL_BUS_TYPE_I2C)

#if (ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C) || \
    (ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI)
#define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST        (0)     // 0/1. Shares the I2C host with the expander
#endif

/**
 * @brief Touch bus parameters configuration
 */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    /* For general */
    #define ESP_PANEL_BOARD_TOUCH_I2C_HOST_ID           (0)     // Typically set to 0
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ            (400 * 1000)
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP        (1)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP        (1)     // 0/1. Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL            (7)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA            (15)
#endif
    /* For panel */
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS           (0)     // 0 = auto; GT911 has 0x5D(default)/0x14

#endif // ESP_PANEL_BOARD_TOUCH_BUS_TYPE

/**
 * @brief Touch panel transformation flags
 */
#define ESP_PANEL_BOARD_TOUCH_SWAP_XY           (0)     // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_X          (0)     // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_Y          (0)     // 0/1

/**
 * @brief Touch panel control pins (reset & strap are handled via the IO expander, see pre-begin hook)
 */
#define ESP_PANEL_BOARD_TOUCH_RST_IO            (-1)    // Reset pin, -1 if not used
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL         (0)
#define ESP_PANEL_BOARD_TOUCH_INT_IO            (-1)    // Interrupt pin, -1 if not used (polling)
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL         (0)

#endif // ESP_PANEL_BOARD_USE_TOUCH

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Backlight configuration flag (0/1)
 *
 * Backlight is the CH32V003's dedicated PWM register, driven through the
 * WAVESHARE_LCD_4_CH32V003 expander class (see custom function below).
 */
#define ESP_PANEL_BOARD_USE_BACKLIGHT           (1)

#if ESP_PANEL_BOARD_USE_BACKLIGHT
/**
 * @brief Backlight control type selection
 */
#define ESP_PANEL_BOARD_BACKLIGHT_TYPE          (ESP_PANEL_BACKLIGHT_TYPE_CUSTOM)

/**
 * @brief Backlight idle state configuration (0/1)
 */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF      (0)

#endif // ESP_PANEL_BOARD_USE_BACKLIGHT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief IO expander configuration flag (0/1)
 *
 * The V4 board uses a CH32V003 microcontroller as its I2C IO expander.
 */
#define ESP_PANEL_BOARD_USE_EXPANDER            (1)

#if ESP_PANEL_BOARD_USE_EXPANDER
/**
 * @brief IO expander chip selection
 */
#define ESP_PANEL_BOARD_EXPANDER_CHIP           WAVESHARE_LCD_4_CH32V003

/**
 * @brief IO expander I2C bus parameters configuration
 */
#define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST     (0)     // 0/1. Initializes the shared I2C host
/* For general */
#define ESP_PANEL_BOARD_EXPANDER_I2C_HOST_ID        (0)     // Typically set to 0
/* For host */
#if !ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
#define ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ         (400 * 1000)
#define ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP     (1)     // 0/1. Typically set to 1
#define ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP     (1)     // 0/1. Typically set to 1
#define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL         (7)
#define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA         (15)
#endif // ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
/* For device */
#define ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS        (0x24)
#endif // ESP_PANEL_BOARD_USE_EXPANDER

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Please utilize the following macros to execute any additional code if required /////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EXIO pin map (CH32V003 register bits): 1=TP_RST 2=TP_INT 3=LCD_RST 5=SYS_EN 6=BEE_EN

/**
 * @brief Post-begin function for IO expander initialization
 *
 * Enable system power and set the control pins to outputs. The output latch powers up low, so
 * LCD_RST/TP_RST stay asserted (in reset) until their own pre-begin hooks release them.
 *
 * @param[in] p Pointer to the board object
 * @return true on success, false on failure
 */
#define ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION(p) \
    {  \
        constexpr int TP_RST = 1, LCD_RST = 3, SYS_EN = 5, BEE_EN = 6; \
        auto board = static_cast<Board *>(p);  \
        auto expander = board->getIO_Expander()->getBase(); \
        expander->pinMode(SYS_EN, OUTPUT); \
        expander->digitalWrite(SYS_EN, HIGH); \
        expander->pinMode(LCD_RST, OUTPUT); \
        expander->pinMode(TP_RST, OUTPUT); \
        expander->pinMode(BEE_EN, OUTPUT); \
        expander->digitalWrite(BEE_EN, LOW); \
        return true;    \
    }

/**
 * @brief Pre-begin function for LCD initialization
 *
 * Reset the LCD (EXIO3) before the ST7701 vendor init sequence runs.
 *
 * @param[in] p Pointer to the board object
 * @return true on success, false on failure
 */
#define ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION(p) \
    {  \
        constexpr int LCD_RST = 3; \
        auto board = static_cast<Board *>(p);  \
        auto expander = board->getIO_Expander()->getBase(); \
        expander->digitalWrite(LCD_RST, LOW); \
        vTaskDelay(pdMS_TO_TICKS(10)); \
        expander->digitalWrite(LCD_RST, HIGH); \
        vTaskDelay(pdMS_TO_TICKS(120)); \
        return true;    \
    }

/**
 * @brief Pre-begin function for touch panel initialization
 *
 * Reset the touch panel (EXIO1) and strap the GT911 address via TP_INT (EXIO2). TP_INT held HIGH
 * on the RST rising edge selects the default GT911 address 0x5D.
 *
 * @param[in] p Pointer to the board object
 * @return true on success, false on failure
 */
#define ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION(p) \
    {  \
        constexpr int TP_RST = 1, TP_INT = 2; \
        auto board = static_cast<Board *>(p);  \
        auto expander = board->getIO_Expander()->getBase(); \
        expander->pinMode(TP_INT, OUTPUT); \
        expander->digitalWrite(TP_INT, HIGH); \
        expander->digitalWrite(TP_RST, LOW); \
        vTaskDelay(pdMS_TO_TICKS(10)); \
        expander->digitalWrite(TP_RST, HIGH); \
        vTaskDelay(pdMS_TO_TICKS(200)); \
        expander->pinMode(TP_INT, INPUT);   /* release INT for polling */ \
        return true;    \
    }

/**
 * @brief Custom backlight function
 *
 * Route the library's brightness control to the CH32V003 PWM register via the expander class.
 *
 * @param[in] percent   Brightness percentage (0-100)
 * @param[in] user_data Pointer to the board object
 * @return true on success, false on failure
 */
#define ESP_PANEL_BOARD_BACKLIGHT_CUSTOM_FUNCTION(percent, user_data) \
    {  \
        auto board = static_cast<Board *>(user_data);  \
        auto expander = static_cast<esp_expander::WAVESHARE_LCD_4_CH32V003 *>(board->getIO_Expander()->getBase()); \
        return expander->setBacklight(percent); \
    }

// Note: the beeper has no equivalent in the public Board API (no library "beep" concept).
// App code drives it through the expander, e.g.:
//   static_cast<esp_expander::WAVESHARE_LCD_4_CH32V003 *>(board->getIO_Expander()->getBase())->beep(true);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// File Version ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Do not change the following versions. These version numbers are used to check compatibility between this
 * configuration file and the library.
 */
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0

// *INDENT-ON*
