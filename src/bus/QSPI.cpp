/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <string.h>
#include "ESP_PanelLog.h"
#include "esp_intr_alloc.h"
#include "QSPI.h"

static const char *TAG = "ESP_PanelBus_QSPI";

ESP_PanelBus_QSPI::ESP_PanelBus_QSPI(int cs_io, int sck_io, int d0_io, int d1_io, int d2_io, int d3_io):
    ESP_PanelBus((int)ESP_PANEL_HOST_SPI_ID_DEFAULT, ESP_PANEL_BUS_TYPE_QSPI, true),
    host_config((spi_bus_config_t)ESP_PANEL_HOST_QSPI_CONFIG_DEFAULT(sck_io, d0_io, d1_io, d2_io, d3_io)),
    io_config((esp_lcd_panel_io_spi_config_t)ESP_PANEL_IO_QSPI_CONFIG_DEFAULT(cs_io))
{
}

ESP_PanelBus_QSPI::ESP_PanelBus_QSPI(int cs_io):
    ESP_PanelBus((int)ESP_PANEL_HOST_SPI_ID_DEFAULT, ESP_PANEL_BUS_TYPE_QSPI, false),
    io_config((esp_lcd_panel_io_spi_config_t)ESP_PANEL_IO_QSPI_CONFIG_DEFAULT(cs_io))
{
}

ESP_PanelBus_QSPI::ESP_PanelBus_QSPI(const spi_bus_config_t &host_config, const esp_lcd_panel_io_spi_config_t &io_config,
                                     spi_host_device_t host_id):
    ESP_PanelBus((int)host_id, ESP_PANEL_BUS_TYPE_QSPI, true),
    host_config(host_config),
    io_config(io_config)
{
}

ESP_PanelBus_QSPI::ESP_PanelBus_QSPI(const esp_lcd_panel_io_spi_config_t &io_config, spi_host_device_t host_id):
    ESP_PanelBus((int)host_id, ESP_PANEL_BUS_TYPE_QSPI, false),
    io_config(io_config)
{
}

ESP_PanelBus_QSPI::~ESP_PanelBus_QSPI()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (handle == NULL) {
        goto end;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete panel io failed");
    }

    if (flags.host_need_init) {
        if (spi_bus_free((spi_host_device_t)host_id) != ESP_OK) {
            ESP_LOGE(TAG, "Delete host[%d] driver failed", host_id);
        } else {
            ESP_LOGD(TAG, "Delete host[%d] driver", host_id);
        }
    }

end:
    ESP_LOGD(TAG, "Destroyed");
}

void ESP_PanelBus_QSPI::configQspiMode(uint8_t mode)
{
    io_config.spi_mode = mode;
}

void ESP_PanelBus_QSPI::configQspiFreqHz(uint32_t hz)
{
    io_config.pclk_hz = hz;
}

void ESP_PanelBus_QSPI::configQspiTransQueueDepth(uint8_t depth)
{
    io_config.trans_queue_depth = depth;
}

bool ESP_PanelBus_QSPI::begin(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (flags.host_need_init) {
        ESP_PANEL_CHECK_ERR_RET(spi_bus_initialize((spi_host_device_t)host_id, &host_config, SPI_DMA_CH_AUTO), false,
                                "Initializeost Host[%d] failed", host_id);
        ESP_LOGD(TAG, "Init host[%d]", host_id);
    }

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)host_id, &io_config, &handle), false,
                            "Create panel io failed");
    ESP_LOGD(TAG, "Panel IO @%p created", handle);

    return true;
}
