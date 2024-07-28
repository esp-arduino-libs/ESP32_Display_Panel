/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>
#include "touch/base/esp_lcd_touch.h"
#include "bus/ESP_PanelBus.h"

/**
 * @brief Touch device default configuration macro
 *
 */
#define ESP_PANEL_TOUCH_CONFIG_DEFAULT(width, height, rst_io, int_io) \
    {                                           \
        .x_max = width,                         \
        .y_max = height,                        \
        .rst_gpio_num = (gpio_num_t)rst_io,     \
        .int_gpio_num = (gpio_num_t)int_io,     \
        .levels = {                             \
            .reset = 0,                         \
            .interrupt = 0,                     \
        },                                      \
        .flags = {                              \
            .swap_xy = 0,                       \
            .mirror_x = 0,                      \
            .mirror_y = 0,                      \
        },                                      \
        .process_coordinates = NULL,            \
        .interrupt_callback = NULL,             \
        .user_data = NULL,                      \
        .driver_data = NULL,                    \
    }

/**
 * @brief The class used for storing touch points
 *
 */
class ESP_PanelTouchPoint {
public:
    ESP_PanelTouchPoint();
    ESP_PanelTouchPoint(uint16_t x, uint16_t y, uint16_t strength);

    bool operator==(ESP_PanelTouchPoint p);
    bool operator!=(ESP_PanelTouchPoint p);

    uint16_t x;
    uint16_t y;
    uint16_t strength;
};

/**
 * @brief The touch device objdec class
 *
 * @note  This class is a base class for all Touchs. Due to it is a virtual class, users cannot use it directly
 */
class ESP_PanelTouch {
public:
    /**
     * @brief Construct a new touch device in a simple way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param width  The width of the touch screen
     * @param height The height of the touch screen
     * @param rst_io The reset pin of the touch screen, set to `-1` if not used
     * @param int_io The interrupt pin of the touch screen, set to `-1` if not used
     */
    ESP_PanelTouch(ESP_PanelBus *bus, uint16_t width, uint16_t height, int rst_io, int int_io);

    /**
     * @brief Construct a new touch device in a complex way, the `init()` function should be called after this function
     *
     * @param bus    Pointer to panel bus
     * @param config Touch device configuration
     */
    ESP_PanelTouch(ESP_PanelBus *bus, const esp_lcd_touch_config_t &config);

    /**
     * @brief Destroy the LCD device
     *
     */
    virtual ~ESP_PanelTouch() = default;

    /**
     * @brief Attach a callback function, which will be called when the refreshing is finished
     *
     * @param callback  The callback function
     * @param user_data The user data which will be passed to the callback function
     *
     * @return true if success, otherwise false
     */
    bool attachInterruptCallback(std::function<bool (void *)> callback, void *user_data = NULL);

    /**
     * @brief Initialize the touch device, the `begin()` function should be called after this function.
     *
     * @note  This function intends to create a semaphore for the interrupt if the interrupt pin is set and the
     *        `interrupt_callback` is not used by the users. If successful, the driver will enable the functionality of
     *        blocking reads.
     *
     * @return true if success, otherwise false
     */
    bool init(void);

    /**
     * @brief Startup the touch device
     *
     * @return true if success, otherwise false
     */
    virtual bool begin(void) = 0;

    /**
     * @brief Delete the touch device, release the resources
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_del()` to delete the touch device
     *
     * @return true if success, otherwise false
     */
    bool del(void);

    /**
     * @brief Swap the X and Y axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_set_swap_xy()` to mirror the axises
     *
     * @param en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool swapXY(bool en);

    /**
     * @brief Mirror the X axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_set_mirror_x()` to mirror the axis
     *
     * @param en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool mirrorX(bool en);

    /**
     * @brief Mirror the Y axis
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_set_mirror_y()` to mirror the axis
     *
     * @param en true: enable, false: disable
     *
     * @return true if success, otherwise false
     */
    bool mirrorY(bool en);

    /**
     * @brief Read the raw data from the touch device, then users should call `getPoints()` to get the points, or call
     *        `getButtonState()` to get the button state
     *
     * @note  This function should be called after `begin()`
     * @note  This function typically calls `esp_lcd_touch_read_data()` and `esp_lcd_touch_get_coordinates()` to read
     *        the raw data
     * @note  If the interrupt pin is set, this function will be blocked until either the interrupt occurs or a timeout
     *        is triggered
     *
     * @param max_points_num The max number of the points to read
     * @param timeout_ms     The timeout of waiting for the interrupt, it is only used when the interrupt pin is set. Set to
     *                       `-1` if waiting forever
     *
     * @return true if success, otherwise false
     */
    bool readRawData(uint8_t max_points_num = CONFIG_ESP_LCD_TOUCH_MAX_POINTS, int timeout_ms = 0);

    /**
     * @brief Get the touch points. This function should be called immediately after the `readRawData()` function
     *
     * @note  This function should be called after `begin()`
     *
     * @param points The buffer to store the points
     * @param num    The number of the points to read
     *
     * @return The number of the points read, `-1` if failed
     */
    int getPoints(ESP_PanelTouchPoint points[], uint8_t num = 1);

    /**
     * @brief Get the button state. This function should be called immediately after the `readRawData()` function
     *
     * @note  This function should be called after `begin()`
     *
     * @param index The index of the button
     *
     * @return The state of the button, `-1` if failed
     */
    int getButtonState(uint8_t index = 0);

    /**
     * @brief Read the points from the touch device. This function is a combination of `readRawData()` and `getPoints()`
     *
     * @note  This function should be called after `begin()`
     * @note  If the interrupt pin is set, this function will be blocked until either the interrupt occurs or a timeout
     *        is triggered
     *
     * @param points     The buffer to store the points
     * @param num        The number of the points to read
     * @param timeout_ms The timeout of waiting for the interrupt, it is only used when the interrupt pin is set. Set to
     *                   `-1` if waiting forever
     *
     * @return The number of the points read, `-1` if failed
     */
    int readPoints(ESP_PanelTouchPoint points[], uint8_t num = 1, int timeout_ms = 0);

    /**
     * @brief Read the button state from the touch device. This function is a combination of `readRawData()` and
     *        `getButtonState()`
     *
     * @note  This function should be called after `begin()`
     * @note  If the interrupt pin is set, this function will be blocked until either the interrupt occurs or a timeout
     *        is triggered
     *
     * @param index      The index of the button
     * @param timeout_ms The timeout of waiting for the interrupt, it is only used when the interrupt pin is set. Set to
     *                   `-1` if waiting forever
     *
     * @return The state of the button, `-1` if failed
     */
    int readButtonState(uint8_t index = 0, int timeout_ms = 0);

    /**
     * @brief Configure the active level of reset signal
     *
     * @param level 1: high level, 0: low level
     */
    void configResetActiveLevel(uint8_t level);

    /**
     * @brief Configure the active level of interrupt signal
     *
     * @param level 1: high level, 0: low level
     */
    void configInterruptActiveLevel(uint8_t level);

    /**
     * @brief Check if the interrupt function is enabled
     *
     * @return true if enabled, otherwise false
     */
    bool isInterruptEnabled(void);

    /**
     * @brief Get the flag of the X and Y axis swap
     *
     * @return true if swap, otherwise not swap
     */
    bool getSwapXYFlag(void);

    /**
     * @brief Get the flag of the X axis mirror
     *
     * @return true if mirror, otherwise not mirror
     */
    bool getMirrorXFlag(void);

    /**
     * @brief Get the flag of the Y axis mirror
     *
     * @return true if mirror, otherwise not mirror
     */
    bool getMirrorYFlag(void);

    /**
     * @brief Get the panel handle of LCD device
     *
     * @return The handle of the LCD panel, or NULL if fail
     */
    esp_lcd_touch_handle_t getHandle(void);

    /**
     * @brief Get the panel bus
     *
     * @return The pointer of the LCD Bus, or NULL if fail
     */
    ESP_PanelBus *getBus(void);

protected:
    ESP_PanelBus *bus;
    esp_lcd_touch_config_t config;
    esp_lcd_touch_handle_t handle;

private:
    static void onTouchInterrupt(esp_lcd_touch_handle_t tp);

    bool _swap_xy;
    bool _mirror_x;
    bool _mirror_y;
    uint8_t _tp_points_num;
    uint8_t _tp_buttons_state[CONFIG_ESP_LCD_TOUCH_MAX_BUTTONS];
    ESP_PanelTouchPoint _tp_points[CONFIG_ESP_LCD_TOUCH_MAX_POINTS];

    std::function<bool (void *)> onTouchInterruptCallback;
    SemaphoreHandle_t _isr_sem;
    typedef struct {
        void *tp_ptr;
        void *user_data;
    } ESP_PanelTouchCallbackData_t;
    ESP_PanelTouchCallbackData_t callback_data;
};
