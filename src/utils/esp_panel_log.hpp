/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <mutex>
#include <iostream>
#include <cstdio>
#include <functional>
#include <cstdarg>
#include <cstring>
#include "ESP_Panel_Conf_Internal.h"

namespace esp_panel {

/**
 * Class to handle logging
 *
 */
class Log {
public:
    // Log level definitions
    enum class OutputLevel {
        DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
        INFO,       /*!< Information messages which describe the normal flow of events */
        WARNING,    /*!< Error conditions from which recovery measures have been taken */
        ERROR,      /*!< Critical errors, software module cannot recover on its own */
    };

    // Singleton pattern: Get the unique instance of the class
    static Log &getInstance()
    {
        static Log instance;
        return instance;
    }

#if ESP_PANEL_ENABLE_LOG
    // Templates and conditional compilation: Filter logs by different levels
    template <OutputLevel level>
    void print(const char *file, int line, const char *func, const char *format, ...)
    {
        // Logs below the global level will not be compiled
        if constexpr (level >= static_cast<OutputLevel>(ESP_PANEL_LOG_GLOBAL_LEVEL)) {
            // Mutex to avoid interleaved log messages
            _mutex.lock();
            // Use variadic arguments for formatted output
            va_list args;
            va_start(args, format);
            vsnprintf(_buffer, sizeof(_buffer), format, args);
            va_end(args);
            // Output log message
            std::cout << "[Panel][" << logLevelToString(level) << "][" << extractFileName(file) << ":" << line << "]("
                      << func << "): " << _buffer << std::endl;
            _mutex.unlock();
        }
    }
#else
    // When logging is disabled, the `print` function has an empty implementation
    template <OutputLevel level>
    void print(const char *, int, const char *, const char *, ...) const {}
#endif /* ESP_PANEL_ENABLE_LOG */

private:
    constexpr Log() {}

#if ESP_PANEL_ENABLE_LOG
    // Convert log level to string
    static const char *logLevelToString(OutputLevel level)
    {
        switch (level) {
        case OutputLevel::DEBUG:   return "DEBUG";
        case OutputLevel::INFO:    return " INFO";
        case OutputLevel::WARNING: return " WARN";
        case OutputLevel::ERROR:   return "ERROR";
        default: break;
        }
        return "UNKNOWN";
    }

    // Extract filename from file path
    static const char *extractFileName(const char *filePath)
    {
        const char *filename = std::strrchr(filePath, '/');
        if (!filename) {
            filename = std::strrchr(filePath, '\\');  // Windows path compatibility
        }
        return filename ? filename + 1 : filePath;
    }

    char _buffer[ESP_PANEL_LOG_BUFFER_SIZE];
    std::mutex _mutex;
#endif
};

/**
 * Macros to simplify logging calls
 *
 */
#define LOGD(format, ...) \
    Log::getInstance().print<Log::OutputLevel::DEBUG>(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define LOGI(format, ...) \
    Log::getInstance().print<Log::OutputLevel::INFO>(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define LOGW(format, ...) \
    Log::getInstance().print<Log::OutputLevel::WARNING>(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#define LOGE(format, ...) \
    Log::getInstance().print<Log::OutputLevel::ERROR>(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

} // namespace esp_panel

/**
 * Macros to replace ESP-IDF logging functions
 *
 */
#ifdef ESP_LOGV
#undef ESP_LOGV
#define ESP_LOGV(TAG, ...) esp_panel::Log::LOGD(__VA_ARGS__)
#endif

#ifdef ESP_LOGD
#undef ESP_LOGD
#define ESP_LOGD(TAG, ...) esp_panel::Log::LOGD(__VA_ARGS__)
#endif

#ifdef ESP_LOGI
#undef ESP_LOGI
#define ESP_LOGI(TAG, ...) esp_panel::Log::LOGI(__VA_ARGS__)
#endif

#ifdef ESP_LOGW
#undef ESP_LOGW
#define ESP_LOGW(TAG, ...) esp_panel::Log::LOGW(__VA_ARGS__)
#endif

#ifdef ESP_LOGE
#undef ESP_LOGE
#define ESP_LOGE(TAG, ...) esp_panel::Log::LOGE(__VA_ARGS__)
#endif
