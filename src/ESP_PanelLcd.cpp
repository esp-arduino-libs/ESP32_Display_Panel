/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_lcd_panel_ops.h"

#include "private/CheckResult.h"
#include "ESP_PanelLcd.h"

#define LCD_DEVICE_CONFIG_DEFAULT()             \
    {                                           \
        .reset_gpio_num = -1,                   \
        .color_space = ESP_LCD_COLOR_SPACE_RGB, \
        .bits_per_pixel = 16,                   \
    }
#define LCD_CONFIG_DEFAULT()                        \
    {                                               \
        .dev_config = LCD_DEVICE_CONFIG_DEFAULT(),  \
    }

static const char *TAG = "ESP_PanelLcd";

ESP_PanelLcd::ESP_PanelLcd(ESP_PanelBus *bus, const ESP_PanelLcdConfig_t *config):
    bus(bus),
    config(*config),
    handle(NULL)
{
}

ESP_PanelLcd::ESP_PanelLcd(ESP_PanelBus *bus):
    bus(bus),
    config(LCD_CONFIG_DEFAULT()),
    handle(NULL)
{
}

void ESP_PanelLcd::begin(void)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_init(handle));
}

void ESP_PanelLcd::reset(void)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_reset(handle));
}

void ESP_PanelLcd::del(void)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_del(handle));
    handle = NULL;
}

void ESP_PanelLcd::drawBitmap(int x_start, int y_start, int x_end, int y_end, const void *color_data)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_draw_bitmap(handle, x_start, y_start, x_end, y_end, color_data));
}

void ESP_PanelLcd::mirror(bool mirror_x, bool mirror_y)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_mirror(handle, mirror_x, mirror_y));
}

void ESP_PanelLcd::swapAxes(bool en)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_swap_xy(handle, en));
}

void ESP_PanelLcd::setGap(int x_gap, int y_gap)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_set_gap(handle, x_gap, y_gap));
}

void ESP_PanelLcd::invertColor(bool en)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_invert_color(handle, en));
}

void ESP_PanelLcd::displayOn(void)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_disp_off(handle, false));
}

void ESP_PanelLcd::displayOff(void)
{
    CHECK_ERROR_RETURN(esp_lcd_panel_disp_off(handle, true));
}

esp_lcd_panel_handle_t ESP_PanelLcd::getHandle(void)
{
    CHECK_NULL_GOTO(handle, err);
    return handle;

err:
    return NULL;
}

ESP_PanelBus *ESP_PanelLcd::getBus(void)
{
    CHECK_NULL_GOTO(bus, err);
    return bus;

err:
    return NULL;
}

void ESP_PanelLcd::setCallback(ESP_PanelBusCallback_t cbs, void *user_ctx)
{
    CHECK_NULL_RETURN(bus);
    bus->setCallback(cbs, user_ctx);
}
