/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../esp_panel_lcd_conf_internal.h"
#if ESP_PANEL_DRIVERS_LCD_ENABLE_OTA7290

#include "soc/soc_caps.h"

#if SOC_MIPI_DSI_SUPPORTED
#include "esp_check.h"
#include "esp_log.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_lcd_panel_vendor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_lcd_ota7290.h"

#include "utils/esp_panel_utils_log.h"
#include "esp_utils_helpers.h"
#include "esp_panel_lcd_vendor_types.h"

#define OTA7290_CMD_DSI_INT0     (0x14)
#define OTA7290_DSI_2_LANE       (0xE1)
#define OTA7290_DSI_3_LANE       (0xE2)
#define OTA7290_DSI_4_LANE       (0xE3)

typedef struct {
    esp_lcd_panel_io_handle_t io;
    int reset_gpio_num;
    uint8_t madctl_val; // save current value of LCD_CMD_MADCTL register
    uint8_t colmod_val; // save surrent value of LCD_CMD_COLMOD register
    const esp_panel_lcd_vendor_init_cmd_t *init_cmds;
    uint16_t init_cmds_size;
    uint8_t lane_num;
    struct {
        unsigned int reset_level: 1;
    } flags;
    // To save the original functions of MIPI DPI panel
    esp_err_t (*del)(esp_lcd_panel_t *panel);
    esp_err_t (*init)(esp_lcd_panel_t *panel);
} ota7290_panel_t;

static const char *TAG = "ota7290";

static esp_err_t panel_ota7290_del(esp_lcd_panel_t *panel);
static esp_err_t panel_ota7290_init(esp_lcd_panel_t *panel);
static esp_err_t panel_ota7290_reset(esp_lcd_panel_t *panel);
static esp_err_t panel_ota7290_disp_on_off(esp_lcd_panel_t *panel, bool on_off);

esp_err_t esp_lcd_new_panel_ota7290(const esp_lcd_panel_io_handle_t io, const esp_lcd_panel_dev_config_t *panel_dev_config,
                                    esp_lcd_panel_handle_t *ret_panel)
{
    ESP_LOGI(TAG, "version: %d.%d.%d", ESP_LCD_OTA7290_VER_MAJOR, ESP_LCD_OTA7290_VER_MINOR, ESP_LCD_OTA7290_VER_PATCH);
    ESP_RETURN_ON_FALSE(io && panel_dev_config && ret_panel, ESP_ERR_INVALID_ARG, TAG, "invalid arguments");
    esp_panel_lcd_vendor_config_t *vendor_config = (esp_panel_lcd_vendor_config_t *)panel_dev_config->vendor_config;
    ESP_RETURN_ON_FALSE(vendor_config && vendor_config->mipi_config.dpi_config && vendor_config->mipi_config.dsi_bus, ESP_ERR_INVALID_ARG, TAG,
                        "invalid vendor config");

    esp_err_t ret = ESP_OK;
    ota7290_panel_t *ota7290 = (ota7290_panel_t *)calloc(1, sizeof(ota7290_panel_t));
    ESP_RETURN_ON_FALSE(ota7290, ESP_ERR_NO_MEM, TAG, "no mem for ota7290 panel");

    if (panel_dev_config->reset_gpio_num >= 0) {
        gpio_config_t io_conf = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = 1ULL << panel_dev_config->reset_gpio_num,
        };
        ESP_GOTO_ON_ERROR(gpio_config(&io_conf), err, TAG, "configure GPIO for RST line failed");
    }

    switch (panel_dev_config->color_space) {
    case LCD_RGB_ELEMENT_ORDER_RGB:
        ota7290->madctl_val = 0;
        break;
    case LCD_RGB_ELEMENT_ORDER_BGR:
        ota7290->madctl_val |= LCD_CMD_BGR_BIT;
        break;
    default:
        ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG, "unsupported color space");
        break;
    }

    switch (panel_dev_config->bits_per_pixel) {
    case 16: // RGB565
        ota7290->colmod_val = 0x55;
        break;
    case 18: // RGB666
        ota7290->colmod_val = 0x66;
        break;
    case 24: // RGB888
        ota7290->colmod_val = 0x77;
        break;
    default:
        ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG, "unsupported pixel width");
        break;
    }

    // uint8_t ID[3];
    // ESP_GOTO_ON_ERROR(esp_lcd_panel_io_rx_param(io, 0x04, ID, 3), err, TAG, "read ID failed");
    // ESP_LOGI(TAG, "LCD ID: %02X %02X %02X", ID[0], ID[1], ID[2]);

    ota7290->io = io;
    ota7290->init_cmds = vendor_config->init_cmds;
    ota7290->init_cmds_size = vendor_config->init_cmds_size;
    ota7290->lane_num = vendor_config->mipi_config.lane_num;
    ota7290->reset_gpio_num = panel_dev_config->reset_gpio_num;
    ota7290->flags.reset_level = panel_dev_config->flags.reset_active_high;

    // Create MIPI DPI panel
    esp_lcd_panel_handle_t panel_handle = NULL;
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_dpi(vendor_config->mipi_config.dsi_bus, vendor_config->mipi_config.dpi_config, &panel_handle), err, TAG,
                      "create MIPI DPI panel failed");
    ESP_LOGD(TAG, "new MIPI DPI panel @%p", panel_handle);

    // Save the original functions of MIPI DPI panel
    ota7290->del = panel_handle->del;
    ota7290->init = panel_handle->init;
    // Overwrite the functions of MIPI DPI panel
    panel_handle->del = panel_ota7290_del;
    panel_handle->init = panel_ota7290_init;
    panel_handle->reset = panel_ota7290_reset;
    panel_handle->disp_on_off = panel_ota7290_disp_on_off;
    panel_handle->user_data = ota7290;
    *ret_panel = panel_handle;
    ESP_LOGD(TAG, "new ota7290 panel @%p", ota7290);

    return ESP_OK;

err:
    if (ota7290) {
        if (panel_dev_config->reset_gpio_num >= 0) {
            gpio_reset_pin(panel_dev_config->reset_gpio_num);
        }
        free(ota7290);
    }
    return ret;
}

static const esp_panel_lcd_vendor_init_cmd_t vendor_specific_init_default[] = {
//  {cmd, { data }, data_size, delay_ms}
    // MIPI  In To  LP Mode
    {0xB0, (uint8_t []){0x5A}, 1, 0},
    // STANDBY Mode
    {0xB1, (uint8_t []){0x00}, 1, 0},
    {0x89, (uint8_t []){0x01}, 1, 0},
    {0x91, (uint8_t []){0x17}, 1, 0},
    // BLANK 03 Memory
    {0xB1, (uint8_t []){0x03}, 1, 0},
    {0x2C, (uint8_t []){0x28}, 1, 0},

    {0x00, (uint8_t []){0xDF}, 1, 0},
    {0x01, (uint8_t []){0xEF}, 1, 0},
    {0x02, (uint8_t []){0xF7}, 1, 0},
    {0x03, (uint8_t []){0xFB}, 1, 0},
    {0x04, (uint8_t []){0xFD}, 1, 0},
    {0x05, (uint8_t []){0x00}, 1, 0},
    {0x06, (uint8_t []){0x00}, 1, 0},
    {0x07, (uint8_t []){0x00}, 1, 0},
    {0x08, (uint8_t []){0x00}, 1, 0},
    {0x09, (uint8_t []){0x00}, 1, 0},
    {0x0A, (uint8_t []){0x01}, 1, 0},
    {0x0B, (uint8_t []){0x3B}, 1, 0},
    {0x0C, (uint8_t []){0x00}, 1, 0},
    {0x0D, (uint8_t []){0x00}, 1, 0},
    {0x0E, (uint8_t []){0x24}, 1, 0},
    {0x0F, (uint8_t []){0x1C}, 1, 0},
    {0x10, (uint8_t []){0xC9}, 1, 0},
    {0x11, (uint8_t []){0x60}, 1, 0},
    {0x12, (uint8_t []){0x70}, 1, 0},
    {0x13, (uint8_t []){0x01}, 1, 0},
    {0x14, (uint8_t []){0xe1}, 1, 0},
    {0x15, (uint8_t []){0xFF}, 1, 0},
    {0x16, (uint8_t []){0x3D}, 1, 0},
    {0x17, (uint8_t []){0x0E}, 1, 0},
    {0x18, (uint8_t []){0x01}, 1, 0},
    {0x19, (uint8_t []){0x00}, 1, 0},
    {0x1A, (uint8_t []){0x00}, 1, 0},
    {0x1B, (uint8_t []){0xFC}, 1, 0},
    {0x1C, (uint8_t []){0x0B}, 1, 0},
    {0x1D, (uint8_t []){0xA0}, 1, 0},
    {0x1E, (uint8_t []){0x03}, 1, 0},
    {0x1F, (uint8_t []){0x04}, 1, 0},
    {0x20, (uint8_t []){0x0C}, 1, 0},
    {0x21, (uint8_t []){0x00}, 1, 0},
    {0x22, (uint8_t []){0x04}, 1, 0},
    {0x23, (uint8_t []){0x81}, 1, 0},
    {0x24, (uint8_t []){0x1F}, 1, 0},
    {0x25, (uint8_t []){0x10}, 1, 0},
    {0x26, (uint8_t []){0x9B}, 1, 0},
    {0x2D, (uint8_t []){0x01}, 1, 0},
    {0x2E, (uint8_t []){0x84}, 1, 0},
    {0x2F, (uint8_t []){0x00}, 1, 0},
    {0x30, (uint8_t []){0x02}, 1, 0},
    {0x31, (uint8_t []){0x08}, 1, 0},
    {0x32, (uint8_t []){0x01}, 1, 0},
    {0x33, (uint8_t []){0x1C}, 1, 0},
    {0x34, (uint8_t []){0x70}, 1, 0},
    {0x35, (uint8_t []){0xFF}, 1, 0},
    {0x36, (uint8_t []){0xFF}, 1, 0},
    {0x37, (uint8_t []){0xFF}, 1, 0},
    {0x38, (uint8_t []){0xFF}, 1, 0},
    {0x39, (uint8_t []){0xFF}, 1, 0},
    {0x3A, (uint8_t []){0x05}, 1, 0},
    {0x3B, (uint8_t []){0x00}, 1, 0},
    {0x3C, (uint8_t []){0x00}, 1, 0},
    {0x3D, (uint8_t []){0x00}, 1, 0},
    {0x3E, (uint8_t []){0x0F}, 1, 0},
    {0x3F, (uint8_t []){0x84}, 1, 0},
    {0x40, (uint8_t []){0x2A}, 1, 0},
    {0x41, (uint8_t []){0x00}, 1, 0},
    {0x42, (uint8_t []){0x01}, 1, 0},
    {0x43, (uint8_t []){0x40}, 1, 0},
    {0x44, (uint8_t []){0x05}, 1, 0},
    {0x45, (uint8_t []){0xE8}, 1, 0},
    {0x46, (uint8_t []){0x16}, 1, 0},
    {0x47, (uint8_t []){0x00}, 1, 0},
    {0x48, (uint8_t []){0x00}, 1, 0},
    {0x49, (uint8_t []){0x88}, 1, 0},
    {0x4A, (uint8_t []){0x08}, 1, 0},
    {0x4B, (uint8_t []){0x05}, 1, 0},
    {0x4C, (uint8_t []){0x03}, 1, 0},
    {0x4D, (uint8_t []){0xD0}, 1, 0},
    {0x4E, (uint8_t []){0x13}, 1, 0},
    {0x4F, (uint8_t []){0xFF}, 1, 0},
    {0x50, (uint8_t []){0x0A}, 1, 0},
    {0x51, (uint8_t []){0x53}, 1, 0},
    {0x52, (uint8_t []){0x26}, 1, 0},
    {0x53, (uint8_t []){0x22}, 1, 0},
    {0x54, (uint8_t []){0x09}, 1, 0},
    {0x55, (uint8_t []){0x22}, 1, 0},
    {0x56, (uint8_t []){0x00}, 1, 0},
    {0x57, (uint8_t []){0x1C}, 1, 0},
    {0x58, (uint8_t []){0x03}, 1, 0},
    {0x59, (uint8_t []){0x3F}, 1, 0},
    {0x5A, (uint8_t []){0x28}, 1, 0},
    {0x5B, (uint8_t []){0x01}, 1, 0},
    {0x5C, (uint8_t []){0xCC}, 1, 0},
    {0x5D, (uint8_t []){0x21}, 1, 0},
    {0x5E, (uint8_t []){0x04}, 1, 0},
    {0x5F, (uint8_t []){0x13}, 1, 0},
    {0x60, (uint8_t []){0x42}, 1, 0},
    {0x61, (uint8_t []){0x08}, 1, 0},
    {0x62, (uint8_t []){0x64}, 1, 0},
    {0x63, (uint8_t []){0xEB}, 1, 0},
    {0x64, (uint8_t []){0x10}, 1, 0},
    {0x65, (uint8_t []){0xA8}, 1, 0},
    {0x66, (uint8_t []){0x84}, 1, 0},
    {0x67, (uint8_t []){0x8E}, 1, 0},
    {0x68, (uint8_t []){0x29}, 1, 0},
    {0x69, (uint8_t []){0x11}, 1, 0},
    {0x6A, (uint8_t []){0x42}, 1, 0},
    {0x6B, (uint8_t []){0x38}, 1, 0},
    {0x6C, (uint8_t []){0x21}, 1, 0},
    {0x6D, (uint8_t []){0x84}, 1, 0},
    {0x6E, (uint8_t []){0x50}, 1, 0},
    {0x6F, (uint8_t []){0xB6}, 1, 0},
    {0x70, (uint8_t []){0x0E}, 1, 0},
    {0x71, (uint8_t []){0xA1}, 1, 0},
    {0x72, (uint8_t []){0xCE}, 1, 0},
    {0x73, (uint8_t []){0xF8}, 1, 0},
    {0x74, (uint8_t []){0xDA}, 1, 0},
    {0x75, (uint8_t []){0x1A}, 1, 0},
    {0x76, (uint8_t []){0x80}, 1, 0},
    {0x77, (uint8_t []){0x00}, 1, 0},
    {0x78, (uint8_t []){0x5F}, 1, 0},
    {0x79, (uint8_t []){0xE0}, 1, 0},
    {0x7A, (uint8_t []){0x01}, 1, 0},
    {0x7B, (uint8_t []){0xFF}, 1, 0},


    {0x7C, (uint8_t []){0xFF}, 1, 0},
    {0x7D, (uint8_t []){0xFF}, 1, 0},
    {0x7E, (uint8_t []){0xFF}, 1, 0},
    {0x7F, (uint8_t []){0xFE}, 1, 0},

    // BLANK 02 Memory
    {0xB1, (uint8_t []){0x02}, 1, 0},

    {0x00, (uint8_t []){0xFF}, 1, 0},
    {0x01, (uint8_t []){0x01}, 1, 0},
    {0x02, (uint8_t []){0x00}, 1, 0},
    {0x03, (uint8_t []){0x00}, 1, 0},
    {0x04, (uint8_t []){0x00}, 1, 0},
    {0x05, (uint8_t []){0x00}, 1, 0},
    {0x06, (uint8_t []){0x00}, 1, 0},
    {0x07, (uint8_t []){0x00}, 1, 0},
    {0x08, (uint8_t []){0xC0}, 1, 0},
    {0x09, (uint8_t []){0x00}, 1, 0},
    {0x0A, (uint8_t []){0x00}, 1, 0},
    {0x0B, (uint8_t []){0x14}, 1, 0},
    {0x0C, (uint8_t []){0xE6}, 1, 0},
    {0x0D, (uint8_t []){0x0D}, 1, 0},
    {0x0F, (uint8_t []){0x00}, 1, 0},
    //GAMMA
    {0x10, (uint8_t []){0x3F}, 1, 0},
    {0x11, (uint8_t []){0x4D}, 1, 0},
    {0x12, (uint8_t []){0x37}, 1, 0},
    {0x13, (uint8_t []){0x6F}, 1, 0},
    {0x14, (uint8_t []){0x73}, 1, 0},
    {0x15, (uint8_t []){0xDF}, 1, 0},
    {0x16, (uint8_t []){0xE7}, 1, 0},
    {0x17, (uint8_t []){0xB5}, 1, 0},
    {0x18, (uint8_t []){0xD5}, 1, 0},
    {0x19, (uint8_t []){0xE9}, 1, 0},
    {0x1A, (uint8_t []){0xF4}, 1, 0},
    {0x1B, (uint8_t []){0x0F}, 1, 0},

    {0x1C, (uint8_t []){0xFF}, 1, 0},
    {0x1D, (uint8_t []){0xFF}, 1, 0},
    {0x1E, (uint8_t []){0xFF}, 1, 0},
    {0x1F, (uint8_t []){0xFF}, 1, 0},
    {0x20, (uint8_t []){0xFF}, 1, 0},
    {0x21, (uint8_t []){0xFF}, 1, 0},
    {0x22, (uint8_t []){0xFF}, 1, 0},
    {0x23, (uint8_t []){0xFF}, 1, 0},
    {0x24, (uint8_t []){0xFF}, 1, 0},
    {0x25, (uint8_t []){0xFF}, 1, 0},
    {0x26, (uint8_t []){0xFF}, 1, 0},
    {0x27, (uint8_t []){0x1F}, 1, 0},
    //VCOM
    {0x28, (uint8_t []){0xF0}, 1, 0},
    {0x29, (uint8_t []){0xFF}, 1, 0},
    {0x2A, (uint8_t []){0xFF}, 1, 0},
    {0x2B, (uint8_t []){0xFF}, 1, 0},

    {0x2C, (uint8_t []){0xFF}, 1, 0},
    {0x2D, (uint8_t []){0x07}, 1, 0},
    {0x2F, (uint8_t []){0xFF}, 1, 0},
    {0x30, (uint8_t []){0x7F}, 1, 0},
    {0x31, (uint8_t []){0x91}, 1, 0},
    {0x32, (uint8_t []){0x00}, 1, 0},
    {0x33, (uint8_t []){0x08}, 1, 0},
    {0x35, (uint8_t []){0x7F}, 1, 0},
    {0x36, (uint8_t []){0x05}, 1, 0},
    {0x38, (uint8_t []){0x7F}, 1, 0},
    {0x3A, (uint8_t []){0x80}, 1, 0},
    {0x3B, (uint8_t []){0x01}, 1, 0},
    {0x3C, (uint8_t []){0xC0}, 1, 0},
    {0x3D, (uint8_t []){0x32}, 1, 0},
    {0x3E, (uint8_t []){0x00}, 1, 0},
    {0x3F, (uint8_t []){0x58}, 1, 0},
    {0x40, (uint8_t []){0x06}, 1, 0},
    {0x41, (uint8_t []){0x00}, 1, 0},
    {0x42, (uint8_t []){0xCB}, 1, 0},
    {0x43, (uint8_t []){0x00}, 1, 0},
    {0x44, (uint8_t []){0x60}, 1, 0},
    {0x45, (uint8_t []){0x09}, 1, 0},
    {0x46, (uint8_t []){0x00}, 1, 0},
    {0x47, (uint8_t []){0x00}, 1, 0},
    {0x48, (uint8_t []){0x8B}, 1, 0},
    {0x49, (uint8_t []){0xD2}, 1, 0},
    {0x4A, (uint8_t []){0x01}, 1, 0},
    {0x4B, (uint8_t []){0x00}, 1, 0},
    {0x4C, (uint8_t []){0x10}, 1, 0},
    {0x4D, (uint8_t []){0x40}, 1, 0},
    {0x4E, (uint8_t []){0x0D}, 1, 0},
    {0x4F, (uint8_t []){0x61}, 1, 0},
    {0x50, (uint8_t []){0x3C}, 1, 0},
    {0x51, (uint8_t []){0x7A}, 1, 0},
    {0x52, (uint8_t []){0x34}, 1, 0},
    {0x53, (uint8_t []){0x99}, 1, 0},
    {0x54, (uint8_t []){0xA2}, 1, 0},
    {0x55, (uint8_t []){0x03}, 1, 0},
    {0x56, (uint8_t []){0x6C}, 1, 0},
    {0x57, (uint8_t []){0x1A}, 1, 0},
    {0x58, (uint8_t []){0x05}, 1, 0},
    {0x59, (uint8_t []){0xF0}, 1, 0},
    {0x5A, (uint8_t []){0xFB}, 1, 0},
    {0x5B, (uint8_t []){0xFD}, 1, 0},
    {0x5C, (uint8_t []){0x7E}, 1, 0},
    {0x5D, (uint8_t []){0xBF}, 1, 0},
    {0x5E, (uint8_t []){0x1F}, 1, 0},
    {0x5F, (uint8_t []){0x00}, 1, 0},
    {0x60, (uint8_t []){0xF0}, 1, 0},
    {0x61, (uint8_t []){0xF3}, 1, 0},
    {0x62, (uint8_t []){0xFB}, 1, 0},
    {0x63, (uint8_t []){0xF9}, 1, 0},
    {0x64, (uint8_t []){0xFD}, 1, 0},
    {0x65, (uint8_t []){0x7E}, 1, 0},
    {0x66, (uint8_t []){0x00}, 1, 0},
    {0x67, (uint8_t []){0x00}, 1, 0},
    {0x68, (uint8_t []){0x14}, 1, 0},
    {0x69, (uint8_t []){0x89}, 1, 0},
    {0x6A, (uint8_t []){0x70}, 1, 0},
    {0x6B, (uint8_t []){0xFC}, 1, 0},
    {0x6C, (uint8_t []){0xFC}, 1, 0},
    {0x6D, (uint8_t []){0xFC}, 1, 0},
    {0x6E, (uint8_t []){0xFC}, 1, 0},
    {0x6F, (uint8_t []){0xFC}, 1, 0},
    {0x70, (uint8_t []){0x7E}, 1, 0},
    {0x71, (uint8_t []){0xBF}, 1, 0},
    {0x72, (uint8_t []){0xDF}, 1, 0},
    {0x73, (uint8_t []){0xCF}, 1, 0},
    {0x74, (uint8_t []){0xCF}, 1, 0},
    {0x75, (uint8_t []){0xCF}, 1, 0},
    {0x76, (uint8_t []){0x0F}, 1, 0},
    {0x77, (uint8_t []){0x00}, 1, 0},
    {0x78, (uint8_t []){0x00}, 1, 0},
    {0x79, (uint8_t []){0x00}, 1, 0},
    {0x7A, (uint8_t []){0x7E}, 1, 0},
    {0x7B, (uint8_t []){0x7E}, 1, 0},
    {0x7C, (uint8_t []){0x7E}, 1, 0},
    {0x7D, (uint8_t []){0x7E}, 1, 0},
    {0x7E, (uint8_t []){0x7E}, 1, 0},
    {0x7F, (uint8_t []){0xBF}, 1, 0},
    {0x0B, (uint8_t []){0x04}, 1, 0},

    {0xB1, (uint8_t []){0x03}, 1, 0},
    {0x2C, (uint8_t []){0x2C}, 1, 0},

    // Nornmal Mode
    {0xB1, (uint8_t []){0x00}, 1, 0},
    {0x89, (uint8_t []){0x03}, 1, 0},
};

static esp_err_t panel_ota7290_del(esp_lcd_panel_t *panel)
{
    ota7290_panel_t *ota7290 = (ota7290_panel_t *)panel->user_data;

    if (ota7290->reset_gpio_num >= 0) {
        gpio_reset_pin(ota7290->reset_gpio_num);
    }
    // Delete MIPI DPI panel
    ota7290->del(panel);
    free(ota7290);
    ESP_LOGD(TAG, "del ota7290 panel @%p", ota7290);

    return ESP_OK;
}

static esp_err_t panel_ota7290_init(esp_lcd_panel_t *panel)
{
    ota7290_panel_t *ota7290 = (ota7290_panel_t *)panel->user_data;
    esp_lcd_panel_io_handle_t io = ota7290->io;
    const esp_panel_lcd_vendor_init_cmd_t *init_cmds = NULL;
    uint16_t init_cmds_size = 0;
    uint8_t lane_command = OTA7290_DSI_2_LANE;
    bool is_user_set = true;
    bool is_cmd_overwritten = false;

    switch (ota7290->lane_num) {
    case 0:
    case 2:
        lane_command = OTA7290_DSI_2_LANE;
        break;
    case 3:
        lane_command = OTA7290_DSI_3_LANE;
        break;
    case 4:
        lane_command = OTA7290_DSI_4_LANE;
        break;
    default:
        ESP_LOGE(TAG, "Invalid lane number %d", ota7290->lane_num);
        return ESP_ERR_INVALID_ARG;
    }

    // ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, OTA7290_CMD_PAGE, (uint8_t[]) {
    //     OTA7290_PAGE_USER
    // }, 1), TAG, "send command failed");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, LCD_CMD_MADCTL, (uint8_t[]) {
        ota7290->madctl_val,
    }, 1), TAG, "send command failed");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, LCD_CMD_COLMOD, (uint8_t[]) {
        ota7290->colmod_val,
    }, 1), TAG, "send command failed");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, OTA7290_CMD_DSI_INT0, (uint8_t[]) {
        lane_command,
    }, 1), TAG, "send command failed");

    // vendor specific initialization, it can be different between manufacturers
    // should consult the LCD supplier for initialization sequence code
    if (ota7290->init_cmds) {
        init_cmds = ota7290->init_cmds;
        init_cmds_size = ota7290->init_cmds_size;
    } else {
        init_cmds = vendor_specific_init_default;
        init_cmds_size = sizeof(vendor_specific_init_default) / sizeof(esp_panel_lcd_vendor_init_cmd_t);
    }

    for (int i = 0; i < init_cmds_size; i++) {
        // Check if the command has been used or conflicts with the internal
        if (is_user_set && (init_cmds[i].data_bytes > 0)) {
            switch (init_cmds[i].cmd) {
            case LCD_CMD_MADCTL:
                is_cmd_overwritten = true;
                ota7290->madctl_val = ((uint8_t *)init_cmds[i].data)[0];
                break;
            case LCD_CMD_COLMOD:
                is_cmd_overwritten = true;
                ota7290->colmod_val = ((uint8_t *)init_cmds[i].data)[0];
                break;
            default:
                is_cmd_overwritten = false;
                break;
            }

            if (is_cmd_overwritten) {
                is_cmd_overwritten = false;
                ESP_LOGW(TAG, "The %02Xh command has been used and will be overwritten by external initialization sequence",
                         init_cmds[i].cmd);
            }
        }

        // Send command
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, init_cmds[i].cmd, init_cmds[i].data, init_cmds[i].data_bytes), TAG, "send command failed");
        vTaskDelay(pdMS_TO_TICKS(init_cmds[i].delay_ms));

        // Check if the current cmd is the "page set" cmd
        // if ((init_cmds[i].cmd == OTA7290_CMD_PAGE) && (init_cmds[i].data_bytes > 0)) {
        //     is_user_set = (((uint8_t *)init_cmds[i].data)[0] == OTA7290_PAGE_USER);
        // }
    }
    ESP_LOGD(TAG, "send init commands success");

    ESP_RETURN_ON_ERROR(ota7290->init(panel), TAG, "init MIPI DPI panel failed");

    return ESP_OK;
}

static esp_err_t panel_ota7290_reset(esp_lcd_panel_t *panel)
{
    ota7290_panel_t *ota7290 = (ota7290_panel_t *)panel->user_data;
    esp_lcd_panel_io_handle_t io = ota7290->io;

    // Perform hardware reset
    if (ota7290->reset_gpio_num >= 0) {
        gpio_set_level(ota7290->reset_gpio_num, !ota7290->flags.reset_level);
        vTaskDelay(pdMS_TO_TICKS(5));
        gpio_set_level(ota7290->reset_gpio_num, ota7290->flags.reset_level);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(ota7290->reset_gpio_num, !ota7290->flags.reset_level);
        vTaskDelay(pdMS_TO_TICKS(120));
    } else if (io) { // Perform software reset
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, LCD_CMD_SWRESET, NULL, 0), TAG, "send command failed");
        vTaskDelay(pdMS_TO_TICKS(120));
    }

    return ESP_OK;
}

static esp_err_t panel_ota7290_disp_on_off(esp_lcd_panel_t *panel, bool on_off)
{
    ota7290_panel_t *ota7290 = (ota7290_panel_t *)panel->user_data;
    esp_lcd_panel_io_handle_t io = ota7290->io;
    int command = 0;

    if (on_off) {
        command = LCD_CMD_DISPON;
    } else {
        command = LCD_CMD_DISPOFF;
    }
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, command, NULL, 0), TAG, "send command failed");
    return ESP_OK;
}
#endif // SOC_MIPI_DSI_SUPPORTED
#endif // ESP_PANEL_DRIVERS_LCD_ENABLE_OTA7290
