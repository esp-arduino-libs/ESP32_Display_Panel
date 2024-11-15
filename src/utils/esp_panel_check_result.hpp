/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_err.h"
#include "ESP_Panel_Conf_Internal.h"
#include "esp_panel_log.hpp"

namespace esp_panel {

#if ESP_PANEL_CHECK_RESULT_ASSERT

#define CHECK_NULL_RETURN(x, ...)       assert(x)
#define CHECK_NULL_GOTO(x,  ...)        assert(x)
#define CHECK_NULL_EXIT(x, ...)         assert(x)

#define CHECK_FALSE_RETURN(x, ...)      assert(x)
#define CHECK_FALSE_GOTO(x, ...)        assert(x)
#define CHECK_FALSE_EXIT(x, ...)        assert(x)

#define CHECK_NOT_ESP_OK_RETURN(x, ...) assert(x)
#define CHECK_NOT_ESP_OK_GOTO(x, ...)   assert(x)
#define CHECK_NOT_ESP_OK_EXIT(x, ...)   assert(x)

#else

#ifndef unlikely
#define unlikely(x)  (x)
#endif

/**
 * @brief Check if the pointer is NULL; if NULL, log an error and return the specified value.
 *
 * @param x Pointer to check
 * @param ret Value to return if the pointer is NULL
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_NULL_RETURN(x, ret, fmt, ...) do { \
            if ((x) == NULL) {                          \
                Log::LOGE(fmt, ##__VA_ARGS__);        \
                return ret;                             \
            }                                           \
        } while(0)

/**
 * @brief Check if the pointer is NULL; if NULL, log an error and goto the specified label.
 *
 * @param x Pointer to check
 * @param goto_tag Label to jump to if the pointer is NULL
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_NULL_GOTO(x, goto_tag, fmt, ...) do { \
            if ((x) == NULL) {                             \
                Log::LOGE(fmt, ##__VA_ARGS__);           \
                goto goto_tag;                             \
            }                                              \
        } while(0)

/**
 * @brief Check if the pointer is NULL; if NULL, log an error and return without a value.
 *
 * @param x Pointer to check
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_NULL_EXIT(x, fmt, ...) do { \
            if ((x) == NULL) {                   \
                Log::LOGE(fmt, ##__VA_ARGS__); \
                return;                          \
            }                                    \
        } while(0)

/**
 * @brief Check if the value is false; if false, log an error and return the specified value.
 *
 * @param x Value to check
 * @param ret Value to return if the value is false
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_FALSE_RETURN(x, ret, fmt, ...) do { \
            if ((x) == false) {                          \
                Log::LOGE(fmt, ##__VA_ARGS__);         \
                return ret;                              \
            }                                            \
        } while(0)

/**
 * @brief Check if the value is false; if false, log an error and goto the specified label.
 *
 * @param x Value to check
 * @param goto_tag Label to jump to if the value is false
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_FALSE_GOTO(x, goto_tag, fmt, ...) do { \
            if (unlikely((x) == false)) {                   \
                Log::LOGE(fmt, ##__VA_ARGS__);            \
                goto goto_tag;                              \
            }                                               \
        } while(0)

/**
 * @brief Check if the value is false; if false, log an error and return without a value.
 *
 * @param x Value to check
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_FALSE_EXIT(x, fmt, ...) do { \
            if ((x) == false) {                   \
                Log::LOGE(fmt, ##__VA_ARGS__);  \
                return;                           \
            }                                     \
        } while(0)

/**
 * @brief Check if the value is not esp_ok; if not, log an error and return the specified value.
 *
 * @param x Value to check
 * @param ret Value to return if the value is false
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_NOT_ESP_OK_RETURN(x, ret, fmt, ...) do { \
            if ((x) != ESP_OK) {                          \
                Log::LOGE(fmt, ##__VA_ARGS__);         \
                return ret;                              \
            }                                            \
        } while(0)

/**
 * @brief Check if the value not esp_ok; if not, log an error and goto the specified label.
 *
 * @param x Value to check
 * @param goto_tag Label to jump to if the value is false
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_NOT_ESP_OK_GOTO(x, goto_tag, fmt, ...) do { \
            if (unlikely((x) != ESP_OK)) {                   \
                Log::LOGE(fmt, ##__VA_ARGS__);            \
                goto goto_tag;                              \
            }                                               \
        } while(0)

/**
 * @brief Check if the value not esp_ok; if not, log an error and return without a value.
 *
 * @param x Value to check
 * @param fmt Format string for the error message
 * @param ... Additional arguments for the format string
 */
#define CHECK_NOT_ESP_OK_EXIT(x, fmt, ...) do { \
            if ((x) != ESP_OK) {                   \
                Log::LOGE(fmt, ##__VA_ARGS__);  \
                return;                           \
            }                                     \
        } while(0)

#endif /* ESP_PANEL_CHECK_RESULT_ASSERT */

}
