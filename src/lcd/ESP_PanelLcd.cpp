/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cstring"
#include "ESP_PanelLog.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_memory_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "soc/soc_caps.h"
#include "sdkconfig.h"
#include "bus/RGB.h"
#include "bus/ESP_PanelBus.h"
#include "ESP_PanelLcd.h"

#define VENDOR_CONFIG_DEFAULT()      \
    {                                \
        .init_cmds = NULL,           \
        .init_cmds_size = 0,         \
        .flags = {                   \
            .mirror_by_cmd = 1,      \
            .auto_del_panel_io = 0,  \
            .use_qspi_interface = 0, \
        }                            \
    }
#define CALLBACK_DATA_DEFAULT() \
    {                           \
        .lcd_ptr = this,        \
        .user_data = NULL,      \
    }

static const char *TAG = "ESP_PanelLcd";

ESP_PanelLcd::ESP_PanelLcd(ESP_PanelBus *bus, uint8_t color_bits, int rst_io):
    x_coord_align(0),
    y_coord_align(0),
    bus(bus),
    panel_config(ESP_PANEL_LCD_DEVICE_CONFIG_DEFAULT(rst_io, color_bits, &vendor_config)),
    vendor_config(VENDOR_CONFIG_DEFAULT()),
    handle(NULL),
    _swap_xy(false),
    _mirror_x(false),
    _mirror_y(false),
    _gap_x(0),
    _gap_y(0),
    onRefreshFinishCallback(NULL),
    _refresh_finish_sem(NULL),
    callback_data(CALLBACK_DATA_DEFAULT())
{
    switch (bus->getType()) {
    case ESP_PANEL_BUS_TYPE_QSPI:
        vendor_config.flags.use_qspi_interface = 1;
        break;
#if SOC_LCD_RGB_SUPPORTED
    /* Retrieve RGB configuration from the bus and register it into the vendor configuration */
    case ESP_PANEL_BUS_TYPE_RGB:
        vendor_config.rgb_config = static_cast<ESP_PanelBus_RGB *>(bus)->getRgbConfig();
        break;
#endif
    default:
        break;
    }
}

ESP_PanelLcd::ESP_PanelLcd(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config):
    x_coord_align(0),
    y_coord_align(0),
    bus(bus),
    panel_config(panel_config),
    vendor_config(VENDOR_CONFIG_DEFAULT()),
    handle(NULL),
    _swap_xy(false),
    _mirror_x(false),
    _mirror_y(false),
    _gap_x(0),
    _gap_y(0),
    onRefreshFinishCallback(NULL),
    _refresh_finish_sem(NULL),
    callback_data(CALLBACK_DATA_DEFAULT())
{
    /* Save vendor configuration to local and register the local one into panel configuration */
    if (panel_config.vendor_config != NULL) {
        vendor_config = *(esp_lcd_panel_vendor_config_t *)panel_config.vendor_config;
    }
    this->panel_config.vendor_config = &vendor_config;

#if SOC_LCD_RGB_SUPPORTED
    /* Retrieve RGB configuration from the bus and register it into the vendor configuration */
    if (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) {
        const esp_lcd_rgb_panel_config_t *rgb_config = static_cast<ESP_PanelBus_RGB *>(bus)->getRgbConfig();
        vendor_config.rgb_config = rgb_config;
    }
#endif
}

bool ESP_PanelLcd::configVendorCommands(const esp_lcd_panel_vendor_init_cmd_t init_cmd[], uint32_t init_cmd_size)
{
    ESP_PANEL_CHECK_FALSE_RET((init_cmd == NULL) || (init_cmd_size > 0), false, "Size of init commands is invalid");

    vendor_config.init_cmds = init_cmd;
    vendor_config.init_cmds_size = init_cmd_size;

    return true;
}

void ESP_PanelLcd::configColorRgbOrder(bool BGR_order)
{
    if (BGR_order) {
        panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR;
    } else {
        panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
    }
}

void ESP_PanelLcd::configMirrorByCommand(bool en)
{
    vendor_config.flags.mirror_by_cmd = en;
}

void ESP_PanelLcd::configAutoReleaseBus(bool en)
{
    vendor_config.flags.auto_del_panel_io = en;
}

bool ESP_PanelLcd::begin(void)
{
    ESP_PANEL_CHECK_NULL_RET(handle, false, "Invalid handle");
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");

    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    ESP_LOGD(TAG, "Begin start");

    /* Initialize LCD panel */
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_init(handle), false, "Init panel failed");

    /* For non-RGB interface, create Semaphore for using API `drawBitmapWaitUntilFinish()` */
    if (bus->getType() != ESP_PANEL_BUS_TYPE_RGB) {
        _refresh_finish_sem = xSemaphoreCreateBinary();
        ESP_PANEL_CHECK_NULL_RET(_refresh_finish_sem, false, "Create semaphore failed");
    }

    /* Register transimit done callback for non-RGB interface and RGB interface */
    if (bus->getType() != ESP_PANEL_BUS_TYPE_RGB) {
        esp_lcd_panel_io_callbacks_t io_cb = {
            .on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)onRefreshFinish,
        };
        ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_io_register_event_callbacks(bus->getHandle(), &io_cb, &callback_data), false,
                                "Register IO callback failed");
    }
#if SOC_LCD_RGB_SUPPORTED
    else {
        const esp_lcd_rgb_panel_config_t *rgb_config = static_cast<ESP_PanelBus_RGB *>(bus)->getRgbConfig();
        esp_lcd_rgb_panel_event_callbacks_t rgb_event_cb = { NULL };
        if (rgb_config->bounce_buffer_size_px == 0) {
            // When bounce buffer is disabled, use `on_vsync` callback to notify draw bitmap finish
            rgb_event_cb.on_vsync = (esp_lcd_rgb_panel_vsync_cb_t)onRefreshFinish;
        } else {
            // When bounce buffer is enabled, use `on_bounce_frame_finish` callback to notify draw bitmap finish
            rgb_event_cb.on_bounce_frame_finish = (esp_lcd_rgb_panel_bounce_buf_finish_cb_t)onRefreshFinish;
        }
        ESP_PANEL_CHECK_ERR_RET(esp_lcd_rgb_panel_register_event_callbacks(handle, &rgb_event_cb, &callback_data),
                                false, "Register RGB callback failed");
    }
#endif

    ESP_LOGD(TAG, "Begin end");

    return true;
}

bool ESP_PanelLcd::reset(void)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_reset(handle), false, "Reset panel failed");

    return true;
}

bool ESP_PanelLcd::del(void)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_del(handle), false, "Delete panel failed");

    ESP_LOGD(TAG, "LCD panel @%p deleted", handle);
    handle = NULL;

    return true;
}

bool ESP_PanelLcd::drawBitmap(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_draw_bitmap(handle, x_start, y_start, x_start + width, y_start + height, color_data),
                            false, "Draw bitmap failed");

    return true;
}

bool ESP_PanelLcd::drawBitmapWaitUntilFinish(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height,
        const uint8_t *color_data, int timeout_ms)
{
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");

    /* For RGB LCD, since `drawBitmap()` uses `memcpy()` instead of DMA operation, doesn't need to wait */
    if (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) {
        ESP_PANEL_CHECK_FALSE_RET(drawBitmap(x_start, y_start, width, height, color_data), false, "Draw bitmap failed");
        return true;
    }

    /* For other LCDs, since `drawBitmap()` use DMA operation, need to use semaphore to wait */
    ESP_PANEL_CHECK_NULL_RET(_refresh_finish_sem, false, "Semaphore is not created");

    ESP_PANEL_CHECK_FALSE_RET(drawBitmap(x_start, y_start, width, height, color_data), false, "Draw bitmap failed");

    /* Wait for the semaphore to be given by the callback function */
    BaseType_t timeout_tick = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    if (xSemaphoreTake(_refresh_finish_sem, timeout_tick) != pdTRUE) {
        ESP_LOGD(TAG, "Draw bitmap wait for finish timeout");
        return false;
    }

    return true;
}

bool ESP_PanelLcd::mirrorX(bool en)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_mirror(handle, en, _mirror_y), false, "Mirror X failed");
    _mirror_x = en;

    return true;
}

bool ESP_PanelLcd::mirrorY(bool en)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_mirror(handle, _mirror_x, en), false, "Mirror X failed");
    _mirror_y = en;

    return true;
}

bool ESP_PanelLcd::swapXY(bool en)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_swap_xy(handle, en), false, "Swap XY failed");
    _swap_xy = en;

    return true;
}

bool ESP_PanelLcd::setGapX(uint16_t gap)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_set_gap(handle, gap, _gap_y), false, "Set X gap failed");
    _gap_x = gap;

    return true;
}

bool ESP_PanelLcd::setGapY(uint16_t gap)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_set_gap(handle, _gap_x, gap), false, "Set Y gap failed");
    _gap_y = gap;

    return true;
}

bool ESP_PanelLcd::invertColor(bool en)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_invert_color(handle, en), false, "Invert color failed");

    return true;
}

bool ESP_PanelLcd::displayOn(void)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_disp_on_off(handle, true), false, "Display on failed");

    return true;
}

bool ESP_PanelLcd::displayOff(void)
{
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_disp_on_off(handle, false), false, "Display off failed");

    return true;
}

bool ESP_PanelLcd::attachRefreshFinishCallback(std::function<bool (void *)> callback, void *user_data)
{
    // For RGB LCD, if the "XIP on PSRAM" function is not enabled, the callback function and user data should be
    // placed in SRAM
#if SOC_LCD_RGB_SUPPORTED && CONFIG_LCD_RGB_ISR_IRAM_SAFE && \
    !(CONFIG_SPIRAM_RODATA && CONFIG_SPIRAM_FETCH_INSTRUCTIONS)
    if (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) {
        ESP_PANEL_CHECK_FALSE_RET((esp_ptr_in_iram(callback), false, "Callback function should be placed in IRAM, add
                                   `IRAM_ATTR` before the function"));
                                   ESP_PANEL_CHECK_FALSE_RET((esp_ptr_internal(user_data), false, "User data should be placed in SRAM"));
    }
#endif

// *INDENT-OFF*
    callback_data.user_data = user_data;
    onRefreshFinishCallback = callback;
// *INDENT-OFF*

    return true;
}

bool ESP_PanelLcd::colorBarTest(uint16_t width, uint16_t height)
{
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");

    int bits_per_piexl = getColorBits();
    ESP_PANEL_CHECK_FALSE_RET(bits_per_piexl > 0, false, "Invalid color bits");

    int bytes_per_piexl = bits_per_piexl / 8;
    int line_per_bar = height / bits_per_piexl;
    int line_count = 0;
    uint8_t *single_bar_buf = NULL;
    int res_line_count = 0;

    /* Malloc memory for a single color bar */
    try {
        single_bar_buf = new uint8_t[line_per_bar * width * bytes_per_piexl];
    } catch (std::bad_alloc &e) {
        ESP_PANEL_CHECK_FALSE_RET(false, false, "Malloc color buffer failed");
    }

    bool ret = true;

    /* Draw color bar from top left to bottom right, the order is B - G - R */
    for (int j = 0; j < bits_per_piexl; j++) {
        for (int i = 0; i < line_per_bar * width; i++) {
            for (int k = 0; k < bytes_per_piexl; k++) {
                if ((bus->getType() == ESP_PANEL_BUS_TYPE_SPI) || (bus->getType() == ESP_PANEL_BUS_TYPE_QSPI)) {
                    // For SPI interface, the data bytes should be swapped since the data is sent by LSB first
                    single_bar_buf[i * bytes_per_piexl + k] = SPI_SWAP_DATA_TX(BIT(j), bits_per_piexl) >> (k * 8);
                } else {
                    single_bar_buf[i * bytes_per_piexl + k] = BIT(j) >> (k * 8);
                }
            }
        }
        line_count += line_per_bar;
        ret = drawBitmapWaitUntilFinish(0, j * line_per_bar, width, line_per_bar, single_bar_buf);
        if (ret != true) {
            ESP_LOGE(TAG, "Draw bitmap failed");
            goto end;
        }
    }

    /* Fill the rest of the screen with white color */
    res_line_count = height - line_count;
    if (res_line_count > 0) {
        ESP_LOGD(TAG, "Fill the rest lines (%d) with white color", res_line_count);

        memset(single_bar_buf, 0xff, res_line_count * width * bytes_per_piexl);
        ret = drawBitmapWaitUntilFinish(0, line_count, width, res_line_count, single_bar_buf);
        if (ret != true) {
            ESP_LOGE(TAG, "Draw bitmap failed");
            goto end;
        }
    }

end:
    delete[] single_bar_buf;

    return ret;
}

int ESP_PanelLcd::getColorBits(void)
{
#if SOC_LCD_RGB_SUPPORTED
    ESP_PANEL_CHECK_NULL_RET(bus, -1, "Invalid bus");
    if (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) {
        const esp_lcd_rgb_panel_config_t *rgb_config = static_cast<ESP_PanelBus_RGB *>(bus)->getRgbConfig();
        ESP_PANEL_CHECK_NULL_RET(rgb_config, -1, "Invalid RGB config");

        return rgb_config->bits_per_pixel;
    }
#endif

    return panel_config.bits_per_pixel;
}

bool ESP_PanelLcd::getSwapXYFlag(void)
{
    return _swap_xy;
}

bool ESP_PanelLcd::getMirrorXFlag(void)
{
    return _mirror_x;
}

bool ESP_PanelLcd::getMirrorYFlag(void)
{
    return _mirror_y;
}

#if SOC_LCD_RGB_SUPPORTED
void *ESP_PanelLcd::getRgbBufferByIndex(uint8_t index)
{
    ESP_PANEL_CHECK_NULL_RET(handle, NULL, "Invalid handle");

    void *buffer[3] = { NULL };
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_rgb_panel_get_frame_buffer(handle, index + 1, &buffer[0], &buffer[1], &buffer[2]),
                  NULL, "Get RGB buffer failed");

    return buffer[index];
}
#endif

uint8_t ESP_PanelLcd::getXCoordAlign(void)
{
    return x_coord_align;
}

uint8_t ESP_PanelLcd::getYCoordAlign(void)
{
    return y_coord_align;
}

esp_lcd_panel_handle_t ESP_PanelLcd::getHandle(void)
{
    if (handle == NULL) {
        ESP_PANEL_ENABLE_TAG_DEBUG_LOG();
        ESP_LOGD(TAG, "Get invalid handle");
    }

    return handle;
}

ESP_PanelBus *ESP_PanelLcd::getBus(void)
{
    if (bus == NULL) {
        ESP_PANEL_ENABLE_TAG_DEBUG_LOG();
        ESP_LOGD(TAG, "Get invalid bus");
    }

    return bus;
}

IRAM_ATTR bool ESP_PanelLcd::onRefreshFinish(void *panel_io, void *edata, void *user_ctx)
{
    ESP_PanelLcdCallbackData_t *callback_data = (ESP_PanelLcdCallbackData_t *)user_ctx;
    if (callback_data == NULL) {
        return false;
    }

    ESP_PanelLcd *lcd_ptr = (ESP_PanelLcd *)callback_data->lcd_ptr;
    if (lcd_ptr == NULL) {
        return false;
    }

    BaseType_t need_yield = pdFALSE;
    if (lcd_ptr->onRefreshFinishCallback != NULL) {
        need_yield = lcd_ptr->onRefreshFinishCallback(callback_data->user_data) ? pdTRUE : need_yield;
    }
    if (lcd_ptr->_refresh_finish_sem != NULL) {
        xSemaphoreGiveFromISR(lcd_ptr->_refresh_finish_sem, &need_yield);
    }

    return (need_yield == pdTRUE);
}
