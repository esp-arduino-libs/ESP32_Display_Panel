/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <memory>
#include "cstring"
#include "ESP_PanelLog.h"
#include "sdkconfig.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_memory_utils.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "bus/RGB.h"
#include "bus/DSI.h"
#include "bus/ESP_PanelBus.h"
#include "ESP_PanelLcd.h"

#define VENDOR_CONFIG_DEFAULT()      \
    {                                \
        .init_cmds = NULL,           \
        .init_cmds_size = 0,         \
        .flags = {                   \
            .mirror_by_cmd = 1,      \
            .auto_del_panel_io = 0,  \
            .use_spi_interface = 0,  \
            .use_qspi_interface = 0, \
            .use_rgb_interface = 0,  \
            .use_mipi_interface = 0, \
        }                            \
    }
#define CALLBACK_DATA_DEFAULT() \
    {                           \
        .lcd_ptr = this,        \
        .user_data = NULL,      \
    }

static const char *TAG = "ESP_PanelLcd";

using namespace std;

ESP_PanelLcd::ESP_PanelLcd(ESP_PanelBus *bus, uint8_t color_bits, int rst_io):
    disabled_functions{},
    x_coord_align(0),
    y_coord_align(0),
    bus(bus),
    panel_config(ESP_PANEL_LCD_DEVICE_CONFIG_DEFAULT(rst_io, color_bits, &vendor_config)),
    vendor_config((esp_lcd_panel_vendor_config_t)VENDOR_CONFIG_DEFAULT()),
    handle(NULL),
    _flags{},
    _gap_x(0),
    _gap_y(0),
    onDrawBitmapFinishCallback(NULL),
    onRefreshFinishCallback(NULL),
    _draw_bitmap_finish_sem(NULL),
    _callback_data(CALLBACK_DATA_DEFAULT())
{
}

ESP_PanelLcd::ESP_PanelLcd(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config):
    disabled_functions{},
    x_coord_align(0),
    y_coord_align(0),
    bus(bus),
    panel_config(panel_config),
    vendor_config(VENDOR_CONFIG_DEFAULT()),
    handle(NULL),
    _flags{},
    _gap_x(0),
    _gap_y(0),
    onDrawBitmapFinishCallback(NULL),
    onRefreshFinishCallback(NULL),
    _draw_bitmap_finish_sem(NULL),
    _callback_data(CALLBACK_DATA_DEFAULT())
{
    /* Save vendor configuration to local and register the local one into panel configuration */
    if (panel_config.vendor_config != NULL) {
        vendor_config = *(esp_lcd_panel_vendor_config_t *)panel_config.vendor_config;
    }
    this->panel_config.vendor_config = &vendor_config;
}

bool ESP_PanelLcd::configVendorCommands(const esp_lcd_panel_vendor_init_cmd_t init_cmd[], uint32_t init_cmd_size)
{
    ESP_PANEL_CHECK_FALSE_RET(!checkIsInit(), false, "This function should be called before `init()`");
    ESP_PANEL_CHECK_FALSE_RET((init_cmd == NULL) || (init_cmd_size > 0), false, "Size of init commands is invalid");

    vendor_config.init_cmds = init_cmd;
    vendor_config.init_cmds_size = init_cmd_size;

    return true;
}

bool ESP_PanelLcd::configColorRgbOrder(bool BGR_order)
{
    ESP_PANEL_CHECK_FALSE_RET(!checkIsInit(), false, "This function should be called before `init()`");

    if (BGR_order) {
        panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR;
    } else {
        panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
    }

    return true;
}

bool ESP_PanelLcd::configResetActiveLevel(int level)
{
    ESP_PANEL_CHECK_FALSE_RET(!checkIsInit(), false, "This function should be called before `init()`");

    panel_config.flags.reset_active_high = level;

    return true;
}

bool ESP_PanelLcd::configMirrorByCommand(bool en)
{
    ESP_PANEL_CHECK_FALSE_RET(!checkIsInit(), false, "This function should be called before `init()`");
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");
    ESP_PANEL_CHECK_FALSE_RET(bus->getType() == ESP_PANEL_BUS_TYPE_RGB, false, "This function is only for RGB interface");

    vendor_config.flags.mirror_by_cmd = en;

    return true;
}

bool ESP_PanelLcd::configEnableIO_Multiplex(bool en)
{
    ESP_PANEL_CHECK_FALSE_RET(!checkIsInit(), false, "This function should be called before `init()`");
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");
    ESP_PANEL_CHECK_FALSE_RET(bus->getType() == ESP_PANEL_BUS_TYPE_RGB, false, "This function is only for RGB interface");

    vendor_config.flags.enable_io_multiplex = en;

    return true;
}

bool ESP_PanelLcd::begin(void)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");
    ESP_PANEL_CHECK_FALSE_RET(!checkIsBegun() || _flags.is_reset, false, "Already begun and not reset");

    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    ESP_LOGD(TAG, "Begin start");

    /* Initialize LCD panel */
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_init(handle), false, "Init panel failed");

    /* If the panel is reset, goto end directly */
    if (checkIsBegun() && _flags.is_reset) {
        goto end;
    }

    /* For non-RGB interface, create Semaphore for using API `drawBitmapWaitUntilFinish()` */
    if ((bus->getType() != ESP_PANEL_BUS_TYPE_RGB) && (_draw_bitmap_finish_sem == NULL)) {
        _draw_bitmap_finish_sem = xSemaphoreCreateBinary();
        ESP_PANEL_CHECK_NULL_RET(_draw_bitmap_finish_sem, false, "Create draw bitmap finish semaphore failed");
    }

    /* Register transimit done callback for different interface */
    switch (bus->getType()) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB: {
        esp_lcd_rgb_panel_event_callbacks_t rgb_event_cb = {};
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0)
        rgb_event_cb.on_frame_buf_complete = (esp_lcd_rgb_panel_frame_buf_complete_cb_t)onRefreshFinish;
#else
        const esp_lcd_rgb_panel_config_t *rgb_config = static_cast<ESP_PanelBus_RGB *>(bus)->getRgbConfig();
        if (rgb_config->bounce_buffer_size_px == 0) {
            // When bounce buffer is disabled, use `on_vsync` callback to notify draw bitmap finish
            rgb_event_cb.on_vsync = (esp_lcd_rgb_panel_vsync_cb_t)onRefreshFinish;
        } else {
            // When bounce buffer is enabled, use `on_bounce_frame_finish` callback to notify draw bitmap finish
            rgb_event_cb.on_bounce_frame_finish = (esp_lcd_rgb_panel_bounce_buf_finish_cb_t)onRefreshFinish;
        }
#endif
        ESP_PANEL_CHECK_ERR_RET(
            esp_lcd_rgb_panel_register_event_callbacks(handle, &rgb_event_cb, &_callback_data), false,
            "Register RGB callback failed"
        );
        break;
    }
#endif
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI: {
        esp_lcd_dpi_panel_event_callbacks_t dpi_event_cb = {
            .on_color_trans_done = (esp_lcd_dpi_panel_color_trans_done_cb_t)onDrawBitmapFinish,
            .on_refresh_done = (esp_lcd_dpi_panel_refresh_done_cb_t)onRefreshFinish,
        };
        ESP_PANEL_CHECK_ERR_RET(
            esp_lcd_dpi_panel_register_event_callbacks(handle, &dpi_event_cb, &_callback_data), false,
            "Register MIPI-DSI callback failed"
        );
        break;
    }
#endif
    default:
        esp_lcd_panel_io_callbacks_t io_cb = {
            .on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)onDrawBitmapFinish,
        };
        ESP_PANEL_CHECK_ERR_RET(
            esp_lcd_panel_io_register_event_callbacks(bus->getPanelIO_Handle(), &io_cb, &_callback_data), false,
            "Register panel IO callback failed"
        );
        break;
    }

end:
    ESP_LOGD(TAG, "Begin end");
    _flags.is_begun = true;
    _flags.is_reset = false;

    return true;
}

bool ESP_PanelLcd::reset(void)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

#if SOC_LCD_RGB_SUPPORTED
    if ((bus->getType() == ESP_PANEL_BUS_TYPE_RGB) && !checkIsBegun() && vendor_config.flags.auto_del_panel_io) {
        ESP_LOGD(TAG, "Ignore reset panel before begun for RGB LCD with auto release bus");
        goto end;
    }
#endif

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_reset(handle), false, "Reset panel failed");

#if SOC_LCD_RGB_SUPPORTED
end:
#endif
    _flags.is_reset = true;
    _flags.is_begun = false;

    return true;
}

bool ESP_PanelLcd::del(void)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_del(handle), false, "Delete panel failed");
    if (_draw_bitmap_finish_sem) {
        vSemaphoreDelete(_draw_bitmap_finish_sem);
        _draw_bitmap_finish_sem = NULL;
    }

    ESP_LOGD(TAG, "LCD panel @%p deleted", handle);
    handle = NULL;
    _flags = {};

    return true;
}

bool ESP_PanelLcd::drawBitmap(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsBegun(), false, "Not begun");

    ESP_PANEL_CHECK_ERR_RET(
        esp_lcd_panel_draw_bitmap(handle, x_start, y_start, x_start + width, y_start + height, color_data),
        false, "Draw bitmap failed"
    );

    return true;
}

bool ESP_PanelLcd::drawBitmapWaitUntilFinish(
    uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data, int timeout_ms
)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsBegun(), false, "Not begun");

    /* For RGB LCD, since `drawBitmap()` uses `memcpy()` instead of DMA operation, doesn't need to wait */
    ESP_PANEL_CHECK_FALSE_RET(drawBitmap(x_start, y_start, width, height, color_data), false, "Draw bitmap failed");

    /* For other interfaces which uses DMA operation, wait for the drawing to finish */
    if (bus->getType() != ESP_PANEL_BUS_TYPE_RGB) {
        /* Wait for the semaphore to be given by the callback function */
        BaseType_t timeout_tick = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
        ESP_PANEL_CHECK_FALSE_RET(
            xSemaphoreTake(_draw_bitmap_finish_sem, timeout_tick) == pdTRUE, false,
            "Draw bitmap wait for finish timeout"
        );
    }

    return true;
}

bool ESP_PanelLcd::mirrorX(bool en)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    if (disabled_functions.mirror) {
        ESP_LOGW(TAG, "Mirror function is disabled");
        return true;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_mirror(handle, en, _flags.mirror_y), false, "Mirror X failed");
    _flags.mirror_x = en;

    return true;
}

bool ESP_PanelLcd::mirrorY(bool en)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    if (disabled_functions.mirror) {
        ESP_LOGW(TAG, "Mirror function is disabled");
        return true;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_mirror(handle, _flags.mirror_x, en), false, "Mirror X failed");
    _flags.mirror_y = en;

    return true;
}

bool ESP_PanelLcd::swapXY(bool en)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    if (disabled_functions.swap_xy) {
        ESP_LOGW(TAG, "Swap XY function is disabled");
        return true;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_swap_xy(handle, en), false, "Swap XY failed");
    _flags.swap_xy = en;

    return true;
}

bool ESP_PanelLcd::setGapX(uint16_t gap)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    if (disabled_functions.set_gap) {
        ESP_LOGW(TAG, "Set gap function is disabled");
        return true;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_set_gap(handle, gap, _gap_y), false, "Set X gap failed");
    _gap_x = gap;

    return true;
}

bool ESP_PanelLcd::setGapY(uint16_t gap)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    if (disabled_functions.set_gap) {
        ESP_LOGW(TAG, "Set gap function is disabled");
        return true;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_set_gap(handle, _gap_x, gap), false, "Set Y gap failed");
    _gap_y = gap;

    return true;
}

bool ESP_PanelLcd::invertColor(bool en)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_invert_color(handle, en), false, "Invert color failed");

    return true;
}

bool ESP_PanelLcd::displayOn(void)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    bool is_disable = disabled_functions.display_on_off;
    if (is_disable) {
        goto end;
    }

#if SOC_LCD_RGB_SUPPORTED
    if (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) {
        is_disable = (bus->getPanelIO_Handle() == NULL) && (vendor_config.rgb_config->disp_gpio_num == -1);
        if (is_disable) {
            goto end;
        }
    }
#endif

end:
    if (is_disable) {
        ESP_LOGW(TAG, "Display on/off function is disabled");
        return true;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_disp_on_off(handle, true), false, "Display on failed");

    return true;
}

bool ESP_PanelLcd::displayOff(void)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");

    if (disabled_functions.display_on_off) {
        ESP_LOGW(TAG, "Display on/off function is disabled");
        return true;
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_disp_on_off(handle, false), false, "Display off failed");

    return true;
}

bool ESP_PanelLcd::attachDrawBitmapFinishCallback(std::function<bool (void *)> callback, void *user_data)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");
    ESP_PANEL_CHECK_FALSE_RET(
        bus->getType() != ESP_PANEL_BUS_TYPE_RGB, false, "RGB interface doesn't support this callback"
    );

// *INDENT-OFF*
    if ((_callback_data.user_data != NULL) && (_callback_data.user_data != user_data)) {
        ESP_LOGW(TAG, "Callback data is not NULL, will overwrite the previous one");
    }
    _callback_data.user_data = user_data;
    onDrawBitmapFinishCallback = callback;
// *INDENT-OFF*

    return true;
}

bool ESP_PanelLcd::attachRefreshFinishCallback(std::function<bool (void *)> callback, void *user_data)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsInit(), false, "Not initialized");
    ESP_PANEL_CHECK_FALSE_RET(
        (bus->getType() != ESP_PANEL_BUS_TYPE_SPI) && (bus->getType() != ESP_PANEL_BUS_TYPE_QSPI), false,
        "SPI and QSPI interfaces don't support this callback"
    );

    /* Check the callback function and user data placement */
    // For RGB LCD, if the "XIP on PSRAM" function is not enabled, the callback function and user data should be
    // placed in SRAM
#if (SOC_MIPI_DSI_SUPPORTED && CONFIG_LCD_DSI_ISR_IRAM_SAFE) || \
    (SOC_LCD_RGB_SUPPORTED && CONFIG_LCD_RGB_ISR_IRAM_SAFE && !(CONFIG_SPIRAM_RODATA && CONFIG_SPIRAM_FETCH_INSTRUCTIONS))
    if (bus->getType() == ESP_PANEL_BUS_TYPE_RGB || bus->getType() == ESP_PANEL_BUS_TYPE_MIPI_DSI) {
        ESP_PANEL_CHECK_FALSE_RET(
            esp_ptr_in_iram(callback), false,
            "Callback function should be placed in IRAM, add `IRAM_ATTR` before the function"
        );
        ESP_PANEL_CHECK_FALSE_RET((esp_ptr_internal(user_data), false, "User data should be placed in SRAM"));
    }
#endif

// *INDENT-OFF*
    if ((_callback_data.user_data != NULL) && (_callback_data.user_data != user_data)) {
        ESP_LOGW(TAG, "Callback data is not NULL, will overwrite the previous one");
    }
    _callback_data.user_data = user_data;
    onRefreshFinishCallback = callback;
// *INDENT-OFF*

    return true;
}

bool ESP_PanelLcd::colorBarTest(uint16_t width, uint16_t height)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsBegun(), false, "Not begun");

    int bits_per_piexl = getColorBits();
    ESP_PANEL_CHECK_FALSE_RET(bits_per_piexl > 0, false, "Invalid color bits");

    ESP_LOGD(TAG, "Color bar test, width: %d, height: %d, bits per pixel: %d", width, height, bits_per_piexl);

    int bytes_per_piexl = bits_per_piexl / 8;
    int row_per_bar = height / bits_per_piexl;
    int line_count = 0;
    int res_line_count = 0;

    /* Malloc memory for a single color bar */
    shared_ptr<uint8_t> single_bar_buf(new uint8_t[row_per_bar * width * bytes_per_piexl]);
    ESP_PANEL_CHECK_FALSE_RET(single_bar_buf != nullptr, false, "Malloc color buffer failed");

    /* Draw color bar from top left to bottom right, the order is B - G - R */
    for (int j = 0; j < bits_per_piexl; j++) {
        for (int i = 0; i < row_per_bar * width; i++) {
            for (int k = 0; k < bytes_per_piexl; k++) {
                if ((bus->getType() == ESP_PANEL_BUS_TYPE_SPI) || (bus->getType() == ESP_PANEL_BUS_TYPE_QSPI)) {
                    // For SPI interface, the data bytes should be swapped since the data is sent by LSB first
                    single_bar_buf.get()[i * bytes_per_piexl + k] = SPI_SWAP_DATA_TX(BIT(j), bits_per_piexl) >> (k * 8);
                } else {
                    single_bar_buf.get()[i * bytes_per_piexl + k] = BIT(j) >> (k * 8);
                }
            }
        }
        line_count += row_per_bar;
        ESP_PANEL_CHECK_FALSE_RET(
            drawBitmapWaitUntilFinish(0, j * row_per_bar, width, row_per_bar, single_bar_buf.get()), false,
            "Draw bitmap failed"
        );
    }

    /* Fill the rest of the screen with white color */
    res_line_count = height - line_count;
    if (res_line_count > 0) {
        ESP_LOGD(TAG, "Fill the rest lines (%d) with white color", res_line_count);

        memset(single_bar_buf.get(), 0xff, res_line_count * width * bytes_per_piexl);
        ESP_PANEL_CHECK_FALSE_RET(
            drawBitmapWaitUntilFinish(0, line_count, width, res_line_count, single_bar_buf.get()), false,
            "Draw bitmap failed"
        );
    }

    return true;
}

#if SOC_MIPI_DSI_SUPPORTED
bool ESP_PanelLcd::showDsiPattern(DsiPatternType type)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsBegun(), false, "Not begun");
    ESP_PANEL_CHECK_FALSE_RET(
        bus->getType() == ESP_PANEL_BUS_TYPE_MIPI_DSI, false, "Invalid bus type(%d)", bus->getType()
    );

    ESP_PANEL_CHECK_ERR_RET(
        esp_lcd_dpi_panel_set_pattern(handle, static_cast<mipi_dsi_pattern_type_t>(type)), false,
        "Set DPI pattern failed"
    );

    return true;
}
#endif /* SOC_MIPI_DSI_SUPPORTED */

int ESP_PanelLcd::getColorBits(void)
{
    ESP_PANEL_CHECK_NULL_RET(bus, -1, "Invalid bus");

    int bits_per_pixel = panel_config.bits_per_pixel;
    switch (bus->getType()) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB: {
        const esp_lcd_rgb_panel_config_t *rgb_config = static_cast<ESP_PanelBus_RGB *>(bus)->getRgbConfig();
        ESP_PANEL_CHECK_NULL_RET(rgb_config, -1, "Invalid RGB config");
        bits_per_pixel = rgb_config->bits_per_pixel;
        break;
    }
#endif /* SOC_LCD_RGB_SUPPORTED */
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI: {
        const esp_lcd_dpi_panel_config_t *dpi_config = static_cast<ESP_PanelBus_DSI *>(bus)->getDpiConfig();
        ESP_PANEL_CHECK_NULL_RET(dpi_config, -1, "Invalid MIPI DPI config");
        switch (dpi_config->pixel_format) {
        case LCD_COLOR_PIXEL_FORMAT_RGB565:
            bits_per_pixel = 16;
            break;
        case LCD_COLOR_PIXEL_FORMAT_RGB666:
            bits_per_pixel = 18;
            break;
        case LCD_COLOR_PIXEL_FORMAT_RGB888:
            bits_per_pixel = 24;
            break;
        default:
            break;
        }
        break;
    }
#endif /* SOC_MIPI_DSI_SUPPORTED */
    default:
        break;
    }

    return bits_per_pixel;
}

void *ESP_PanelLcd::getFrameBufferByIndex(uint8_t index)
{
    ESP_PANEL_CHECK_FALSE_RET(checkIsBegun(), NULL, "Not begun");
    ESP_PANEL_CHECK_FALSE_RET(
        index < ESP_PANEL_LCD_FRAME_BUFFER_MAX_NUM, NULL, "Index(%d) out of range(0-%d)", index,
        ESP_PANEL_LCD_FRAME_BUFFER_MAX_NUM - 1
    );

    void *buffer[ESP_PANEL_LCD_FRAME_BUFFER_MAX_NUM] = {};
    switch (bus->getType()) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB:
        ESP_PANEL_CHECK_ERR_RET(
            esp_lcd_rgb_panel_get_frame_buffer(handle, index + 1, &buffer[0], &buffer[1], &buffer[2]), NULL,
            "Get RGB buffer failed"
        );
        break;
#endif
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI:
        ESP_PANEL_CHECK_ERR_RET(
            esp_lcd_dpi_panel_get_frame_buffer(handle, index + 1, &buffer[0], &buffer[1], &buffer[2]), NULL,
            "Get MIPI DPI buffer failed"
        );
        break;
#endif
    default:
        ESP_PANEL_CHECK_FALSE_RET(false, NULL, "Invalid bus type(%d)", bus->getType());
        break;
    }

    return buffer[index];
}

bool ESP_PanelLcd::loadVendorConfigFromBus(void)
{
    ESP_PANEL_CHECK_FALSE_RET(!checkIsInit(), false, "This function should be called before `init()`");
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");

    switch (bus->getType()) {
    case ESP_PANEL_BUS_TYPE_SPI:
        vendor_config.flags.use_spi_interface = 1;
        break;
    case ESP_PANEL_BUS_TYPE_QSPI:
        vendor_config.flags.use_qspi_interface = 1;
        break;
#if SOC_LCD_RGB_SUPPORTED
    /* Retrieve RGB configuration from the bus and register it into the vendor configuration */
    case ESP_PANEL_BUS_TYPE_RGB:
        vendor_config.flags.use_rgb_interface = 1;
        vendor_config.rgb_config = static_cast<ESP_PanelBus_RGB *>(bus)->getRgbConfig();
        break;
#endif
#if SOC_MIPI_DSI_SUPPORTED
    /* Retrieve MIPI DPI configuration from the bus and register it into the vendor configuration */
    case ESP_PANEL_BUS_TYPE_MIPI_DSI:
        vendor_config.flags.use_mipi_interface = 1;
        vendor_config.mipi_config = {
            .lane_num = static_cast<ESP_PanelBus_DSI *>(bus)->getDsiConfig()->num_data_lanes,
            .dsi_bus = static_cast<ESP_PanelBus_DSI *>(bus)->getBusHandle(),
            .dpi_config = static_cast<ESP_PanelBus_DSI *>(bus)->getDpiConfig(),
        };
        break;
#endif
    default:
        ESP_LOGE(TAG, "Unsupported bus type");
        break;
    }

    return true;
}

IRAM_ATTR bool ESP_PanelLcd::onDrawBitmapFinish(void *panel_io, void *edata, void *user_ctx)
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
    if (lcd_ptr->onDrawBitmapFinishCallback != NULL) {
        need_yield = lcd_ptr->onDrawBitmapFinishCallback(callback_data->user_data) ? pdTRUE : need_yield;
    }
    if (lcd_ptr->_draw_bitmap_finish_sem != NULL) {
        xSemaphoreGiveFromISR(lcd_ptr->_draw_bitmap_finish_sem, &need_yield);
    }

    return (need_yield == pdTRUE);
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

    return (need_yield == pdTRUE);
}
