/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * @file   esp_lcd_touch_cst9217.c
 * @brief  ESP LCD touch: CST9217
  */

#include "../esp_panel_touch_conf_internal.h"
#if ESP_PANEL_DRIVERS_TOUCH_ENABLE_CST9217

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_touch.h"

#include "utils/esp_panel_utils_log.h"
#include "esp_utils_helpers.h"
#include "esp_lcd_touch_cst9217.h"

#define CST9217_POINT_NUM_MAX	(1)
#define CST9217_DATA_LENGTH 	(CST9217_POINT_NUM_MAX * 5 + 5)

#define CST9217_REG_DATA		(0xD000)
#define CST9217_REG_CMDMODE 	(0xD101)
#define CST9217_REG_CHECKCODE  	(0xD1FC)
#define CST9217_REG_RESOLUTION 	(0xD1F8)
#define CST9217_REG_PROJECT_ID 	(0xD204)

#define CST9217_ACK 			(0xAB)

static const char *TAG = "CST9217";

static esp_err_t cst9217_read_data(esp_lcd_touch_handle_t tp);
static bool cst9217_get_xy(esp_lcd_touch_handle_t tp, uint16_t *x, uint16_t *y, uint16_t *strength, uint8_t *point_num, uint8_t max_point_num);
static esp_err_t cst9217_del(esp_lcd_touch_handle_t tp);
static esp_err_t cst9217_reset(esp_lcd_touch_handle_t tp);
static esp_err_t cst9217_read_config(esp_lcd_touch_handle_t tp);
static esp_err_t i2c_read_bytes(esp_lcd_touch_handle_t tp, uint16_t reg, uint8_t *data, uint8_t len);

static esp_err_t cst9217_read_register(esp_lcd_touch_handle_t tp, uint16_t reg, uint8_t *data, size_t len);
static esp_err_t cst9217_write_register(esp_lcd_touch_handle_t tp, uint16_t reg, uint8_t *data, size_t len);


esp_err_t esp_lcd_touch_new_i2c_cst9217(const esp_lcd_panel_io_handle_t io, const esp_lcd_touch_config_t *config, esp_lcd_touch_handle_t *tp)
{
    ESP_LOGI(TAG, "version: %d.%d.%d", ESP_LCD_TOUCH_CST9217_VER_MAJOR, ESP_LCD_TOUCH_CST9217_VER_MINOR, ESP_LCD_TOUCH_CST9217_VER_PATCH);
    ESP_RETURN_ON_FALSE(io, ESP_ERR_INVALID_ARG, TAG, "Invalid io");
    ESP_RETURN_ON_FALSE(config, ESP_ERR_INVALID_ARG, TAG, "Invalid config");
    ESP_RETURN_ON_FALSE(tp, ESP_ERR_INVALID_ARG, TAG, "Invalid touch handle");

    /* Prepare main structure */
    esp_err_t ret = ESP_OK;
    esp_lcd_touch_handle_t cst9217 = calloc(1, sizeof(esp_lcd_touch_t));
    ESP_GOTO_ON_FALSE(cst9217, ESP_ERR_NO_MEM, err, TAG, "Touch handle malloc failed");

    /* Communication interface */
    cst9217->io = io;
    /* Only supported callbacks are set */
    cst9217->read_data = cst9217_read_data;
    cst9217->get_xy = cst9217_get_xy;
    cst9217->del = cst9217_del;
    /* Mutex */
    cst9217->data.lock.owner = portMUX_FREE_VAL;
    /* Save config */
    memcpy(&cst9217->config, config, sizeof(esp_lcd_touch_config_t));

    /* Prepare pin for touch controller reset */
    if (cst9217->config.rst_gpio_num != GPIO_NUM_NC) {
        const gpio_config_t rst_gpio_config = {
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = BIT64(cst9217->config.rst_gpio_num)
        };
        ESP_GOTO_ON_ERROR(gpio_config(&rst_gpio_config), err, TAG, "GPIO reset config failed");
    }
    /* Reset controller */
    ESP_GOTO_ON_ERROR(cst9217_reset(cst9217), err, TAG, "Reset failed");
    /* Read config */
    ESP_GOTO_ON_ERROR(cst9217_read_config(cst9217), err, TAG, "Read config failed");
    *tp = cst9217;

    return ESP_OK;
err:
    if (cst9217) {
        cst9217_del(cst9217);
    }
    ESP_LOGE(TAG, "Initialization failed!");
    return ret;
}

static esp_err_t cst9217_read_data(esp_lcd_touch_handle_t tp)
{
	uint8_t data[CST9217_DATA_LENGTH] = {0};
    esp_err_t ret = ESP_OK;

	ESP_RETURN_ON_ERROR(i2c_read_bytes(tp, CST9217_REG_DATA, (uint8_t *)data, sizeof(data)), TAG, "I2C read failed");
	 
	if (data[6] != CST9217_ACK) {
        ESP_LOGE(TAG, "Invalid response when requesting data");
        return ESP_ERR_INVALID_RESPONSE;
    }

	uint8_t points = data[5] & 0x7F;
	if (points > CST9217_POINT_NUM_MAX)
		points = CST9217_POINT_NUM_MAX;

	portENTER_CRITICAL(&tp->data.lock);
	 
	tp->data.points = 0;
	for (int i = 0; i < points; i++) {
		uint8_t *p = &data[i * 5 + (i ? 2 : 0)];
		uint8_t status = p[0] & 0x0F;

		if (status == 0x06) {
			tp->data.coords[i].x = ((p[1] << 4) | (p[3] >> 4));
			tp->data.coords[i].y = ((p[2] << 4) | (p[3] & 0x0F));
			tp->data.points++;

			ESP_LOGV(TAG, "Touch point %d: (%d, %d)", i, tp->data.coords[i].x, tp->data.coords[i].y);
		}
	}

	portEXIT_CRITICAL(&tp->data.lock);

	return ret;

	err:
    if (tp->config.rst_gpio_num != GPIO_NUM_NC) {
        gpio_set_level(tp->config.rst_gpio_num, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(tp->config.rst_gpio_num, 1);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    return ESP_FAIL;
}

static bool cst9217_get_xy(esp_lcd_touch_handle_t tp, uint16_t *x, uint16_t *y, uint16_t *strength, uint8_t *point_num, uint8_t max_point_num)
{
	assert(tp && x && y && point_num);

    portENTER_CRITICAL(&tp->data.lock);
    /* Count of points */
    *point_num = (tp->data.points > max_point_num ? max_point_num : tp->data.points);
    for (size_t i = 0; i < *point_num; i++) {
        x[i] = tp->data.coords[i].x;
        y[i] = tp->data.coords[i].y;

        if (strength) {
            strength[i] = 1; //Not supported
        }
    }
    /* Invalidate */
    tp->data.points = 0;
    portEXIT_CRITICAL(&tp->data.lock);

    return (*point_num > 0);
}

static esp_err_t cst9217_del(esp_lcd_touch_handle_t tp)
{
    /* Reset GPIO pin settings */
    if (tp->config.int_gpio_num != GPIO_NUM_NC) {
        gpio_reset_pin(tp->config.int_gpio_num);
    }
    if (tp->config.rst_gpio_num != GPIO_NUM_NC) {
        gpio_reset_pin(tp->config.rst_gpio_num);
    }
    /* Release memory */
    free(tp);

    return ESP_OK;
}

static esp_err_t cst9217_reset(esp_lcd_touch_handle_t tp)
{
    if (tp->config.rst_gpio_num != GPIO_NUM_NC) {
        ESP_RETURN_ON_ERROR(gpio_set_level(tp->config.rst_gpio_num, tp->config.levels.reset), TAG, "GPIO set level failed");
        vTaskDelay(pdMS_TO_TICKS(10));
        ESP_RETURN_ON_ERROR(gpio_set_level(tp->config.rst_gpio_num, !tp->config.levels.reset), TAG, "GPIO set level failed");
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    return ESP_OK;
}

static esp_err_t cst9217_read_config(esp_lcd_touch_handle_t tp)
{
	uint8_t data[4] = {0};
    esp_err_t ret = ESP_OK;

	uint8_t cmd_mode[2] = {0xD1, 0x01};
    ESP_RETURN_ON_ERROR( cst9217_write_register(tp, CST9217_REG_CMDMODE, cmd_mode, sizeof(cmd_mode)), TAG, "Enter command mode failed");
    vTaskDelay(pdMS_TO_TICKS(10));

	ESP_RETURN_ON_ERROR( cst9217_read_register(tp, CST9217_REG_CHECKCODE, data, 4), TAG, "Read checkcode failed");
    ESP_LOGI(TAG, "Checkcode: 0x%02X%02X%02X%02X", data[0], data[1], data[2], data[3]);

    ESP_RETURN_ON_ERROR( cst9217_read_register(tp, CST9217_REG_RESOLUTION, data, 4), TAG, "Read resolution failed");
    uint16_t res_x = (data[1] << 8) | data[0];
    uint16_t res_y = (data[3] << 8) | data[2];
    ESP_LOGI(TAG, "Resolution X: %d, Y: %d", res_x, res_y);

    ESP_RETURN_ON_ERROR( cst9217_read_register(tp, CST9217_REG_PROJECT_ID, data, 4), TAG, "Read project ID failed");
    uint16_t chipType = (data[3] << 8) | data[2];
    uint32_t projectID = (data[1] << 8) | data[0];
    ESP_LOGI(TAG, "Chip Type: 0x%04X, ProjectID: 0x%04lX", chipType, projectID);

	return ret;
}

static esp_err_t i2c_read_bytes(esp_lcd_touch_handle_t tp, uint16_t reg, uint8_t *data, uint8_t len)
{
    ESP_RETURN_ON_FALSE(data, ESP_ERR_INVALID_ARG, TAG, "Invalid data");

    return esp_lcd_panel_io_rx_param(tp->io, reg, data, len);
}

static esp_err_t cst9217_read_register(esp_lcd_touch_handle_t tp, uint16_t reg, uint8_t *data, size_t len)
{
    uint8_t reg_buf[2] = {reg >> 8, reg & 0xFF};
    const int max_retries = 5;
    esp_err_t ret;

    for (int retry = 0; retry < max_retries; retry++) {
        ret = esp_lcd_panel_io_tx_param(tp->io, reg_buf[0], &reg_buf[1], 1);
        if (ret != ESP_OK) {
            ESP_LOGD(TAG, "TX failed, retry %d", retry);
            vTaskDelay(pdMS_TO_TICKS(3));
            continue;
        }

        vTaskDelay(pdMS_TO_TICKS(2));

        ret = esp_lcd_panel_io_rx_param(tp->io, -1, data, len);
        if (ret == ESP_OK) {
            return ESP_OK;
        }
        ESP_LOGD(TAG, "RX failed, retry %d", retry);
        vTaskDelay(pdMS_TO_TICKS(3));
    }

    if (tp->config.rst_gpio_num != GPIO_NUM_NC) {
        ESP_LOGW(TAG, "Trigger hardware reset");
        gpio_set_level(tp->config.rst_gpio_num, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(tp->config.rst_gpio_num, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    return ESP_FAIL;
}

static esp_err_t cst9217_write_register(esp_lcd_touch_handle_t tp, uint16_t reg, uint8_t *data, size_t len)
{
    uint8_t reg_buf[2] = {reg >> 8, reg & 0xFF};
    const int max_retries = 5;
    esp_err_t ret;

    for (int retry = 0; retry < max_retries; retry++) {
        ret = esp_lcd_panel_io_tx_param(tp->io, reg_buf[0], &reg_buf[1], 1);
        if (ret != ESP_OK) {
            ESP_LOGD(TAG, "Addr TX failed, retry %d", retry);
            vTaskDelay(pdMS_TO_TICKS(3));
            continue;
        }

        vTaskDelay(pdMS_TO_TICKS(2));

        ret = esp_lcd_panel_io_tx_param(tp->io, data[0], &data[1], len-1);
        if (ret == ESP_OK) {
            return ESP_OK;
        }
        ESP_LOGD(TAG, "Data TX failed, retry %d", retry);
        vTaskDelay(pdMS_TO_TICKS(3));
    }
    return ESP_FAIL;
}

#endif // ESP_PANEL_DRIVERS_TOUCH_ENABLE_CST9217
