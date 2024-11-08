/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "soc/soc_caps.h"

#if SOC_MIPI_DSI_SUPPORTED
#include "esp_ldo_regulator.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_lcd_panel_io.h"
#include "ESP_PanelBus.h"

/**
 * @brief Macro for MIPI DSI bus configuration
 *
 */
#define ESP_PANEL_HOST_DSI_ID_DEFAULT   (0)
#define ESP_PANEL_HOST_DSI_CONFIG_DEFAULT(lane_num, lane_rate_mbps) \
    {                                                     \
        .bus_id = ESP_PANEL_HOST_DSI_ID_DEFAULT,          \
        .num_data_lanes = lane_num,                       \
        .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT,      \
        .lane_bit_rate_mbps = lane_rate_mbps,             \
    }

/**
 * @brief Macro for MIPI DBI panel IO configuration
 *
 */
#define ESP_PANEL_IO_DBI_CONFIG_DEFAULT() \
    { \
        .virtual_channel = 0,         \
        .lcd_cmd_bits = 8,            \
        .lcd_param_bits = 8,          \
    }

/**
 * @brief Macro for MIPI DPI panel configuration
 *
 */
#define ESP_PANEL_DPI_CONFIG_DEFAULT(clk_mhz, bits_per_pixel, w, h, hpw, hbp, hfp, vpw, vbp, vfp) \
    { \
        .virtual_channel = 0, \
        .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT, \
        .dpi_clock_freq_mhz = clk_mhz, \
        .pixel_format = (bits_per_pixel == 16) ?  LCD_COLOR_PIXEL_FORMAT_RGB565 : \
                        ((bits_per_pixel == 18) ? LCD_COLOR_PIXEL_FORMAT_RGB666 : LCD_COLOR_PIXEL_FORMAT_RGB888), \
        .num_fbs = 1, \
        .video_timing = { \
            .h_size = w, \
            .v_size = h, \
            .hsync_pulse_width = hpw, \
            .hsync_back_porch = hbp, \
            .hsync_front_porch = hfp, \
            .vsync_pulse_width = vpw, \
            .vsync_back_porch = vbp, \
            .vsync_front_porch = vfp, \
        }, \
        .flags = { \
            .use_dma2d = true, \
        }, \
    }

/**
 * @brief MIPI-DSI bus object class
 *
 * @note  This class is a derived class of `ESP_PanelBus`, user can use it directly
 */
class ESP_PanelBus_DSI: public ESP_PanelBus {
public:
    /**
     * @brief Construct a MIPI-DSI bus object in a common way, the host_handle will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_HOST_DSI_CONFIG_DEFAULT && ESP_PANEL_IO_DBI_CONFIG_DEFAULT)
     *        to config the bus object, use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param ldo_chan_id  MIPI-DSI CS pin
     */
    ESP_PanelBus_DSI(
        uint8_t dsi_lane_num, uint32_t dsi_lane_rate_mbps,
        uint32_t dpi_clk_mhz, uint32_t dpi_bits_per_pixel, uint16_t dpi_w, uint16_t dpi_h,
        uint16_t dpi_hpw, uint16_t dpi_hbp, uint16_t dpi_hfp, uint16_t dpi_vpw, uint16_t dpi_vbp, uint16_t dpi_vfp,
        int phy_ldo_id = -1
    );

    /**
     * @brief Construct a MIPI-DSI bus object in a common way, the host_handle will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_HOST_DSI_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param sck_io    MIPI-DSI SCK pin
     * @param mosi_io   MIPI-DSI MOSI pin
     * @param miso_io   MIPI-DSI MISO pin
     * @param io_config MIPI-DSI panel IO configuration
     */
    ESP_PanelBus_DSI(
        uint8_t dsi_lane_num, uint32_t dsi_lane_rate_mbps, const esp_lcd_dpi_panel_config_t &dpi_config,
        int phy_ldo_id = -1
    );

    /**
     * @brief Construct a MIPI-DSI bus object in a complex way, the host_handle will be initialized by the driver
     *
     * @note  The `init()` function should be called after this function
     *
     * @param host_config MIPI-DSI host_handle configuration
     * @param io_config   MIPI-DSI panel IO configuration
     * @param host_id     MIPI-DSI host_handle ID, default is `ESP_PANEL_HOST_DSI_ID_DEFAULT`
     */
    ESP_PanelBus_DSI(
        const esp_lcd_dsi_bus_config_t &dsi_config, const esp_lcd_dpi_panel_config_t &dpi_config, int phy_ldo_id = -1
    );

    /**
     * @brief Destroy the MIPI-DSI bus object
     *
     */
    ~ESP_PanelBus_DSI() override;

    /**
     * @brief Delete the bus object, release the resources
     *
     * @return true if success, otherwise false
     */
    bool del(void) override;

    /**
     * @brief Here are some functions to configure the MIPI-DSI bus object. These functions should be called before `begin()`
     *
     */
    void configDpiFrameBufferNumber(uint8_t num);

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     */
    bool begin(void) override;

    esp_lcd_dsi_bus_handle_t getBusHandle(void)
    {
        return _dsi_handle;
    }

    const esp_lcd_dsi_bus_config_t *getDsiConfig(void)
    {
        return &_dsi_config;
    }

    const esp_lcd_dpi_panel_config_t *getDpiConfig(void)
    {
        return &_dpi_config;
    }

private:
    int _phy_ldo_id;
    esp_lcd_dsi_bus_config_t _dsi_config;
    esp_lcd_dbi_io_config_t _dbi_config;
    esp_lcd_dpi_panel_config_t _dpi_config;
    esp_ldo_channel_handle_t _phy_ldo_handle;
    esp_lcd_dsi_bus_handle_t _dsi_handle;
};
#endif /* SOC_MIPI_DSI_SUPPORTED */
