// /*
// SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
//  *
//  * SPDX-License-Identifier: Apache-2.0
//  */

// #include "soc/soc_caps.h"


// #if SOC_LCD_RGB_SUPPORTED
// #include "driver/gpio.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_check.h"
// #include "esp_lcd_panel_io.h"
// #include "esp_lcd_panel_rgb.h"
// #include "esp_lcd_panel_vendor.h"
// #include "esp_log.h"

// #include "ESP_PanelPrivate.h"
// #include "../bus/RGB.h"
// #include "ST7262.h"

// static const char *TAG = "st7262";

// ESP_PanelLcd_ST7262::~ESP_PanelLcd_ST7262()
// {
//     if (handle) {
//         del();
//     }
// }

// void ESP_PanelLcd_ST7262::init()
// {
//     if (panel_config.reset_gpio_num >= 0) {
//         gpio_config_t gpio_conf = {
//             .pin_bit_mask = BIT64(panel_config.reset_gpio_num),
//             .mode = GPIO_MODE_OUTPUT,
//             .pull_up_en = GPIO_PULLUP_DISABLE,
//             .pull_down_en = GPIO_PULLDOWN_DISABLE,
//             .intr_type = GPIO_INTR_DISABLE,
//         };
//         CHECK_ERROR_RETURN(gpio_config(&gpio_conf));
//     }

//     CHECK_ERROR_RETURN(esp_lcd_new_rgb_panel(vendor_config.rgb_config, &handle));
// }

// void ESP_PanelLcd_ST7262::reset()
// {
//     if (panel_config.reset_gpio_num >= 0) {
//         gpio_set_level((gpio_num_t)panel_config.reset_gpio_num, panel_config.flags.reset_active_high);
//         vTaskDelay(pdMS_TO_TICKS(10));
//         gpio_set_level((gpio_num_t)panel_config.reset_gpio_num, !panel_config.flags.reset_active_high);
//         vTaskDelay(pdMS_TO_TICKS(120));
//     }
//     CHECK_ERROR_RETURN(esp_lcd_panel_reset(handle));
// }

// #endif /* SOC_LCD_RGB_SUPPORTED */
