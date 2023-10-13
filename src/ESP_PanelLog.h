/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <assert.h>
#include "ESP_Panel_Conf_Internal.h"
#if ESP_PANEL_ENABLE_LOG
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#endif
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"

#if ESP_PANEL_ENABLE_LOG
#define ESP_PANEL_ENABLE_TAG_DEBUG_LOG() do {   \
        esp_log_level_set(TAG, ESP_LOG_DEBUG);  \
    } while(0)
#else
#define ESP_PANEL_ENABLE_TAG_DEBUG_LOG() do {} while(0)
#endif

#if ESP_PANEL_CHECK_RESULT_ASSERT
#define ESP_PANEL_CHECK_ERR_RET(x, ...)     assert((x) == ESP_OK)
#define ESP_PANEL_CHECK_NULL_RET(x, ...)    assert((x) != NULL)
#define ESP_PANEL_CHECK_FALSE_RET(x, ...)   assert((x) != false)
#else
#if ESP_PANEL_ENABLE_LOG
#define ESP_PANEL_ERROR_CHECK_LOG_FORMAT(err, format)     "[%s] %s(%u): " format, esp_err_to_name(err), __FUNCTION__, __LINE__
#define ESP_PANEL_ERROR_CHECK_LOGE(tag, err, format, ...) ESP_LOGE(tag, ESP_PANEL_ERROR_CHECK_LOG_FORMAT(err, format), ##__VA_ARGS__)
#define ESP_PANEL_OTHER_CHECK_LOG_FORMAT(format)      "%s(%u): " format, __FUNCTION__, __LINE__
#define ESP_PANEL_OTHER_CHECK_LOGE(tag, format, ...)  ESP_LOGE(tag, ESP_PANEL_OTHER_CHECK_LOG_FORMAT(format), ##__VA_ARGS__)
#else
#define ESP_PANEL_ERROR_CHECK_LOGE(tag, err, format, ...) do {} while(0)
#define ESP_PANEL_OTHER_CHECK_LOGE(tag, format, ...) do {} while(0)
#endif

#define ESP_PANEL_CHECK_ERR_RET(x, ret, fmt, ...) do {                    \
        esp_err_t err_rc_ = (x);                                          \
        if (unlikely(err_rc_ != ESP_OK)) {                                \
            ESP_PANEL_ERROR_CHECK_LOGE(TAG, err_rc_, fmt, ##__VA_ARGS__); \
            return ret;                                                   \
        }                                                                 \
    } while(0)

#define ESP_PANEL_CHECK_NULL_RET(x, ret, fmt, ...) do {          \
        if ((x) == NULL) {                                       \
            ESP_PANEL_OTHER_CHECK_LOGE(TAG, fmt, ##__VA_ARGS__); \
            return ret;                                          \
        }                                                        \
    } while(0)

#define ESP_PANEL_CHECK_FALSE_RET(x, ret, fmt, ...) do {         \
        if ((x) == false) {                                      \
            ESP_PANEL_OTHER_CHECK_LOGE(TAG, fmt, ##__VA_ARGS__); \
            return ret;                                          \
        }                                                        \
    } while(0)
#endif
