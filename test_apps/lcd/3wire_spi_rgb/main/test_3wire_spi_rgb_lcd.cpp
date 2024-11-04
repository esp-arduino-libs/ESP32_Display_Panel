/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "unity.h"
#include "unity_test_runner.h"
#include "ESP_Panel_Library.h"

using namespace std;

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_LCD_WIDTH                      (800)
#define TEST_LCD_HEIGHT                     (480)
                                                    // | 8-bit RGB888 | 16-bit RGB565 |
#define TEST_LCD_COLOR_BITS                 (18)    // |      24      |   16/18/24    |
#define TEST_LCD_RGB_DATA_WIDTH             (16)    // |      8       |      16       |
#define TEST_LCD_RGB_TIMING_FREQ_HZ         (16 * 1000 * 1000)
#define TEST_LCD_RGB_TIMING_HPW             (10)
#define TEST_LCD_RGB_TIMING_HBP             (10)
#define TEST_LCD_RGB_TIMING_HFP             (20)
#define TEST_LCD_RGB_TIMING_VPW             (10)
#define TEST_LCD_RGB_TIMING_VBP             (10)
#define TEST_LCD_RGB_TIMING_VFP             (10)
#define TEST_LCD_RGB_BOUNCE_BUFFER_SIZE     (TEST_LCD_WIDTH * 10)
#define TEST_LCD_USE_EXTERNAL_CMD           (1)
#if TEST_LCD_USE_EXTERNAL_CMD
/**
 * LCD initialization commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code.
 *
 * Please uncomment and change the following macro definitions, then use `configVendorCommands()` to pass them in the
 * same format if needed. Otherwise, the LCD driver will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
const esp_lcd_panel_vendor_init_cmd_t lcd_init_cmd[] = {
    // {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    // {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},
    // {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},
    // {0x29, (uint8_t []){0x00}, 0, 120},
    // // or
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),
    ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),
};
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_LCD_PIN_NUM_RGB_DISP           (-1)
#define TEST_LCD_PIN_NUM_RGB_VSYNC          (3)
#define TEST_LCD_PIN_NUM_RGB_HSYNC          (46)
#define TEST_LCD_PIN_NUM_RGB_DE             (17)
#define TEST_LCD_PIN_NUM_RGB_PCLK           (9)
                                                    // | RGB565 | RGB666 | RGB888 |
                                                    // |--------|--------|--------|
#define TEST_LCD_PIN_NUM_RGB_DATA0          (10)    // |   B0   |  B0-1  |   B0-3 |
#define TEST_LCD_PIN_NUM_RGB_DATA1          (11)    // |   B1   |  B2    |   B4   |
#define TEST_LCD_PIN_NUM_RGB_DATA2          (12)    // |   B2   |  B3    |   B5   |
#define TEST_LCD_PIN_NUM_RGB_DATA3          (13)    // |   B3   |  B4    |   B6   |
#define TEST_LCD_PIN_NUM_RGB_DATA4          (14)    // |   B4   |  B5    |   B7   |
#define TEST_LCD_PIN_NUM_RGB_DATA5          (21)    // |   G0   |  G0    |   G0-2 |
#define TEST_LCD_PIN_NUM_RGB_DATA6          (47)    // |   G1   |  G1    |   G3   |
#define TEST_LCD_PIN_NUM_RGB_DATA7          (48)    // |   G2   |  G2    |   G4   |
#if TEST_LCD_RGB_DATA_WIDTH > 8
#define TEST_LCD_PIN_NUM_RGB_DATA8          (45)    // |   G3   |  G3    |   G5   |
#define TEST_LCD_PIN_NUM_RGB_DATA9          (38)    // |   G4   |  G4    |   G6   |
#define TEST_LCD_PIN_NUM_RGB_DATA10         (39)    // |   G5   |  G5    |   G7   |
#define TEST_LCD_PIN_NUM_RGB_DATA11         (40)    // |   R0   |  R0-1  |   R0-3 |
#define TEST_LCD_PIN_NUM_RGB_DATA12         (41)    // |   R1   |  R2    |   R4   |
#define TEST_LCD_PIN_NUM_RGB_DATA13         (42)    // |   R2   |  R3    |   R5   |
#define TEST_LCD_PIN_NUM_RGB_DATA14         (2)     // |   R3   |  R4    |   R6   |
#define TEST_LCD_PIN_NUM_RGB_DATA15         (1)     // |   R4   |  R5    |   R7   |
#endif
#define TEST_LCD_PIN_NUM_SPI_CS             (39)
#define TEST_LCD_PIN_NUM_SPI_SCK            (48)
#define TEST_LCD_PIN_NUM_SPI_SDA            (47)
#define TEST_LCD_PIN_NUM_RST                (-1)    // Set to -1 if not used
#define TEST_LCD_PIN_NUM_BK_LIGHT           (-1)    // Set to -1 if not used
#define TEST_LCD_BK_LIGHT_ON_LEVEL          (1)
#define TEST_LCD_BK_LIGHT_OFF_LEVEL !TEST_LCD_BK_LIGHT_ON_LEVEL

// *INDENT-OFF*

/* Enable or disable printing RGB refresh rate */
#define TEST_ENABLE_PRINT_LCD_FPS           (1)
/* Enable or disable the attachment of a callback function that is called after each bitmap drawing is completed */
#define TEST_ENABLE_ATTACH_CALLBACK         (1)
#define TEST_COLOR_BAR_SHOW_TIME_MS         (3000)

static const char *TAG = "test_3wire_spi_rgb_lcd";

static shared_ptr<ESP_PanelBacklight> init_backlight(void)
{
#if TEST_LCD_PIN_NUM_BK_LIGHT >= 0
    ESP_LOGI(TAG, "Initialize backlight control pin and turn it on");
    shared_ptr<ESP_PanelBacklight> backlight = make_shared<ESP_PanelBacklight>(
                TEST_LCD_PIN_NUM_BK_LIGHT, TEST_LCD_BK_LIGHT_ON_LEVEL, true
            );
    TEST_ASSERT_NOT_NULL_MESSAGE(backlight, "Create backlight object failed");

    TEST_ASSERT_TRUE_MESSAGE(backlight->begin(), "Backlight begin failed");
    TEST_ASSERT_TRUE_MESSAGE(backlight->on(), "Backlight on failed");

    return backlight;
#else
    return nullptr;
#endif
}

static shared_ptr<ESP_PanelBus_RGB> init_panel_bus(void)
{
    ESP_LOGI(TAG, "Create LCD bus");
    shared_ptr<ESP_PanelBus_RGB> panel_bus = make_shared<ESP_PanelBus_RGB>(
#if TEST_LCD_RGB_DATA_WIDTH == 8
        TEST_LCD_WIDTH, TEST_LCD_HEIGHT,
        TEST_LCD_PIN_NUM_SPI_CS, TEST_LCD_PIN_NUM_SPI_SCK, TEST_LCD_PIN_NUM_SPI_SDA,
        TEST_LCD_PIN_NUM_RGB_DATA0, TEST_LCD_PIN_NUM_RGB_DATA1, TEST_LCD_PIN_NUM_RGB_DATA2, TEST_LCD_PIN_NUM_RGB_DATA3,
        TEST_LCD_PIN_NUM_RGB_DATA4, TEST_LCD_PIN_NUM_RGB_DATA5, TEST_LCD_PIN_NUM_RGB_DATA6, TEST_LCD_PIN_NUM_RGB_DATA7,
        TEST_LCD_PIN_NUM_RGB_HSYNC, TEST_LCD_PIN_NUM_RGB_VSYNC, TEST_LCD_PIN_NUM_RGB_PCLK, TEST_LCD_PIN_NUM_RGB_DE,
        TEST_LCD_PIN_NUM_RGB_DISP
#elif TEST_LCD_RGB_DATA_WIDTH == 16
        TEST_LCD_WIDTH, TEST_LCD_HEIGHT,
        TEST_LCD_PIN_NUM_SPI_CS, TEST_LCD_PIN_NUM_SPI_SCK, TEST_LCD_PIN_NUM_SPI_SDA,
        TEST_LCD_PIN_NUM_RGB_DATA0, TEST_LCD_PIN_NUM_RGB_DATA1, TEST_LCD_PIN_NUM_RGB_DATA2, TEST_LCD_PIN_NUM_RGB_DATA3,
        TEST_LCD_PIN_NUM_RGB_DATA4, TEST_LCD_PIN_NUM_RGB_DATA5, TEST_LCD_PIN_NUM_RGB_DATA6, TEST_LCD_PIN_NUM_RGB_DATA7,
        TEST_LCD_PIN_NUM_RGB_DATA8, TEST_LCD_PIN_NUM_RGB_DATA9, TEST_LCD_PIN_NUM_RGB_DATA10, TEST_LCD_PIN_NUM_RGB_DATA11,
        TEST_LCD_PIN_NUM_RGB_DATA12, TEST_LCD_PIN_NUM_RGB_DATA13, TEST_LCD_PIN_NUM_RGB_DATA14, TEST_LCD_PIN_NUM_RGB_DATA15,
        TEST_LCD_PIN_NUM_RGB_HSYNC, TEST_LCD_PIN_NUM_RGB_VSYNC, TEST_LCD_PIN_NUM_RGB_PCLK, TEST_LCD_PIN_NUM_RGB_DE,
        TEST_LCD_PIN_NUM_RGB_DISP
#endif
    );
    TEST_ASSERT_NOT_NULL_MESSAGE(panel_bus, "Create panel bus object failed");

    panel_bus->configRgbTimingFreqHz(TEST_LCD_RGB_TIMING_FREQ_HZ);
    panel_bus->configRgbTimingPorch(
        TEST_LCD_RGB_TIMING_HPW, TEST_LCD_RGB_TIMING_HBP, TEST_LCD_RGB_TIMING_HFP,
        TEST_LCD_RGB_TIMING_VPW, TEST_LCD_RGB_TIMING_VBP, TEST_LCD_RGB_TIMING_VFP
    );
    panel_bus->configRgbBounceBufferSize(TEST_LCD_RGB_BOUNCE_BUFFER_SIZE); // Set bounce buffer to avoid screen drift
    TEST_ASSERT_TRUE_MESSAGE(panel_bus->begin(), "Panel bus begin failed");

    return panel_bus;
}

#if TEST_ENABLE_PRINT_LCD_FPS
#define TEST_LCD_FPS_COUNT_MAX               (100)
#ifndef millis
#define millis()                                (esp_timer_get_time() / 1000)
#endif

DRAM_ATTR int frame_count = 0;
DRAM_ATTR int fps = 0;
DRAM_ATTR long start_time = 0;

IRAM_ATTR bool onVsyncEndCallback(void *user_data)
{
    long frame_start_time = *(long *)user_data;
    if (frame_start_time == 0) {
        (*(long *)user_data) = millis();

        return false;
    }

    frame_count++;
    if (frame_count >= TEST_LCD_FPS_COUNT_MAX) {
        fps = TEST_LCD_FPS_COUNT_MAX * 1000 / (millis() - frame_start_time);
        frame_count = 0;
        (*(long *)user_data) = millis();
    }

    return false;
}
#endif

static void run_test(shared_ptr<ESP_PanelLcd> lcd)
{
#if TEST_LCD_USE_EXTERNAL_CMD
    // Configure external initialization commands, should called before `init()`
    lcd->configVendorCommands(lcd_init_cmd, sizeof(lcd_init_cmd) / sizeof(lcd_init_cmd[0]));
#endif
    TEST_ASSERT_TRUE_MESSAGE(lcd->init(), "LCD init failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->reset(), "LCD reset failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->begin(), "LCD begin failed");
#if TEST_LCD_PIN_NUM_RGB_DISP >= 0
    TEST_ASSERT_TRUE_MESSAGE(lcd->displayOn(), "LCD display on failed");
#endif
#if TEST_ENABLE_PRINT_LCD_FPS
    TEST_ASSERT_TRUE_MESSAGE(
        lcd->attachRefreshFinishCallback(onVsyncEndCallback, (void *)&start_time), "Attach refresh callback failed"
    );
#endif

    ESP_LOGI(TAG, "Draw color bar from top left to bottom right, the order is B - G - R");
    TEST_ASSERT_TRUE_MESSAGE(lcd->colorBarTest(TEST_LCD_WIDTH, TEST_LCD_HEIGHT), "LCD color bar test failed");
}

#define CREATE_LCD(name, panel_bus) \
    ({ \
        ESP_LOGI(TAG, "Create LCD device: " #name); \
        shared_ptr<ESP_PanelLcd> lcd = make_shared<ESP_PanelLcd_##name>(panel_bus, TEST_LCD_COLOR_BITS, TEST_LCD_PIN_NUM_RST); \
        TEST_ASSERT_NOT_NULL_MESSAGE(lcd, "Create LCD object failed"); \
        lcd; \
    })
#define CREATE_TEST_CASE(name) \
    TEST_CASE("Test LCD (" #name ") to draw color bar", "[3wire_spi_rgb_lcd][" #name "]") \
    { \
        shared_ptr<ESP_PanelBacklight> backlight = init_backlight(); \
        shared_ptr<ESP_PanelBus_RGB> panel_bus = init_panel_bus(); \
        shared_ptr<ESP_PanelLcd> lcd = CREATE_LCD(name, panel_bus.get()); \
        run_test(lcd); \
    }

/**
 * Here to create test cases for different LCDs
 *
 */
CREATE_TEST_CASE(GC9503)
CREATE_TEST_CASE(ST7701)
