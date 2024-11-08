/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_lcd_types.h"
#include "ESP_PanelTypes.h"

/**
 * @brief Bus object class
 *
 * @note  This class is a base class for all types of bus. Due to it is a virtual class, users cannot use it directly
 */
class ESP_PanelBus {
public:
    /**
     * @brief Construct a new bus object, the `begin()` function should be called after this function
     *
     * @param host_id        The host ID of bus
     * @param bus_type       The type of bus.
     *                          - ESP_PANEL_BUS_TYPE_I2C:  I2C bus
     *                          - ESP_PANEL_BUS_TYPE_SPI:  SPI bus
     *                          - ESP_PANEL_BUS_TYPE_RGB:  RGB bus
     *                          - ESP_PANEL_BUS_TYPE_QSPI: QSPI bus
     * @param flags.host_need_init Whether the host should be initialized inside.
     *                          - true: the host needs to be initialized inside
     *                          - false: the host should been initialized by users
     */
    ESP_PanelBus(int host_id, uint8_t bus_type, bool host_need_init);

    /**
     * @brief Destroy the bus object
     *
     */
    virtual ~ESP_PanelBus() = default;

    /**
     * @brief Here are some functions to configure the bus object. These functions should be called before `begin()`
     *
     */
    void configHostId(int id)
    {
        host_id = id;
    }

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     */
    virtual bool begin(void) = 0;

    /**
     * @brief Delete the bus object, release the resources
     *
     * @return true if success, otherwise false
     */
    virtual bool del(void);

    bool delSkipPanelIO(void)
    {
        flags.del_skip_panel_io = 1;
        return del();
    }

    /**
     * @brief Read the register data
     *
     * @param address   The address of the register
     * @param data      The buffer to store the register data
     * @param data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool readRegisterData(uint32_t address, void *data, uint32_t data_size);

    /**
     * @brief Write the register data
     *
     * @param address   The address of the register
     * @param data      The buffer to store the register data
     * @param data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool writeRegisterData(uint32_t address, const void *data, uint32_t data_size);

    /**
     * @brief Write the color data
     *
     * @param address   The address of the register
     * @param data      The buffer to store the color data
     * @param data_size The size of the data (in bytes)
     *
     * @return true if success, otherwise false
     */
    bool writeColorData(uint32_t address, const void *color, uint32_t color_size);

    /**
     * @brief Get the type of bus
     *
     * @return
     *      - ESP_PANEL_BUS_TYPE_UNKNOWN:  Unknown bus
     *      - ESP_PANEL_BUS_TYPE_SPI:  SPI bus
     *      - ESP_PANEL_BUS_TYPE_RGB:  RGB (DPI)bus
     *      - ESP_PANEL_BUS_TYPE_QSPI: QSPI bus
     *      - ESP_PANEL_BUS_TYPE_I2C:  I2C bus
     *      - ESP_PANEL_BUS_TYPE_I80:  I80 (Parallel/8080/DBI) bus
     */
    uint8_t getType(void);

    /**
     * @brief Get the panel IO handle
     *
     * @return
     *      - NULL:   if fail
     *      - Others: the handle of bus
     */
    [[deprecated("This API is deprecated. Please use `getPanelIO_Handle()` instead.")]]
    esp_lcd_panel_io_handle_t getHandle(void)
    {
        return getPanelIO_Handle();
    }

    /**
     * @brief Get the panel IO handle
     *
     * @return
     *      - NULL:   if fail
     *      - Others: the handle of bus
     */
    esp_lcd_panel_io_handle_t getPanelIO_Handle(void)
    {
        return handle;
    }

protected:
    struct {
        uint8_t host_need_init: 1;
        uint8_t del_skip_panel_io: 1;
    } flags;
    int host_id;
    uint8_t bus_type;
    esp_lcd_panel_io_handle_t handle;
};
