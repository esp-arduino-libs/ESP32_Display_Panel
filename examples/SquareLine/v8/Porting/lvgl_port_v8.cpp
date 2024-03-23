/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <lvgl.h>
#include "lvgl_port_v8.h"

#define LVGL_PORT_BUFFER_NUM_MAX       (2)

static const char *TAG = "lvgl_port";
static SemaphoreHandle_t lvgl_mux = nullptr;                  // LVGL mutex
static TaskHandle_t lvgl_task_handle = nullptr;

#if LVGL_PORT_ROTATION_DEGREE != 0
static void *get_next_frame_buffer(ESP_PanelLcd *lcd)
{
    static void *next_fb = NULL;
    static void *fbs[2] = { NULL };

    if (next_fb == NULL) {
        fbs[0] = lcd->getRgbBufferByIndex(0);
        fbs[1] = lcd->getRgbBufferByIndex(1);
        next_fb = fbs[1];
    } else {
        next_fb = (next_fb == fbs[0]) ? fbs[1] : fbs[0];
    }

    return next_fb;
}

IRAM_ATTR static void rotate_copy_pixel(const lv_color_t *from, lv_color_t *to, uint16_t x_start, uint16_t y_start,
                                        uint16_t x_end, uint16_t y_end, uint16_t w, uint16_t h, uint16_t rotate)
{
    int from_index = 0;
    int to_index = 0;
    int to_index_const = 0;

    switch (rotate) {
    case 90:
        to_index_const = (w - x_start - 1) * h;
        for (int from_y = y_start; from_y < y_end + 1; from_y++) {
            from_index = from_y * w + x_start;
            to_index = to_index_const + from_y;
            for (int from_x = x_start; from_x < x_end + 1; from_x++) {
                *(to + to_index) = *(from + from_index);
                from_index += 1;
                to_index -= h;
            }
        }
        break;
    case 180:
        to_index_const = h * w - x_start - 1;
        for (int from_y = y_start; from_y < y_end + 1; from_y++) {
            from_index = from_y * w + x_start;
            to_index = to_index_const - from_y * w;
            for (int from_x = x_start; from_x < x_end + 1; from_x++) {
                *(to + to_index) = *(from + from_index);
                from_index += 1;
                to_index -= 1;
            }
        }
        break;
    case 270:
        to_index_const = (x_start + 1) * h - 1;
        for (int from_y = y_start; from_y < y_end + 1; from_y++) {
            from_index = from_y * w + x_start;
            to_index = to_index_const - from_y;
            for (int from_x = x_start; from_x < x_end + 1; from_x++) {
                *(to + to_index) = *(from + from_index);
                from_index += 1;
                to_index += h;
            }
        }
        break;
    default:
        break;
    }
}
#endif /* LVGL_PORT_ROTATION_DEGREE */

#if LVGL_PORT_AVOID_TEAR
#if LVGL_PORT_DIRECT_MODE
#if LVGL_PORT_ROTATION_DEGREE != 0
typedef struct {
    uint16_t inv_p;
    uint8_t inv_area_joined[LV_INV_BUF_SIZE];
    lv_area_t inv_areas[LV_INV_BUF_SIZE];
} lv_port_dirty_area_t;

static lv_port_dirty_area_t dirty_area;

static void flush_dirty_save(lv_port_dirty_area_t *dirty_area)
{
    lv_disp_t *disp = _lv_refr_get_disp_refreshing();
    dirty_area->inv_p = disp->inv_p;
    for (int i = 0; i < disp->inv_p; i++) {
        dirty_area->inv_area_joined[i] = disp->inv_area_joined[i];
        dirty_area->inv_areas[i] = disp->inv_areas[i];
    }
}

typedef enum {
    FLUSH_STATUS_PART,
    FLUSH_STATUS_FULL
} lv_port_flush_status_t;

typedef enum {
    FLUSH_PROBE_PART_COPY,
    FLUSH_PROBE_SKIP_COPY,
    FLUSH_PROBE_FULL_COPY,
} lv_port_flush_probe_t;

/**
 * @brief Probe dirty area to copy
 *
 * @note This function is used to avoid tearing effect, and only work with LVGL direct-mode.
 *
 */
static lv_port_flush_probe_t flush_copy_probe(lv_disp_drv_t *drv)
{
    static lv_port_flush_status_t prev_status = FLUSH_STATUS_PART;
    lv_port_flush_status_t cur_status;
    lv_port_flush_probe_t probe_result;
    lv_disp_t *disp_refr = _lv_refr_get_disp_refreshing();

    uint32_t flush_ver = 0;
    uint32_t flush_hor = 0;
    for (int i = 0; i < disp_refr->inv_p; i++) {
        if (disp_refr->inv_area_joined[i] == 0) {
            flush_ver = (disp_refr->inv_areas[i].y2 + 1 - disp_refr->inv_areas[i].y1);
            flush_hor = (disp_refr->inv_areas[i].x2 + 1 - disp_refr->inv_areas[i].x1);
            break;
        }
    }
    /* Check if the current full screen refreshes */
    cur_status = ((flush_ver == drv->ver_res) && (flush_hor == drv->hor_res)) ? (FLUSH_STATUS_FULL) : (FLUSH_STATUS_PART);

    if (prev_status == FLUSH_STATUS_FULL) {
        if ((cur_status == FLUSH_STATUS_PART)) {
            probe_result = FLUSH_PROBE_FULL_COPY;
        } else {
            probe_result = FLUSH_PROBE_SKIP_COPY;
        }
    } else {
        probe_result = FLUSH_PROBE_PART_COPY;
    }
    prev_status = cur_status;

    return probe_result;
}

static inline void *flush_get_next_buf(ESP_PanelLcd *lcd)
{
    return get_next_frame_buffer(lcd);
}

/**
 * @brief Copy dirty area
 *
 * @note This function is used to avoid tearing effect, and only work with LVGL direct-mode.
 *
 */
static void flush_dirty_copy(void *dst, void *src, lv_port_dirty_area_t *dirty_area)
{
    lv_coord_t x_start, x_end, y_start, y_end;
    for (int i = 0; i < dirty_area->inv_p; i++) {
        /* Refresh the unjoined areas*/
        if (dirty_area->inv_area_joined[i] == 0) {
            x_start = dirty_area->inv_areas[i].x1;
            x_end = dirty_area->inv_areas[i].x2;
            y_start = dirty_area->inv_areas[i].y1;
            y_end = dirty_area->inv_areas[i].y2;

            rotate_copy_pixel((lv_color_t *)src, (lv_color_t *)dst, x_start, y_start, x_end, y_end, LV_HOR_RES, LV_VER_RES,
                              LVGL_PORT_ROTATION_DEGREE);
        }
    }
}

static void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ESP_PanelLcd *lcd = (ESP_PanelLcd *)drv->user_data;
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;
    void *next_fb = NULL;
    lv_port_flush_probe_t probe_result = FLUSH_PROBE_PART_COPY;
    lv_disp_t *disp = lv_disp_get_default();

    /* Action after last area refresh */
    if (lv_disp_flush_is_last(drv)) {
        /* Check if the `full_refresh` flag has been triggered */
        if (drv->full_refresh) {
            /* Reset flag */
            drv->full_refresh = 0;

            // Roate and copy data from the whole screen LVGL's buffer to the next frame buffer
            next_fb = flush_get_next_buf(lcd);
            rotate_copy_pixel((lv_color_t *)color_map, (lv_color_t *)next_fb, offsetx1, offsety1, offsetx2, offsety2,
                              LV_HOR_RES, LV_VER_RES, LVGL_PORT_ROTATION_DEGREE);

            /* Switch the current RGB frame buffer to `next_fb` */
            lcd->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)next_fb);

            /* Waiting for the current frame buffer to complete transmission */
            ulTaskNotifyValueClear(NULL, ULONG_MAX);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            /* Synchronously update the dirty area for another frame buffer */
            flush_dirty_copy(flush_get_next_buf(lcd), color_map, &dirty_area);
            flush_get_next_buf(lcd);
        } else {
            /* Probe the copy method for the current dirty area */
            probe_result = flush_copy_probe(drv);

            if (probe_result == FLUSH_PROBE_FULL_COPY) {
                /* Save current dirty area for next frame buffer */
                flush_dirty_save(&dirty_area);

                /* Set LVGL full-refresh flag and set flush ready in advance */
                drv->full_refresh = 1;
                disp->rendering_in_progress = false;
                lv_disp_flush_ready(drv);

                /* Force to refresh whole screen, and will invoke `flush_callback` recursively */
                lv_refr_now(_lv_refr_get_disp_refreshing());
            } else {
                /* Update current dirty area for next frame buffer */
                next_fb = flush_get_next_buf(lcd);
                flush_dirty_save(&dirty_area);
                flush_dirty_copy(next_fb, color_map, &dirty_area);

                /* Switch the current RGB frame buffer to `next_fb` */
                lcd->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)next_fb);

                /* Waiting for the current frame buffer to complete transmission */
                ulTaskNotifyValueClear(NULL, ULONG_MAX);
                ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

                if (probe_result == FLUSH_PROBE_PART_COPY) {
                    /* Synchronously update the dirty area for another frame buffer */
                    flush_dirty_save(&dirty_area);
                    flush_dirty_copy(flush_get_next_buf(lcd), color_map, &dirty_area);
                    flush_get_next_buf(lcd);
                }
            }
        }
    }

    lv_disp_flush_ready(drv);
}

#else

static void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ESP_PanelLcd *lcd = (ESP_PanelLcd *)drv->user_data;
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;

    /* Action after last area refresh */
    if (lv_disp_flush_is_last(drv)) {
        /* Switch the current RGB frame buffer to `color_map` */
        lcd->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)color_map);

        /* Waiting for the last frame buffer to complete transmission */
        ulTaskNotifyValueClear(NULL, ULONG_MAX);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    lv_disp_flush_ready(drv);
}
#endif /* LVGL_PORT_ROTATION_DEGREE */

#elif LVGL_PORT_FULL_REFRESH && LVGL_PORT_DISP_BUFFER_NUM == 2

static void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ESP_PanelLcd *lcd = (ESP_PanelLcd *)drv->user_data;
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;

    /* Switch the current RGB frame buffer to `color_map` */
    lcd->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)color_map);

    /* Waiting for the last frame buffer to complete transmission */
    ulTaskNotifyValueClear(NULL, ULONG_MAX);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    lv_disp_flush_ready(drv);
}

#elif LVGL_PORT_FULL_REFRESH && LVGL_PORT_DISP_BUFFER_NUM == 3

#if LVGL_PORT_ROTATION_DEGREE == 0
static void *lvgl_port_rgb_last_buf = NULL;
static void *lvgl_port_rgb_next_buf = NULL;
static void *lvgl_port_flush_next_buf = NULL;
#endif

void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ESP_PanelLcd *lcd = (ESP_PanelLcd *)drv->user_data;
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;

#if LVGL_PORT_ROTATION_DEGREE != 0
    void *next_fb = get_next_frame_buffer(lcd);

    /* Rotate and copy dirty area from the current LVGL's buffer to the next RGB frame buffer */
    rotate_copy_pixel((lv_color_t *)color_map, (lv_color_t *)next_fb, offsetx1, offsety1, offsetx2, offsety2, LV_HOR_RES,
                      LV_VER_RES, LVGL_PORT_ROTATION_DEGREE);

    /* Switch the current RGB frame buffer to `next_fb` */
    lcd->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)next_fb);
#else
    drv->draw_buf->buf1 = color_map;
    drv->draw_buf->buf2 = lvgl_port_flush_next_buf;
    lvgl_port_flush_next_buf = color_map;

    /* Switch the current RGB frame buffer to `color_map` */
    lcd->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)color_map);

    lvgl_port_rgb_next_buf = color_map;
#endif

    lv_disp_flush_ready(drv);
}
#endif

IRAM_ATTR bool onRgbVsyncCallback(void *user_data)
{
    BaseType_t need_yield = pdFALSE;
#if LVGL_PORT_FULL_REFRESH && (LVGL_PORT_DISP_BUFFER_NUM == 3) && (LVGL_PORT_ROTATION_DEGREE == 0)
    if (lvgl_port_rgb_next_buf != lvgl_port_rgb_last_buf) {
        lvgl_port_flush_next_buf = lvgl_port_rgb_last_buf;
        lvgl_port_rgb_last_buf = lvgl_port_rgb_next_buf;
    }
#else
    TaskHandle_t task_handle = (TaskHandle_t)user_data;
    // Notify that the current RGB frame buffer has been transmitted
    xTaskNotifyFromISR(task_handle, ULONG_MAX, eNoAction, &need_yield);
#endif
    return (need_yield == pdTRUE);
}

#else

void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ESP_PanelLcd *lcd = (ESP_PanelLcd *)drv->user_data;
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;

    lcd->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)color_map);
    // For RGB LCD, directly notify LVGL that the buffer is ready
    if (lcd->getBus()->getType() == ESP_PANEL_BUS_TYPE_RGB) {
        lv_disp_flush_ready(drv);
    }
}

static void update_callback(lv_disp_drv_t *drv)
{
    ESP_PanelLcd *lcd = (ESP_PanelLcd *)drv->user_data;
    static bool disp_init_mirror_x = lcd->getMirrorXFlag();
    static bool disp_init_mirror_y = lcd->getMirrorYFlag();
    static bool disp_init_swap_xy = lcd->getSwapXYFlag();

    switch (drv->rotated) {
    case LV_DISP_ROT_NONE:
        lcd->swapXY(disp_init_swap_xy);
        lcd->mirrorX(disp_init_mirror_x);
        lcd->mirrorY(disp_init_mirror_y);
        break;
    case LV_DISP_ROT_90:
        lcd->swapXY(!disp_init_swap_xy);
        lcd->mirrorX(disp_init_mirror_x);
        lcd->mirrorY(!disp_init_mirror_y);
        break;
    case LV_DISP_ROT_180:
        lcd->swapXY(disp_init_swap_xy);
        lcd->mirrorX(!disp_init_mirror_x);
        lcd->mirrorY(!disp_init_mirror_y);
        break;
    case LV_DISP_ROT_270:
        lcd->swapXY(!disp_init_swap_xy);
        lcd->mirrorX(!disp_init_mirror_x);
        lcd->mirrorY(disp_init_mirror_y);
        break;
    }

    ESP_LOGD(TAG, "Update display rotation to %d", drv->rotated);
    ESP_LOGD(TAG, "Current mirror x: %d, mirror y: %d, swap xy: %d", lcd->getMirrorXFlag(), lcd->getMirrorYFlag(), lcd->getSwapXYFlag());
}

#endif /* LVGL_PORT_AVOID_TEAR */

void rounder_callback(lv_disp_drv_t *drv, lv_area_t *area)
{
    ESP_PanelLcd *lcd = (ESP_PanelLcd *)drv->user_data;
    uint16_t x1 = area->x1;
    uint16_t x2 = area->x2;
    uint16_t y1 = area->y1;
    uint16_t y2 = area->y2;

    uint8_t x_align = lcd->getXCoordAlign();
    if (x_align > 1) {
        // round the start of coordinate down to the nearest (x_align * M) number
        area->x1 = (x1 / x_align) * x_align;
        // round the end of coordinate down to the nearest (x_align * (N + 1) - 1) number
        area->x2 = ((x2 + x_align - 1) / x_align + 1) * x_align - 1;
    }

    uint8_t y_align = lcd->getYCoordAlign();
    if (y_align > 1) {
        // round the start of coordinate down to the nearest (y_align * M) number
        area->y1 = (y1 / y_align) * y_align;
        // round the end of coordinate down to the nearest (y_align * (N + 1) - 1) number
        area->y2 = ((y2 + y_align - 1) / y_align + 1) * y_align - 1;
    }
}

static lv_disp_t *display_init(ESP_PanelLcd *lcd)
{
    ESP_PANEL_CHECK_FALSE_RET(lcd != nullptr, nullptr, "Invalid LCD device");
    ESP_PANEL_CHECK_FALSE_RET(lcd->getHandle() != nullptr, nullptr, "LCD device is not initialized");

    static lv_disp_draw_buf_t disp_buf;
    static lv_disp_drv_t disp_drv;

    // Alloc draw buffers used by LVGL
    void *buf[LVGL_PORT_BUFFER_NUM_MAX] = { nullptr };
    int buffer_size = 0;

    ESP_LOGD(TAG, "Malloc memory for LVGL buffer");
#if !LVGL_PORT_AVOID_TEAR
    // Avoid tearing function is disabled
    buffer_size = LVGL_PORT_BUFFER_SIZE;
    for (int i = 0; (i < LVGL_PORT_BUFFER_NUM) && (i < LVGL_PORT_BUFFER_NUM_MAX); i++) {
        buf[i] = heap_caps_malloc(buffer_size * sizeof(lv_color_t), LVGL_PORT_BUFFER_MALLOC_CAPS);
        assert(buf[i]);
        ESP_LOGD(TAG, "Buffer[%d] address: %p, size: %d", i, buf[i], buffer_size * sizeof(lv_color_t));
    }
#else
    // To avoid the tearing effect, we should use at least two frame buffers: one for LVGL rendering and another for RGB output
    buffer_size = LVGL_PORT_DISP_WIDTH * LVGL_PORT_DISP_HEIGHT;
#if (LVGL_PORT_DISP_BUFFER_NUM >= 3) && (LVGL_PORT_ROTATION_DEGREE == 0) && LVGL_PORT_FULL_REFRESH

    // With the usage of three buffers and full-refresh, we always have one buffer available for rendering,
    // eliminating the need to wait for the RGB's sync signal
    lvgl_port_rgb_last_buf = lcd->getRgbBufferByIndex(0);
    buf[0] = lcd->getRgbBufferByIndex(1);
    buf[1] = lcd->getRgbBufferByIndex(2);
    lvgl_port_rgb_next_buf = lvgl_port_rgb_last_buf;
    lvgl_port_flush_next_buf = buf[1];

#elif (LVGL_PORT_DISP_BUFFER_NUM >= 3) && (LVGL_PORT_ROTATION_DEGREE != 0)

    buf[0] = lcd->getRgbBufferByIndex(2);

#elif LVGL_PORT_DISP_BUFFER_NUM >= 2

    for (int i = 0; (i < LVGL_PORT_DISP_BUFFER_NUM) && (i < LVGL_PORT_BUFFER_NUM_MAX); i++) {
        buf[i] = lcd->getRgbBufferByIndex(i);
    }

#endif
#endif /* LVGL_PORT_AVOID_TEAR */

    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, buf[0], buf[1], buffer_size);

    ESP_LOGD(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = flush_callback;
#if LVGL_PORT_ROTATION_90 || LVGL_PORT_ROTATION_270
    disp_drv.hor_res = LVGL_PORT_DISP_HEIGHT;
    disp_drv.ver_res = LVGL_PORT_DISP_WIDTH;
#else
    disp_drv.hor_res = LVGL_PORT_DISP_WIDTH;
    disp_drv.ver_res = LVGL_PORT_DISP_HEIGHT;
#endif
#if LVGL_PORT_AVOID_TEAR    // Only available when the tearing effect is enabled
#if LVGL_PORT_FULL_REFRESH
    disp_drv.full_refresh = 1;
#elif LVGL_PORT_DIRECT_MODE
    disp_drv.direct_mode = 1;
#endif
#else                       // Only available when the tearing effect is disabled
    disp_drv.drv_update_cb = update_callback;
#endif /* LVGL_PORT_AVOID_TEAR */
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = (void *)lcd;
    // Only available when the coordinate alignment is enabled
    if (lcd->getXCoordAlign() > 1 || lcd->getYCoordAlign() > 1) {
        disp_drv.rounder_cb = rounder_callback;
    }

    return lv_disp_drv_register(&disp_drv);
}

static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    ESP_PanelTouch *tp = (ESP_PanelTouch *)indev_drv->user_data;
    ESP_PanelTouchPoint point;

    /* Read data from touch controller */
    int read_touch_result = tp->readPoints(&point, 1);
    if (read_touch_result > 0) {
        data->point.x = point.x;
        data->point.y = point.y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static lv_indev_t *indev_init(ESP_PanelTouch *tp)
{
    ESP_PANEL_CHECK_FALSE_RET(tp != nullptr, nullptr, "Invalid touch device");
    ESP_PANEL_CHECK_FALSE_RET(tp->getHandle() != nullptr, nullptr, "Touch device is not initialized");

    static lv_indev_drv_t indev_drv_tp;

    ESP_LOGD(TAG, "Register input driver to LVGL");
    lv_indev_drv_init(&indev_drv_tp);
    indev_drv_tp.type = LV_INDEV_TYPE_POINTER;
    indev_drv_tp.read_cb = touchpad_read;
    indev_drv_tp.user_data = (void *)tp;

    return lv_indev_drv_register(&indev_drv_tp);
}

#if !LV_TICK_CUSTOM
static void tick_increment(void *arg)
{
    /* Tell LVGL how many milliseconds have elapsed */
    lv_tick_inc(LVGL_PORT_TICK_PERIOD_MS);
}

static esp_err_t tick_init(void)
{
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &tick_increment,
        .name = "LVGL tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    return esp_timer_start_periodic(lvgl_tick_timer, LVGL_PORT_TICK_PERIOD_MS * 1000);
}
#endif

static void lvgl_port_task(void *arg)
{
    ESP_LOGD(TAG, "Starting LVGL task");

    uint32_t task_delay_ms = LVGL_PORT_TASK_MAX_DELAY_MS;
    while (1) {
        if (lvgl_port_lock(-1)) {
            task_delay_ms = lv_timer_handler();
            lvgl_port_unlock();
        }
        if (task_delay_ms > LVGL_PORT_TASK_MAX_DELAY_MS) {
            task_delay_ms = LVGL_PORT_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < LVGL_PORT_TASK_MIN_DELAY_MS) {
            task_delay_ms = LVGL_PORT_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

IRAM_ATTR bool onRefreshFinishCallback(void *user_data)
{
    lv_disp_drv_t *drv = (lv_disp_drv_t *)user_data;

    lv_disp_flush_ready(drv);

    return false;
}

bool lvgl_port_init(ESP_PanelLcd *lcd, ESP_PanelTouch *tp)
{
    ESP_PANEL_CHECK_FALSE_RET(lcd != nullptr, false, "Invalid LCD device");
#if LVGL_PORT_AVOID_TEAR
    ESP_PANEL_CHECK_FALSE_RET(lcd->getBus()->getType() == ESP_PANEL_BUS_TYPE_RGB, false, "Avoid tearing function only works with RGB LCD now");
    ESP_LOGD(TAG, "Avoid tearing is enabled, mode: %d", LVGL_PORT_AVOID_TEARING_MODE);
#endif

    lv_disp_t *disp = nullptr;
    lv_indev_t *indev = nullptr;

    lv_init();
#if !LV_TICK_CUSTOM
    ESP_PANEL_CHECK_ERR_RET(tick_init(), false, "Initialize LVGL tick failed");
#endif

    ESP_LOGD(TAG, "Initialize LVGL display driver");
    disp = display_init(lcd);
    ESP_PANEL_CHECK_NULL_RET(disp, false, "Initialize LVGL display driver failed");
    // Record the initial rotation of the display
    lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);

    // For non-RGB LCD, need to notify LVGL that the buffer is ready when the refresh is finished
    if (lcd->getBus()->getType() != ESP_PANEL_BUS_TYPE_RGB) {
        ESP_LOGD(TAG, "Attach refresh finish callback to LCD");
        lcd->attachRefreshFinishCallback(onRefreshFinishCallback, (void *)disp->driver);
    }

    if (tp != nullptr) {
        ESP_LOGD(TAG, "Initialize LVGL input driver");
        indev = indev_init(tp);
        ESP_PANEL_CHECK_NULL_RET(indev, false, "Initialize LVGL input driver failed");

#if LVGL_PORT_ROTATION_DEGREE == 90
        tp->swapXY(!tp->getSwapXYFlag());
        tp->mirrorY(!tp->getMirrorYFlag());
#elif LVGL_PORT_ROTATION_DEGREE == 180
        tp->mirrorX(!tp->getMirrorXFlag());
        tp->mirrorY(!tp->getMirrorYFlag());
#elif LVGL_PORT_ROTATION_DEGREE == 270
        tp->swapXY(!tp->getSwapXYFlag());
        tp->mirrorX(!tp->getMirrorYFlag());
#endif
    }

    ESP_LOGD(TAG, "Create mutex for LVGL");
    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    ESP_PANEL_CHECK_NULL_RET(lvgl_mux, false, "Create LVGL mutex failed");

    ESP_LOGD(TAG, "Create LVGL task");
    BaseType_t core_id = (LVGL_PORT_TASK_CORE < 0) ? tskNO_AFFINITY : LVGL_PORT_TASK_CORE;
    BaseType_t ret = xTaskCreatePinnedToCore(lvgl_port_task, "lvgl", LVGL_PORT_TASK_STACK_SIZE, NULL,
                     LVGL_PORT_TASK_PRIORITY, &lvgl_task_handle, core_id);
    ESP_PANEL_CHECK_FALSE_RET(ret == pdPASS, false, "Create LVGL task failed");

#if LVGL_PORT_AVOID_TEAR
    lcd->attachRefreshFinishCallback(onRgbVsyncCallback, (void *)lvgl_task_handle);
#endif

    return true;
}

bool lvgl_port_lock(int timeout_ms)
{
    ESP_PANEL_CHECK_NULL_RET(lvgl_mux, false, "LVGL mutex is not initialized");

    const TickType_t timeout_ticks = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return (xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE);
}

bool lvgl_port_unlock(void)
{
    ESP_PANEL_CHECK_NULL_RET(lvgl_mux, false, "LVGL mutex is not initialized");

    xSemaphoreGiveRecursive(lvgl_mux);

    return true;
}
