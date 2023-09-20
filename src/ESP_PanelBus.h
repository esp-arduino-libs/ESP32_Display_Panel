/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ESP_PANELBUS_H
#define ESP_PANELBUS_H

#include "driver/i2c.h"
#include "driver/spi_master.h"

#include "esp_lcd_panel_io.h"

#define ESP_PANEL_BUS_TYPE_I2C      (0)
#define ESP_PANEL_BUS_TYPE_SPI      (1)
#define ESP_PANEL_BUS_TYPE_I80      (2)
#define ESP_PANEL_BUS_TYPE_RGB      (3)

typedef bool (*ESP_PanelBusCallback_t)(void *user_ctx);

typedef struct {
    void *bus;
    void *user_ctx;
} ESP_CallbackCtx_t;

class ESP_PanelBus {
public:
    ESP_PanelBus();
    virtual ~ESP_PanelBus() = default;

    void readParam(int lcd_cmd, void *param, size_t param_size);
    void writeParam(int lcd_cmd, const void *param, size_t param_size);
    void writeColor(int lcd_cmd, const void *color, size_t color_size);
    void del(void);
    esp_lcd_panel_io_handle_t getHandle(void);
    void setCallback(ESP_PanelBusCallback_t cbs, void *user_ctx);
    virtual void init(void) = 0;

protected:
    esp_lcd_panel_io_handle_t handle;
    ESP_CallbackCtx_t ctx;
    ESP_PanelBusCallback_t cbs;
    struct {
        uint32_t host_need_init: 1;
    } flags;

    static bool callback(void *panel_io, void *edata, void *user_ctx);
};

#endif
