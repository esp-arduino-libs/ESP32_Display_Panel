/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"

#if SOC_MIPI_DSI_SUPPORTED
#include <stdlib.h>
#include <string.h>
#include "ESP_PanelLog.h"
#include "DSI.h"

#define MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV (2500)

static const char *TAG = "ESP_PanelBus_DSI";

ESP_PanelBus_DSI::ESP_PanelBus_DSI(
    uint8_t dsi_lane_num, uint32_t dsi_lane_rate_mbps,
    uint32_t dpi_clk_mhz, uint32_t dpi_bits_per_pixel, uint16_t dpi_w, uint16_t dpi_h,
    uint16_t dpi_hpw, uint16_t dpi_hbp, uint16_t dpi_hfp, uint16_t dpi_vpw, uint16_t dpi_vbp, uint16_t dpi_vfp,
    int phy_ldo_id
):
    ESP_PanelBus(ESP_PANEL_HOST_DSI_ID_DEFAULT, ESP_PANEL_BUS_TYPE_MIPI_DSI, true),
    _phy_ldo_id(phy_ldo_id),
    _dsi_config((esp_lcd_dsi_bus_config_t)ESP_PANEL_HOST_DSI_CONFIG_DEFAULT(dsi_lane_num, dsi_lane_rate_mbps)),
    _dbi_config((esp_lcd_dbi_io_config_t)ESP_PANEL_IO_DBI_CONFIG_DEFAULT()),
    _dpi_config(
        (esp_lcd_dpi_panel_config_t)ESP_PANEL_DPI_CONFIG_DEFAULT(
            dpi_clk_mhz, dpi_bits_per_pixel, dpi_w, dpi_h, dpi_hpw, dpi_hbp, dpi_hfp, dpi_vpw, dpi_vbp, dpi_vfp
        )
    ),
    _phy_ldo_handle(NULL),
    _dsi_handle(NULL)
{
}

ESP_PanelBus_DSI::ESP_PanelBus_DSI(
    uint8_t dsi_lane_num, uint32_t dsi_lane_rate_mbps, const esp_lcd_dpi_panel_config_t &dpi_config, int phy_ldo_id
):
    ESP_PanelBus(ESP_PANEL_HOST_DSI_ID_DEFAULT, ESP_PANEL_BUS_TYPE_MIPI_DSI, true),
    _phy_ldo_id(phy_ldo_id),
    _dsi_config((esp_lcd_dsi_bus_config_t)ESP_PANEL_HOST_DSI_CONFIG_DEFAULT(dsi_lane_num, dsi_lane_rate_mbps)),
    _dbi_config((esp_lcd_dbi_io_config_t)ESP_PANEL_IO_DBI_CONFIG_DEFAULT()),
    _dpi_config(dpi_config),
    _phy_ldo_handle(NULL),
    _dsi_handle(NULL)
{
}

ESP_PanelBus_DSI::ESP_PanelBus_DSI(
    const esp_lcd_dsi_bus_config_t &dsi_config, const esp_lcd_dpi_panel_config_t &dpi_config, int phy_ldo_id
):
    ESP_PanelBus(ESP_PANEL_HOST_DSI_ID_DEFAULT, ESP_PANEL_BUS_TYPE_MIPI_DSI, true),
    _phy_ldo_id(phy_ldo_id),
    _dsi_config(dsi_config),
    _dbi_config((esp_lcd_dbi_io_config_t)ESP_PANEL_IO_DBI_CONFIG_DEFAULT()),
    _dpi_config(dpi_config),
    _phy_ldo_handle(NULL),
    _dsi_handle(NULL)
{
}

ESP_PanelBus_DSI::~ESP_PanelBus_DSI()
{
    if (handle == NULL) {
        goto end;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete panel io failed");
    }

end:
    ESP_LOGD(TAG, "Destroyed");
}

bool ESP_PanelBus_DSI::del(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    ESP_PANEL_CHECK_FALSE_RET(ESP_PanelBus::del(), false, "Delete base panel failed");
    if (_dsi_handle != NULL) {
        if (esp_lcd_del_dsi_bus(_dsi_handle) != ESP_OK) {
            ESP_LOGE(TAG, "Delete _dsi_handle[%d] driver failed", host_id);
        } else {
            ESP_LOGD(TAG, "Delete _dsi_handle[%d] driver", host_id);
        }
        _dsi_handle = NULL;
    }
    if (_phy_ldo_handle != NULL) {
        if (esp_ldo_release_channel(_phy_ldo_handle) != ESP_OK) {
            ESP_LOGE(TAG, "Release LDO channel[%d] failed", _phy_ldo_id);
        } else {
            ESP_LOGD(TAG, "MIPI DSI PHY (LDO %d) Powered off", _phy_ldo_id);
        }
        _phy_ldo_handle = NULL;
    }

    return true;
}

void ESP_PanelBus_DSI::configDpiFrameBufferNumber(uint8_t num)
{
    _dpi_config.num_fbs = num;
}

bool ESP_PanelBus_DSI::begin(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (_phy_ldo_id >= 0) {
        // Turn on the power for MIPI DSI PHY, so it can go from "No Power" state to "Shutdown" state
        esp_ldo_channel_config_t ldo_config = {
            .chan_id = _phy_ldo_id,
            .voltage_mv = MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV,
        };
        ESP_PANEL_CHECK_ERR_RET(esp_ldo_acquire_channel(&ldo_config, &_phy_ldo_handle), false, "Acquire LDO channel failed");
        ESP_LOGD(TAG, "MIPI DSI PHY (LDO %d) Powered on", _phy_ldo_id);
    }
    if (flags.host_need_init) {
        ESP_PANEL_CHECK_ERR_RET(esp_lcd_new_dsi_bus(&_dsi_config, &_dsi_handle), false, "Initialize Host[%d] failed", host_id);
        ESP_LOGD(TAG, "Init MIPI DSI _dsi_handle[%d]", host_id);
    }
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_new_panel_io_dbi(_dsi_handle, &_dbi_config, &handle), false, "Create panel io failed");
    ESP_LOGD(TAG, "Create panel io @%p", handle);

    return true;
}
#endif /* SOC_MIPI_DSI_SUPPORTED */
