/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <array>
#include <memory>
#include "utils/esp_panel_utils_log.h"
#include "utils/esp_panel_utils_cxx.hpp"

namespace esp_panel::drivers {

/**
 * @brief The base bus host template class implementing a variant of the Singleton pattern
 *
 * @tparam Derived Derived class type
 * @tparam Config Configuration type
 * @tparam N Maximum number of instances allowed
 */
template <class Derived, typename Config, int N>
class Host {
public:
    static_assert(N > 0, "Number of instances must be greater than 0");

    /**
     * @brief Host handle type definition
     */
    using NativeHandle = void *;

    using HostHandle [[deprecated("Deprecated, use `NativeHandle` instead")]] = NativeHandle;

    /**
     * @brief Driver state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,     /*!< Driver is not initialized */
        BEGIN,          /*!< Driver is initialized and ready */
    };

    /**
     * @brief Delete copy constructor and assignment operator
     */
    Host(const Host &) = delete;
    Host &operator=(const Host &) = delete;
    Host(Host &&) = delete;
    Host &operator=(Host &&) = delete;

    /**
     * @brief Virtual destructor
     */
    virtual ~Host() = default;

    /**
     * @brief Startup the host
     *
     * @return `true` if successful, `false` otherwise
     */
    virtual bool begin() = 0;

    /**
     * @brief Get the ID of the host
     *
     * @return Host ID
     */
    int getID() const
    {
        return id_;
    }

    /**
     * @brief Get the native handle of the host
     *
     * @return Host handle
     */
    NativeHandle getNativeHandle() const
    {
        return handle_;
    }

    /**
     * @brief Check if driver has reached specified state
     *
     * @param[in] state State to check against
     * @return `true` if current state >= given state, `false` otherwise
     */
    bool isOverState(State state)
    {
        return (state_ >= state);
    }

    /**
     * @brief Get the number of instances
     *
     * @return Number of instances
     */
    static int getInstanceCount()
    {
        return instances_.size();
    }

    /**
     * @brief Get a instance of the host
     *
     * @param[in] id Host ID
     * @param[in] config Host configuration
     * @return Shared pointer to the derived class instance, nullptr if failed
     */
    static std::shared_ptr<Derived> getInstance(int id, const Config &config);

    /**
     * @brief Get a instance of the host
     *
     * @param[in] id Host ID
     * @return Shared pointer to the derived class instance, nullptr if failed
     */
    static std::shared_ptr<Derived> getInstance(int id);

    /**
     * @brief Try to release the instance
     *
     * @param[in] id Host ID
     * @return `true` if successful, `false` otherwise
     */
    static bool tryReleaseInstance(int id);

protected:
    /**
     * @brief Protected constructor for derived classes
     *
     * @param[in] id Host ID
     * @param[in] config Host configuration
     */
    Host(int id, const Config &config): config_(config), id_(id) {}

    /**
     * @brief Set driver state
     *
     * @param[in] state New state to set
     */
    void setState(State state)
    {
        state_ = state;
    }

    Config config_ = {};                /*!< Host configuration */
    NativeHandle handle_ = nullptr;     /*!< Host native handle */

private:
    /**
     * @brief Calibrate configuration when host already exists
     *
     * @param[in] config New configuration
     * @return `true` if successful, `false` otherwise
     */
    virtual bool calibrateConfig(const Config &config) = 0;

    int id_ = -1;                       /*!< Host ID */
    State state_ = State::DEINIT;       /*!< Current driver state */

    inline static std::array<std::shared_ptr<Derived>, N> instances_;  /*!< Array of host instances */
};

template <class Derived, typename Config, int N>
bool Host<Derived, Config, N>::tryReleaseInstance(int id)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: id(%d)", id);
    ESP_UTILS_CHECK_FALSE_RETURN((size_t)id < instances_.size(), false, "Invalid ID");

    if ((instances_[id] != nullptr) && (instances_[id].use_count() == 1)) {
        instances_[id] = nullptr;
        ESP_UTILS_LOGD("Release host(%d)", id);
    }

    ESP_UTILS_LOG_TRACE_EXIT();

    return true;
}

template <class Derived, typename Config, int N>
std::shared_ptr<Derived> Host<Derived, Config, N>::getInstance(int id, const Config &config)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: id(%d), config(@%p)", id, &config);
    ESP_UTILS_CHECK_FALSE_RETURN((size_t)id < instances_.size(), nullptr, "Invalid host ID");

    if (instances_[id] == nullptr) {
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (instances_[id] = utils::make_shared<Derived>(id, config)), nullptr, "Create instance failed"
        );
        ESP_UTILS_LOGD("No instance exist, create new one(@%p)", instances_[id].get());
    } else {
        ESP_UTILS_LOGD("Instance exist(@%p)", instances_[id].get());

        Config new_config = config;
        ESP_UTILS_CHECK_FALSE_RETURN(
            instances_[id]->calibrateConfig(new_config), nullptr,
            "Calibrate configuration failed, attempt to configure host with a incompatible configuration"
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT();

    return instances_[id];
}

template <class Derived, typename Config, int N>
std::shared_ptr<Derived> Host<Derived, Config, N>::getInstance(int id)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: id(%d)", id);
    ESP_UTILS_CHECK_FALSE_RETURN((size_t)id < instances_.size(), nullptr, "Invalid host ID");

    ESP_UTILS_LOG_TRACE_EXIT();

    return instances_[id];
}

} // namespace esp_panel::drivers
