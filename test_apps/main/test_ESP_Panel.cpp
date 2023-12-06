/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <inttypes.h>

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "unity.h"
#include "unity_test_runner.h"

#include <ESP_Panel_Library.h>
#include <ESP_IOExpander_Library.h>

static const char *TAG = "test_ESP_Panel";

#if ESP_PANEL_LCD_BUS_TYPE != ESP_PANEL_BUS_TYPE_RGB
static bool lcd_trans_done_callback(void *user_ctx)
{
    BaseType_t need_yield = pdFALSE;
    SemaphoreHandle_t sem_lcd_trans_done = (SemaphoreHandle_t)user_ctx;
    xSemaphoreGiveFromISR(sem_lcd_trans_done, &need_yield);

    return (need_yield == pdTRUE);
}
#endif /* ESP_PANEL_LCD_BUS_TYPE */

TEST_CASE("test draw lcd", "[panel][lcd]")
{
    ESP_Panel *panel = new ESP_Panel();
    TEST_ASSERT_NOT_NULL(panel);

    ESP_IOExpander *expander = NULL;
    SemaphoreHandle_t sem_lcd_trans_done = NULL;

    /* There are some extral initialization for ESP32-S3-LCD-EV-Board */
#ifdef CONFIG_ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD
    /* Initialize IO expander */
    expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    TEST_ASSERT_NOT_NULL(expander);
    expander->init();
    expander->begin();
    /* Add into panel for 3-wire SPI */
    panel->addIOExpander(expander);
    /* For the newest version sub board, need to set `ESP_PANEL_LCD_RGB_IO_VSYNC` to high before initialize LCD */
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = BIT64(ESP_PANEL_LCD_RGB_IO_VSYNC);
    io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf);
    gpio_set_level((gpio_num_t)ESP_PANEL_LCD_RGB_IO_VSYNC, 1);
#endif

    /* There are some extral initialization for ESP32-S3-Korvo-2 */
#ifdef CONFIG_ESP_PANEL_BOARD_ESP32_S3_KORVO_2
    /* Initialize IO expander */
    expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    TEST_ASSERT_NOT_NULL(expander);
    expander->init();
    expander->begin();
    /* Reset LCD */
    expander->pinMode(2, OUTPUT);
    expander->digitalWrite(2, LOW);
    vTaskDelay(pdMS_TO_TICKS(20));
    expander->digitalWrite(2, LOW);
    vTaskDelay(pdMS_TO_TICKS(120));
    expander->digitalWrite(2, HIGH);
    /* Turn on backlight */
    expander->pinMode(1, OUTPUT);
    expander->digitalWrite(1, HIGH);
    /* Keep CS low */
    expander->pinMode(3, OUTPUT);
    expander->digitalWrite(3, LOW);
#endif

    ESP_LOGI(TAG, "Initialize panel");
    /* Initialize bus and device of panel */
    panel->init();
#if ESP_PANEL_LCD_BUS_TYPE != ESP_PANEL_BUS_TYPE_RGB
    sem_lcd_trans_done = xSemaphoreCreateBinary();
    TEST_ASSERT_NOT_NULL(sem_lcd_trans_done);
    /* Register a function to notify when the panel is ready to refresh */
    /* This is useful for refreshing the screen using DMA transfers */
    panel->getLcd()->attachFrameEndCallback(lcd_trans_done_callback, sem_lcd_trans_done);
#endif
    /* Start panel */
    panel->begin();

    ESP_LOGI(TAG, "Draw color bar from top to bottom, the order is B - G - R");
    uint16_t line_per_bar = ESP_PANEL_LCD_V_RES / ESP_PANEL_LCD_COLOR_BITS;
    uint16_t *color = (uint16_t *)calloc(1, line_per_bar * ESP_PANEL_LCD_H_RES * ESP_PANEL_LCD_COLOR_BITS / 8);
    for (int j = 0; j < ESP_PANEL_LCD_COLOR_BITS; j++) {
        for (int i = 0; i < line_per_bar * ESP_PANEL_LCD_H_RES; i++) {
#if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
            color[i] = SPI_SWAP_DATA_TX(1ULL << j, ESP_PANEL_LCD_COLOR_BITS);
#else
            color[i] = 1ULL << j;
#endif
        }
        panel->getLcd()->drawBitmap(0, j * line_per_bar, ESP_PANEL_LCD_H_RES, (j + 1) * line_per_bar, color);
#if ESP_PANEL_LCD_BUS_TYPE != ESP_PANEL_BUS_TYPE_RGB
        xSemaphoreTake(sem_lcd_trans_done, portMAX_DELAY);
#endif
    }
    free(color);

    vTaskDelay(pdMS_TO_TICKS(3000));

    delete panel;
    if (expander) {
        delete expander;
    }
    if (sem_lcd_trans_done) {
        vSemaphoreDelete(sem_lcd_trans_done);
    }
}

#if ESP_PANEL_USE_LCD_TOUCH
TEST_CASE("test read touch", "[panel][touch]")
{
    ESP_Panel *panel = new ESP_Panel();
    TEST_ASSERT_NOT_NULL(panel);

    ESP_IOExpander *expander = NULL;

    /* There are some extral initialization for ESP32-S3-LCD-EV-Board */
#ifdef CONFIG_ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD
    /* Initialize IO expander */
    expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    TEST_ASSERT_NOT_NULL(expander);
    expander->init();
    expander->begin();
    /* Add into panel for 3-wire SPI */
    panel->addIOExpander(expander);
    /* For the newest version sub board, need to set `ESP_PANEL_LCD_RGB_IO_VSYNC` to high before initialize LCD */
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = BIT64(ESP_PANEL_LCD_RGB_IO_VSYNC);
    io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&io_conf);
    gpio_set_level((gpio_num_t)ESP_PANEL_LCD_RGB_IO_VSYNC, 1);
#endif

    /* There are some extral initialization for ESP32-S3-Korvo-2 */
#ifdef CONFIG_ESP_PANEL_BOARD_ESP32_S3_KORVO_2
    /* Initialize IO expander */
    expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    TEST_ASSERT_NOT_NULL(expander);
    expander->init();
    expander->begin();
#endif

    ESP_LOGI(TAG, "Initialize panel");
    /* Initialize bus and device of panel */
    panel->init();
    /* Start panel */
    panel->begin();

    ESP_LOGI(TAG, "Read touch point in 3s");
    int cnt = 0;
    while (cnt++ < 100) {
        panel->getLcdTouch()->readData();
        bool touched = panel->getLcdTouch()->getLcdTouchState();
        if (touched) {
            TouchPoint point = panel->getLcdTouch()->getPoint();
            ESP_LOGI(TAG, "Touch point: x %d, y %d\n", point.x, point.y);
        }
        vTaskDelay(pdMS_TO_TICKS(30));
    }

    delete panel;
    if (expander) {
        delete expander;
    }
}
#endif

// Some resources are lazy allocated in the LCD driver, the threadhold is left for that case
#define TEST_MEMORY_LEAK_THRESHOLD (-300)

static size_t before_free_8bit;
static size_t before_free_32bit;

static void check_leak(size_t before_free, size_t after_free, const char *type)
{
    ssize_t delta = after_free - before_free;
    printf("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_MESSAGE(delta >= TEST_MEMORY_LEAK_THRESHOLD, "memory leak");
}

void setUp(void)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
}

void tearDown(void)
{
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

extern "C" void app_main(void)
{
    //    __  __    ___     ___                 _
    //   /__\/ _\  / _ \   / _ \__ _ _ __   ___| |
    //  /_\  \ \  / /_)/  / /_)/ _` | '_ \ / _ \ |
    // //__  _\ \/ ___/  / ___/ (_| | | | |  __/ |
    // \__/  \__/\/      \/    \__,_|_| |_|\___|_|
    printf("    __  __    ___     ___                 _\r\n");
    printf("   /__\\/ _\\  / _ \\   / _ \\__ _ _ __   ___| |\r\n");
    printf("  /_\\  \\ \\  / /_)/  / /_)/ _` | '_ \\ / _ \\ |\r\n");
    printf(" //__  _\\ \\/ ___/  / ___/ (_| | | | |  __/ |\r\n");
    printf(" \\__/  \\__/\\/      \\/    \\__,_|_| |_|\\___|_|\r\n");
    unity_run_menu();
}
