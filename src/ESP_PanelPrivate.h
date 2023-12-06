/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <assert.h>
#include "esp_err.h"
#include "esp_check.h"
#include "ESP_Panel_Conf_Internal.h"
#if ESP_PANEL_ENABLE_DEBUG_LOG
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#endif
#include "esp_log.h"

#if ESP_PANEL_ENABLE_DEBUG_LOG
#define ENABLE_TAG_PRINT_DEBUG_LOG() do {        \
        esp_log_level_set(TAG, ESP_LOG_DEBUG); \
    } while(0)
#else
#define ENABLE_TAG_PRINT_DEBUG_LOG() do {        \
        esp_log_level_set(TAG, ESP_LOG_WARN); \
    } while(0)
#endif

#if ESP_PANEL_CHECK_RESULT_ASSERT
#define CHECK_NULL_RETURN(x)                assert(x)
#define CHECK_NULL_GOTO(x, goto_tag)        assert(x)
#define CHECK_FALSE_RETURN(x)               assert(x)
#define CHECK_FALSE_GOTO(x, goto_tag)       assert(x)
#else
// #define ERROR_LOG_FORMAT(format)            "[%s:%u] %s(): ", format, esp_panel::path_to_file_name(__FILE__), \
//                                                                                                 __LINE__, __FUNCTION__
// #define ERROR_CHECK_LOGE(tag, format, ...)  printf(ERROR_LOG_FORMAT(format), ##__VA_ARGS__)

// #define CHECK_ERR_RET(x, ret, fmt, ...) do {           \
//         if ((x) != ESP_OK) {                  \
//             ERROR_CHECK_LOGE(fmt, ##__VA_ARGS__); \
//             return ret;                         \
//         }                                   \
//     } while(0)

// #define CHECK_NULL_RET(x, ret, fmt, ...) do {           \
//         if ((x) == NULL) {                  \
//             ERROR_CHECK_LOGE(fmt, ##__VA_ARGS__); \
//             return ret;                         \
//         }                                   \
//     } while(0)

// #define CHECK_FALSE_RET(x, ret, fmt, ...) do {           \
//         if ((x) == false) {                  \
//             ERROR_CHECK_LOGE(fmt, ##__VA_ARGS__); \
//             return ret;                         \
//         }                                   \
//     } while(0)

// #define CHECK_NULL_RETURN(x, fmt, ...) do {           \
//         if ((x) == NULL) {                  \
//             ERROR_CHECK_LOGE(fmt, ##__VA_ARGS__); \
//             return;                         \
//         }                                   \
//     } while(0)

// #define CHECK_ERR_GOTO(x, goto_tag, fmt, ...) do {           \
//         if ((x) != ESP_OK) {                  \
//             ERROR_CHECK_LOGE(fmt, ##__VA_ARGS__); \
//             goto goto_tag;                  \
//         }                                   \
//     } while(0)

// #define CHECK_NULL_GOTO(x, goto_tag, fmt, ...) do {   \
//         if ((x) == NULL) {                  \
//             ERROR_CHECK_LOGE(fmt, ##__VA_ARGS__); \
//             goto goto_tag;                  \
//         }                                   \
//     } while(0)

// #define CHECK_FALSE_RETURN(x, fmt, ...)  do {         \
//         if (unlikely((x) == false)) {       \
//             ERROR_CHECK_LOGE(fmt, ##__VA_ARGS__); \
//             return;                         \
//         }                                   \
//     } while(0)

// #define CHECK_FALSE_GOTO(x, goto_tag, fmt, ...) do {  \
//         if (unlikely((x) == false)) {       \
//             ERROR_CHECK_LOGE(fmt, ##__VA_ARGS__); \
//             goto goto_tag;                  \
//         }                                   \
//     } while(0)
// #endif /* ESP_PANEL_CHECK_RESULT_ASSERT */

// namespace esp_panel {
// const char *path_to_file_name(const char *path);
// }

#define ERROR_CHECK_LOG_FORMAT(err, format)     "[%s] %s(%u): " format, esp_err_to_name(err), __FUNCTION__, __LINE__
#define ERROR_CHECK_LOGE(tag, err, format, ...) ESP_LOGE(tag, ERROR_CHECK_LOG_FORMAT(err, format), ##__VA_ARGS__)

#define OTHER_CHECK_LOG_FORMAT(format)      "%s(%u): " format, __FUNCTION__, __LINE__
#define OTHER_CHECK_LOGE(tag, format, ...)  ESP_LOGE(tag, OTHER_CHECK_LOG_FORMAT(format), ##__VA_ARGS__)

#define CHECK_ERR_RET(x, ret, fmt, ...) do {           \
        esp_err_t err_rc_ = (x);            \
        if (unlikely(err_rc_ != ESP_OK)) {  \
            ERROR_CHECK_LOGE(TAG, err_rc_, fmt, ##__VA_ARGS__); \
            return ret;                         \
        }                                   \
    } while(0)

#define CHECK_NULL_RET(x, ret, fmt, ...) do {           \
        if ((x) == NULL) {                  \
            OTHER_CHECK_LOGE(TAG, fmt, ##__VA_ARGS__); \
            return ret;                         \
        }                                   \
    } while(0)

#define CHECK_FALSE_RET(x, ret, fmt, ...) do {           \
        if ((x) == false) {                  \
            OTHER_CHECK_LOGE(TAG, fmt, ##__VA_ARGS__); \
            return ret;                         \
        }                                   \
    } while(0)

#define CHECK_ERROR_RETURN(x)  do {         \
        esp_err_t err_rc_ = (x);            \
        if (unlikely(err_rc_ != ESP_OK)) {  \
            OTHER_CHECK_LOGE(TAG, "Check error %d (%s)", err_rc_, esp_err_to_name(err_rc_)); \
            return;                         \
        }                                   \
    } while(0)

#define CHECK_ERROR_GOTO(x, goto_tag) do {  \
        esp_err_t err_rc_ = (x);            \
        if (unlikely(err_rc_ != ESP_OK)) {  \
            OTHER_CHECK_LOGE(TAG, "Check error %d (%s)", err_rc_, esp_err_to_name(err_rc_)); \
            goto goto_tag;                  \
        }                                   \
    } while(0)

#define CHECK_NULL_RETURN(x) do {           \
        if ((x) == NULL) {                  \
            OTHER_CHECK_LOGE(TAG, "Check NULL"); \
            return;                         \
        }                                   \
    } while(0)

#define CHECK_NULL_GOTO(x, goto_tag) do {   \
        if ((x) == NULL) {                  \
            OTHER_CHECK_LOGE(TAG, "Check NULL"); \
            goto goto_tag;                  \
        }                                   \
    } while(0)

#define CHECK_FALSE_RETURN(x)  do {         \
        if (unlikely((x) == false)) {       \
            OTHER_CHECK_LOGE(TAG, "Check false"); \
            return;                         \
        }                                   \
    } while(0)

#define CHECK_FALSE_GOTO(x, goto_tag) do {  \
        if (unlikely((x) == false)) {       \
            OTHER_CHECK_LOGE(TAG, "Check false"); \
            goto goto_tag;                  \
        }                                   \
    } while(0)

namespace esp_panel {
const char *path_to_file_name(const char *path);
}
#endif
