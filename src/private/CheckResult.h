/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CHECKRESULT_H
#define CHECKRESULT_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "esp_check.h"
#include "esp_log.h"

#include "ESP_Panel_Conf_Internal.h"

#if ESP_PANEL_CHECK_RESULT_ASSERT
#define CHECK_ERROR_RETURN(x)               ESP_ERROR_CHECK(x)
#define CHECK_ERROR_GOTO(x, goto_tag)       ESP_ERROR_CHECK(x)
#define CHECK_NULL_RETURN(x)                assert(x)
#define CHECK_NULL_GOTO(x, goto_tag)        assert(x)
#define CHECK_FALSE_RETURN(x)               assert(x)
#define CHECK_FALSE_GOTO(x, goto_tag)       assert(x)
#else
#define ERROR_CHECK_LOG_FORMAT(format)      "[%s:%u] %s(): " format, esp_panel::path_to_file_name(__FILE__), __LINE__, __FUNCTION__
#define ERROR_CHECK_LOGE(tag, format, ...)  ESP_LOGE(tag, ERROR_CHECK_LOG_FORMAT(format), ##__VA_ARGS__)

#define CHECK_ERROR_RETURN(x)  do {         \
        esp_err_t err_rc_ = (x);            \
        if (unlikely(err_rc_ != ESP_OK)) {  \
            ERROR_CHECK_LOGE(TAG, "Check error %d (%s)", err_rc_, esp_err_to_name(err_rc_)); \
            return;                         \
        }                                   \
    } while(0)

#define CHECK_ERROR_GOTO(x, goto_tag) do {  \
        esp_err_t err_rc_ = (x);            \
        if (unlikely(err_rc_ != ESP_OK)) {  \
            ERROR_CHECK_LOGE(TAG, "Check error %d (%s)", err_rc_, esp_err_to_name(err_rc_)); \
            goto goto_tag;                  \
        }                                   \
    } while(0)

#define CHECK_NULL_RETURN(x) do {           \
        if ((x) == NULL) {                  \
            ERROR_CHECK_LOGE(TAG, "Check NULL"); \
            return;                         \
        }                                   \
    } while(0)

#define CHECK_NULL_GOTO(x, goto_tag) do {   \
        if ((x) == NULL) {                  \
            ERROR_CHECK_LOGE(TAG, "Check NULL"); \
            goto goto_tag;                  \
        }                                   \
    } while(0)

#define CHECK_FALSE_RETURN(x)  do {         \
        if (unlikely((x) == false)) {       \
            ERROR_CHECK_LOGE(TAG, "Check false"); \
            return;                         \
        }                                   \
    } while(0)

#define CHECK_FALSE_GOTO(x, goto_tag) do {  \
        if (unlikely((x) == false)) {       \
            ERROR_CHECK_LOGE(TAG, "Check false"); \
            goto goto_tag;                  \
        }                                   \
    } while(0)

namespace esp_panel {
const char *path_to_file_name(const char *path);
}
#endif

#endif
