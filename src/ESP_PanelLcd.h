/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_rgb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "lcd/base/esp_lcd_custom_types.h"
#include "ESP_PanelBus.h"

#define ESP_LCD_DEVICE_CONFIG_DEFAULT(rst_io, color_bits, vendor_cfg) \
    {                                                                 \
        .reset_gpio_num = rst_io,                                     \
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,                   \
        .data_endian = LCD_RGB_DATA_ENDIAN_BIG,                       \
        .bits_per_pixel = color_bits,                                 \
        .flags = {                                                    \
            .reset_active_high = 0,                                   \
        },                                                            \
        .vendor_config = vendor_cfg,                                  \
    }

#define ESP_LCD_COMMON_VENDOR_CONFIG_DEFAULT(cmds, cmds_size)   \
    {                                                           \
        .init_cmds = cmds,                                      \
        .init_cmds_size = cmds_size,                            \
    }

class ESP_PanelLcd {
public:
    ESP_PanelLcd(ESP_PanelBus *bus, uint8_t color_bits, int rst_io, const esp_lcd_panel_vendor_init_cmd_t init_cmd[],
                 uint16_t init_cmd_size);
    ESP_PanelLcd(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config);
    virtual ~ESP_PanelLcd() = default;

    /**
     * The below functions can be used to configure LCD device when not using `panel_config`.
     * These functions should be called before `init()`.
     *
     */
    void setColorBits(int bits_per_pixel);
    void setResetPin(int rst_io);
    void setInitCommands(const esp_lcd_panel_vendor_init_cmd_t init_cmd[], int init_cmd_size);
#if SOC_LCD_RGB_SUPPORTED
    void enableAutoReleaseBus(void);
#endif

    virtual void init(void) = 0;
    bool begin(void);
    void reset(void);
    void del(void);
    void drawBitmap(int x_start, int y_start, int x_end, int y_end, const void *color_data);
    void drawBitmapWaitUntilFinish(int x_start, int y_start, int x_end, int y_end,
                                   const void *color_data, int timeout_ms = -1);
    void mirror(bool mirror_x, bool mirror_y);
    void swapAxes(bool en);
    void setGap(int x_gap, int y_gap);
    void invertColor(bool en);
    void displayOn(void);
    void displayOff(void);
    void drawColorBar(int width, int height);
    void attachDrawBitmapFinishCallback(std::function<bool (void *)> callback, void *user_data = NULL);

    uint8_t getPixelColorBits(void);
    esp_lcd_panel_handle_t getHandle(void);
    ESP_PanelBus *getBus(void);

protected:
    ESP_PanelBus *bus;
    esp_lcd_panel_dev_config_t panel_config;
    esp_lcd_panel_vendor_config_t vendor_config;
    esp_lcd_panel_handle_t handle;

private:
    static bool onDrawBitmapFinish(void *panel_io, void *edata, void *user_ctx);

    std::function<bool (void *)> onDrawBitmapFinishCallback;
    SemaphoreHandle_t sem_draw_bitmap_finish;

    typedef struct {
        void *lcd_ptr;
        void *user_data;
    } ESP_PanelLcdCallbackData_t;
    ESP_PanelLcdCallbackData_t callback_data;
};
