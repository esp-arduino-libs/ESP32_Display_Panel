/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "esp_check.h"
#include "esp_display_panel.hpp"
#include "esp_lib_utils.h"
#include "lvgl.h"
#include "lvgl_v8_port.h"
#include "lv_demos.h"

#define EXAMPLE_LCD_USE_EXTERNAL_INIT_CMD   (0)

using namespace esp_panel::drivers;
using namespace esp_panel::board;

static const char *TAG = "example";
#if EXAMPLE_LCD_USE_EXTERNAL_INIT_CMD
static const esp_panel_lcd_vendor_init_cmd_t external_init_cmd[] = {
//  {cmd, { data }, data_size, delay_ms}
    // {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x13}, 5, 0},
    // {0xEF, (uint8_t []){0x08}, 1, 0},
    // {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    // ...
};
#endif // EXAMPLE_LCD_USE_EXTERNAL_INIT_CMD

extern "C" void app_main()
{
    Board *board = new Board();
    assert(board);

    ESP_LOGI(TAG, "Initializing board");
    ESP_UTILS_CHECK_FALSE_EXIT(board->init(), "Board init failed");

#if LVGL_PORT_AVOID_TEARING_MODE
    {
        auto lcd = board->getLCD();
        // When avoid tearing function is enabled, the frame buffer number should be set in the board driver
        lcd->configFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
#   if ESP_PANEL_DRIVERS_BUS_ENABLE_RGB && CONFIG_IDF_TARGET_ESP32S3
        auto lcd_bus = lcd->getBus();
        /**
         * As the anti-tearing feature typically consumes more PSRAM bandwidth, for the ESP32-S3, we need to utilize the
         * "bounce buffer" functionality to enhance the RGB data bandwidth.
         * This feature will consume `bounce_buffer_size * bytes_per_pixel * 2` of SRAM memory.
         */
        if (lcd_bus->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) {
            static_cast<BusRGB *>(lcd_bus)->configRGB_BounceBufferSize(lcd->getFrameWidth() * 10);
        }
#   endif
    }
#endif // LVGL_PORT_AVOID_TEARING_MODE

#if EXAMPLE_LCD_USE_EXTERNAL_INIT_CMD
    ESP_LOGI(TAG, "Using external LCD init command");
    {
        auto lcd = board->getLCD();
        ESP_UTILS_CHECK_FALSE_EXIT(
            lcd->configVendorCommands(external_init_cmd, sizeof(external_init_cmd) / sizeof(external_init_cmd[0])),
            "LCD init failed"
        );
    }
    /**
     * In addition, you can also get handles to any other devices (like touch) and use `config*()` functions to
     * configure or replace default parameters, but this must be completed before `board->begin()`
     */
#endif // EXAMPLE_LCD_USE_EXTERNAL_INIT_CMD

    ESP_UTILS_CHECK_FALSE_EXIT(board->begin(), "Board begin failed");

    ESP_LOGI(TAG, "Initializing LVGL");
    ESP_UTILS_CHECK_FALSE_EXIT(lvgl_port_init(board->getLCD(), board->getTouch()), "LVGL init failed");

    ESP_LOGI(TAG, "Creating UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    // lv_demo_widgets();
    // lv_demo_benchmark();
    lv_demo_music();
    // lv_demo_stress();

    /* Release the mutex */
    lvgl_port_unlock();
}
