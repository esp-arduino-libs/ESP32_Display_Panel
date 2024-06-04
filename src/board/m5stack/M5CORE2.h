/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
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
 * LCD Controller of M5Core2 is ILI9342C, but the driver is compatible with ILI9341.
 */
#define ESP_PANEL_LCD_NAME          ILI9341

/* LCD resolution in pixels */
#define ESP_PANEL_LCD_WIDTH         (320)
#define ESP_PANEL_LCD_HEIGHT        (240)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_BUS_SKIP_INIT_HOST    (0)     // 0/1
/**
 * LCD Bus Type.
 */
#define ESP_PANEL_LCD_BUS_TYPE      (ESP_PANEL_BUS_TYPE_SPI)
/**
 * LCD Bus Parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html and
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html for more details.
 *
 */
#if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

    #define ESP_PANEL_LCD_BUS_HOST_ID           (1)     // Typically set to 1
    #define ESP_PANEL_LCD_SPI_IO_CS             (5)
#if !ESP_PANEL_LCD_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_LCD_SPI_IO_SCK            (18)
    #define ESP_PANEL_LCD_SPI_IO_MOSI           (23)
    #define ESP_PANEL_LCD_SPI_IO_MISO           (-1)    // -1 if not used
#endif
    #define ESP_PANEL_LCD_SPI_IO_DC             (15)
    #define ESP_PANEL_LCD_SPI_MODE              (0)     // 0/1/2/3, typically set to 0
    #define ESP_PANEL_LCD_SPI_CLK_HZ            (40 * 1000 * 1000)
                                                        // Should be an integer divisor of 80M, typically set to 40M
    #define ESP_PANEL_LCD_SPI_TRANS_QUEUE_SZ    (10)    // Typically set to 10
    #define ESP_PANEL_LCD_SPI_CMD_BITS          (8)     // Typically set to 8
    #define ESP_PANEL_LCD_SPI_PARAM_BITS        (8)     // Typically set to 8

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
#define ESP_PANEL_LCD_COLOR_BITS    (16)        // 8/16/18/24
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_LCD_BGR_ORDER     (1)         // 0/1
#define ESP_PANEL_LCD_INEVRT_COLOR  (1)         // 0/1

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
 * Touch Controller of M5Core2 is FT6336U, but the driver is compatible with FT5x06.
 */
#define ESP_PANEL_TOUCH_NAME        FT5x06

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
    #define ESP_PANEL_TOUCH_I2C_IO_SCL      (22)
    #define ESP_PANEL_TOUCH_I2C_IO_SDA      (21)
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
#define ESP_PANEL_TOUCH_IO_INT          (39)
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
#define ESP_PANEL_BEGIN_END_FUNCTION( panel ) \
    { \
        static const uint8_t AXP_ADDR = 0x34; \
        static const uint8_t I2C_MASTER_TIMEOUT_MS = 1000; \
        static i2c_port_t i2c_master_port = I2C_NUM_0; \
         \
        uint8_t device_id = 0; \
        uint8_t read_value = 0; \
        uint8_t write_value = 0; \
        uint8_t reg_addr = 0; \
        uint8_t write_buf[2] = {0}; \
         \
        reg_addr = 0x03; \
        i2c_master_write_read_device(i2c_master_port, AXP_ADDR, &reg_addr, 1, &device_id, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
        if (device_id == 0x03) { \
            reg_addr = 0x28; \
            i2c_master_write_read_device(i2c_master_port, AXP_ADDR, &reg_addr, 1, &read_value, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
            write_value = (read_value & 0x0F) | ((((3300 - 1800) / 100) & 0x0F) << 4); \
            write_buf[0] = 0x28; \
            write_buf[1] = write_value; \
            i2c_master_write_to_device(i2c_master_port, AXP_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
             \
            reg_addr = 0x12; \
            i2c_master_write_read_device(i2c_master_port, AXP_ADDR, &reg_addr, 1, &read_value, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
            write_value = (read_value | (0x01 << 2)); \
            write_buf[0] = 0x12; \
            write_buf[1] = write_value; \
            i2c_master_write_to_device(i2c_master_port, AXP_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
             \
            reg_addr = 0x12; \
            i2c_master_write_read_device(i2c_master_port, AXP_ADDR, &reg_addr, 1, &read_value, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
            write_value = (read_value | (0x01 << 1)); \
            write_buf[0] = 0x12; \
            write_buf[1] = write_value; \
            i2c_master_write_to_device(i2c_master_port, AXP_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
        } else if (device_id == 0x4A) { \
            reg_addr = 0x90; \
            read_value = device_id; \
            i2c_master_write_read_device(i2c_master_port, AXP_ADDR, &reg_addr, 1, &read_value, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
            write_value = (read_value | (1 << 4)); \
            write_buf[0] = 0x90; \
            write_buf[1] = write_value; \
            i2c_master_write_to_device(i2c_master_port, AXP_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
             \
            write_value = (3000 - 500) / 100; \
            write_buf[0] = 0x94; \
            write_buf[1] = write_value; \
            i2c_master_write_to_device(i2c_master_port, AXP_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS); \
        } else { \
         \
        } \
    }
// *INDENT-OFF*
