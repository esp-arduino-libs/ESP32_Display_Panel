/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ESP_PanelLog.h"
#include "GT911.h"

static const char *TAG = "GT911_CPP";

ESP_PanelTouch_GT911::ESP_PanelTouch_GT911(ESP_PanelBus *bus, uint16_t width, uint16_t height, int rst_io, int int_io):
    ESP_PanelTouch(bus, width, height, rst_io, int_io)
{
}

ESP_PanelTouch_GT911::ESP_PanelTouch_GT911(ESP_PanelBus *bus, const esp_lcd_touch_config_t &config):
    ESP_PanelTouch(bus, config)
{
}

ESP_PanelTouch_GT911::~ESP_PanelTouch_GT911()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (handle == NULL) {
        goto end;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete device failed");
    }

end:
    ESP_LOGD(TAG, "Destroyed");
}

bool ESP_PanelTouch_GT911::begin(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");

    ESP_PanelBus_I2C *i2c_bus = static_cast<ESP_PanelBus_I2C *>(bus);
    esp_lcd_touch_io_gt911_config_t tp_gt911_config = {
        .dev_addr = i2c_bus->getI2cAddress(),
    };
    if (config.driver_data == NULL) {
        ESP_LOGD(TAG, "Use default GT911 driver data(address: 0x%02x)", tp_gt911_config.dev_addr);
        config.driver_data = (void *)&tp_gt911_config;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_touch_new_i2c_gt911(bus->getPanelIO_Handle(), &config, &handle), false, "New driver failed");

    return true;
}
