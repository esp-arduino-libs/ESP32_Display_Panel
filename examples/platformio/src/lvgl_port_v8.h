/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#pragma once

#include <ESP_Panel_Library.h>
#include <lvgl.h>

// *INDENT-OFF*

/**
 * LVGL related parameters, can be adjusted by users
 *
 */
#define LVGL_PORT_DISP_WIDTH                    (ESP_PANEL_LCD_WIDTH)   // The width of the display
#define LVGL_PORT_DISP_HEIGHT                   (ESP_PANEL_LCD_HEIGHT)  // The height of the display
#define LVGL_PORT_TICK_PERIOD_MS                (2) // The period of the LVGL tick task, in milliseconds

/**
 *
 * LVGL buffer related parameters, can be adjusted by users:
 *
 *  (These parameters will be useless if the avoid tearing function is enabled)
 *
 *  - Memory type for buffer allocation:
 *      - MALLOC_CAP_SPIRAM: Allocate LVGL buffer in PSRAM
 *      - MALLOC_CAP_INTERNAL: Allocate LVGL buffer in SRAM
 *
 *      (The SRAM is faster than PSRAM, but the PSRAM has a larger capacity)
 *      (For SPI/QSPI LCD, it is recommended to allocate the buffer in SRAM, because the SPI DMA does not directly support PSRAM now)
 *
 *  - The size (in bytes) and number of buffers:
 *      - Lager buffer size can improve FPS, but it will occupy more memory. Maximum buffer size is `LVGL_PORT_DISP_WIDTH * LVGL_PORT_DISP_HEIGHT`.
 *      - The number of buffers should be 1 or 2.
 *
 */
#define LVGL_PORT_BUFFER_MALLOC_CAPS            (MALLOC_CAP_INTERNAL)       // Allocate LVGL buffer in SRAM
// #define LVGL_PORT_BUFFER_MALLOC_CAPS            (MALLOC_CAP_SPIRAM)      // Allocate LVGL buffer in PSRAM
#define LVGL_PORT_BUFFER_SIZE                   (LVGL_PORT_DISP_WIDTH * 20)
#define LVGL_PORT_BUFFER_NUM                    (2)

/**
 * LVGL timer handle task related parameters, can be adjusted by users
 *
 */
#define LVGL_PORT_TASK_MAX_DELAY_MS             (500)       // The maximum delay of the LVGL timer task, in milliseconds
#define LVGL_PORT_TASK_MIN_DELAY_MS             (2)         // The minimum delay of the LVGL timer task, in milliseconds
#define LVGL_PORT_TASK_STACK_SIZE               (6 * 1024)  // The stack size of the LVGL timer task, in bytes
#define LVGL_PORT_TASK_PRIORITY                 (2)         // The priority of the LVGL timer task
#define LVGL_PORT_TASK_CORE                     (-1)        // The core of the LVGL timer task, `-1` means the don't specify the core
                                                            // This can be set to `1` only if the SoCs support dual-core,
                                                            // otherwise it should be set to `-1` or `0`

/**
 * Avoid tering related configurations, can be adjusted by users.
 *
 *  (Currently, This function only supports RGB LCD)
 *
 */
/**
 * Set the avoid tearing mode:
 *      - 0: Disable avoid tearing function
 *      - 1: LCD double-buffer & LVGL full-refresh
 *      - 2: LCD triple-buffer & LVGL full-refresh
 *      - 3: LCD double-buffer & LVGL direct-mode (recommended)
 *
 */
#define LVGL_PORT_AVOID_TEARING_MODE            (0)

#if LVGL_PORT_AVOID_TEARING_MODE != 0
/**
 * As the anti-tearing feature typically consumes more PSRAM bandwidth, for the ESP32-S3, we need to utilize the Bounce
 * buffer functionality to enhance the RGB data bandwidth.
 *
 * This feature will occupy `LVGL_PORT_RGB_BOUNCE_BUFFER_SIZE * 2 * bytes_per_pixel` of SRAM memory.
 *
 */
#define LVGL_PORT_RGB_BOUNCE_BUFFER_SIZE        (LVGL_PORT_DISP_WIDTH * 10)
/**
 * When avoid tearing is enabled, the LVGL software rotation `lv_disp_set_rotation()` is not supported.
 * But users can set the rotation degree(0/90/180/270) here, but this funciton will extremely reduce FPS.
 * So it is recommended to be used when using a low resolution display.
 *
 * Set the rotation degree:
 *      - 0: 0 degree
 *      - 90: 90 degree
 *      - 180: 180 degree
 *      - 270: 270 degree
 *
 */
#define LVGL_PORT_ROTATION_DEGREE               (0)

/**
 * Here, some important configurations will be set based on different anti-tearing modes and rotation angles.
 * No modification is required here.
 *
 * Users should use `lcd_bus->configRgbFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);` to set the buffer number before. If screen drifting occurs, please refer to the Troubleshooting section in the README.
 * initializing the LCD bus
 *
 */
#define LVGL_PORT_AVOID_TEAR                    (1)
// Set the buffer number and refresh mode according to the different modes
#if LVGL_PORT_AVOID_TEARING_MODE == 1
    #define LVGL_PORT_DISP_BUFFER_NUM           (2)
    #define LVGL_PORT_FULL_REFRESH              (1)
#elif LVGL_PORT_AVOID_TEARING_MODE == 2
    #define LVGL_PORT_DISP_BUFFER_NUM           (3)
    #define LVGL_PORT_FULL_REFRESH              (1)
#elif LVGL_PORT_AVOID_TEARING_MODE == 3
    #define LVGL_PORT_DISP_BUFFER_NUM           (2)
    #define LVGL_PORT_DIRECT_MODE               (1)
#else
    #error "Invalid avoid tearing mode, please set macro `LVGL_PORT_AVOID_TEARING_MODE` to one of `LVGL_PORT_AVOID_TEARING_MODE_*`"
#endif
// Check rotation
#if (LVGL_PORT_ROTATION_DEGREE != 0) && (LVGL_PORT_ROTATION_DEGREE != 90) && (LVGL_PORT_ROTATION_DEGREE != 180) && \
    (LVGL_PORT_ROTATION_DEGREE != 270)
    #error "Invalid rotation degree, please set to 0, 90, 180 or 270"
#elif LVGL_PORT_ROTATION_DEGREE != 0
    #ifdef LVGL_PORT_DISP_BUFFER_NUM
        #undef LVGL_PORT_DISP_BUFFER_NUM
        #define LVGL_PORT_DISP_BUFFER_NUM           (3)
    #endif
#endif
#endif /* LVGL_PORT_AVOID_TEARING_MODE */

// *INDENT-OFF*

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Porting LVGL with LCD and touch panel. This function should be called after the initialization of the LCD and touch panel.
 *
 * @param lcd The pointer to the LCD panel device, mustn't be nullptr
 * @param tp  The pointer to the touch panel device, set to nullptr if is not used
 *
 * @return true if success, otherwise false
 */
bool lvgl_port_init(ESP_PanelLcd *lcd, ESP_PanelTouch *tp);

/**
 * @brief Lock the LVGL mutex. This function should be called before calling any LVGL APIs when not in LVGL task,
 *        and the `lvgl_port_unlock()` function should be called later.
 *
 * @param timeout_ms The timeout of the mutex lock, in milliseconds. If the timeout is set to `-1`, it will wait indefinitely.
 *
 * @return ture if success, otherwise false
 */
bool lvgl_port_lock(int timeout_ms);

/**
 * @brief Unlock the LVGL mutex. This function should be called after using LVGL APIs when not in LVGL task, and the
 *        `lvgl_port_lock()` function should be called before.
 *
 * @return ture if success, otherwise false
 */
bool lvgl_port_unlock(void);

#ifdef __cplusplus
}
#endif
