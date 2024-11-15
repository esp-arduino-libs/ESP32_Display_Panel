/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <iostream>
#include <memory>
#include <cstdlib>
#include "ESP_Panel_Conf_Internal.h"
#include "esp_panel_log.hpp"
#if ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_GENERAL_ALLOC_TYPE_ESP
#include "esp_heap_caps.h"
#elif ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_GENERAL_ALLOC_TYPE_CUSTOM
#include ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_INCLUDE
#endif

namespace esp_panel {

template <typename T>
struct MemoryAllocator {
    using value_type = T;

    MemoryAllocator() = default;

    template <typename U>
    MemoryAllocator(const MemoryAllocator<U> &) {}

    T *allocate(std::size_t n)
    {
        if (n == 0) {
            return nullptr;
        }
        void *ptr = std::malloc(n * sizeof(T));
        if (!ptr) {
            throw std::bad_alloc();
        }
        return static_cast<T *>(ptr);
    }

    void deallocate(T *p, std::size_t n)
    {
        free(p);
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&... args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U *p)
    {
        p->~U();
    }

    static void *malloc(size_t size)
    {
        void *p = nullptr;
#if ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_MICROPYTHON
#elif ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_ESP
        p = heap_caps_malloc(size, ESP_PANEL_MEM_GENERAL_ALLOC_ESP_CAPS);
#elif ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_CUSTOM
        p = ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_MALLOC(size);
#else
        p = std::malloc(size);
#endif // ESP_PANEL_MEM_GENERAL_ALLOC_TYPE
        LOGD("Malloc @%p: %d", p, static_cast<int>(size));
        return p;
    }

    static void free(void *p)
    {
        LOGD("Free @%p", p);
#if ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_MICROPYTHON
#elif ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_ESP
        heap_caps_free(p);
#elif ESP_PANEL_MEM_GENERAL_ALLOC_TYPE == ESP_PANEL_MEM_ALLOC_TYPE_CUSTOM
        ESP_PANEL_MEM_GENERAL_ALLOC_CUSTOM_FREE(p);
#else
        std::free(p);
#endif // ESP_PANEL_MEM_GENERAL_ALLOC_TYPE
    }
};

/**
 * @brief Helper functions to allocate memory using the memory allocator
 *
 */
#undef malloc
#undef calloc
#undef free
#define malloc(size)    MemoryAllocator<void>::malloc(size)
#define calloc(n, size)                \
    ({                              \
        size_t _size = (size_t)n * size;  \
        void *p = malloc(_size);    \
        if (p != nullptr) {         \
            std::memset(p, 0, _size); \
        }                           \
        p;                          \
    })
#define free(ptr)       MemoryAllocator<void>::free(ptr)

/**
 * @brief Helper function to create a shared pointer using the memory allocator
 *
 */
template <typename T, typename... Args>
std::shared_ptr<T> panel_make_shared(Args &&... args)
{
    return std::allocate_shared<T>(MemoryAllocator<T>(), std::forward<Args>(args)...);
}

} // namespace esp_panel
