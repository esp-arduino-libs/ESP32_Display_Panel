/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELLCD_H
#define ESP_PANELLCD_H

#include <stdint.h>

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_rgb.h"

#include "lcd/base/esp_lcd_custom_types.h"
#include "../bus/ESP_PanelBus.h"

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
#define ESP_LCD_DEVICE_CONFIG_DEFAULT(rst_io, pixel_bits, vendor_cfg)   \
    {                                                               \
        .reset_gpio_num = rst_io,                                   \
        .color_space = ESP_LCD_COLOR_SPACE_RGB,                     \
        .bits_per_pixel = pixel_bits,                               \
        .flags = {                                                  \
            .reset_active_high = 0,                                 \
        },                                                          \
        .vendor_config = vendor_cfg,                                \
    }
#else
#define ESP_LCD_DEVICE_CONFIG_DEFAULT(rst_io, pixel_bits, vendor_cfg)   \
    {                                                               \
        .reset_gpio_num = rst_io,                                   \
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,                 \
        .data_endian = LCD_RGB_DATA_ENDIAN_BIG,                     \
        .bits_per_pixel = pixel_bits,                               \
        .flags = {                                                  \
            .reset_active_high = 0,                                 \
        },                                                          \
        .vendor_config = vendor_cfg,                                \
    }
#endif

#define ESP_LCD_COMMON_VENDOR_CONFIG_DEFAULT(cmds, cmds_size)   \
    {                                                           \
        .init_cmds = cmds,                                      \
        .init_cmds_size = cmds_size,                            \
    }

#if SOC_LCD_RGB_SUPPORTED && (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(5, 0, 0))
#define ESP_LCD_RGB_VENDOR_CONFIG_DEFAULT(cmds, cmds_size, rgb_cfg, enable_3wire_spi_share_pin)  \
    {                                                                   \
        .init_cmds = cmds,                                              \
        .init_cmds_size = cmds_size,                                    \
        .rgb_config = rgb_cfg,                                          \
        .flags = {                                                      \
            .auto_del_panel_io = enable_3wire_spi_share_pin,            \
        },                                                              \
    }
#endif

class ESP_PanelLcd {
public:
    ESP_PanelLcd(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t *panel_config);
    ESP_PanelLcd(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config);
    ESP_PanelLcd(ESP_PanelBus *bus, int color_bits, int rst_io = -1,
                 const lcd_init_cmd_t init_cmd[] = NULL, int init_cmd_size = 0);
    ESP_PanelLcd(ESP_PanelBus *bus);

    void attachFrameEndCallback(ESP_BusCallback_t onFrameEndCallback, void *user_data = NULL);

    /**
     * The below functions can be used to configure LCD device when not using `panel_config`.
     * The below functions should be called before `begin()`.
     */
    void setColorBits(int bits_per_pixel);
    void setResetPin(int rst_io);
    void setInitCommands(const lcd_init_cmd_t init_cmd[], int init_cmd_size);
#if SOC_LCD_RGB_SUPPORTED && (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(5, 0, 0))
    void enableAutoReleaseBus(void);
#endif

    void begin(void);
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

    int getColorBits(void);
    int getColorBytes(void);
    esp_lcd_panel_handle_t getHandle(void);
    ESP_PanelBus *getBus(void);

    virtual void init(void) = 0;
    virtual ~ESP_PanelLcd() = default;

protected:
    ESP_PanelBus *bus;
    esp_lcd_panel_dev_config_t panel_config;
    lcd_vendor_config_t vendor_config;
    esp_lcd_panel_handle_t handle;

private:
#if SOC_LCD_RGB_SUPPORTED && (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(5, 0, 0))
    void generateRGBConfig(void);
    void attachRGBEventCallback(void);
#endif
};

#endif
