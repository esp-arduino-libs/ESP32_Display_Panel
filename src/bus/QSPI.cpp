/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <string.h>
#include "esp_intr_alloc.h"
#include "ESP_PanelPrivate.h"
#include "QSPI.h"

static const char *TAG = "ESP_PanelBus_QSPI";

ESP_PanelBus_QSPI::ESP_PanelBus_QSPI(int cs_io, int sck_io, int d0_io, int d1_io, int d2_io, int d3_io,
                                     spi_host_device_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_QSPI, true),
    host_id(host_id),
    host_config((spi_bus_config_t)ESP_PANEL_HOST_QSPI_CONFIG_DEFAULT(sck_io, d0_io, d1_io, d2_io, d3_io)),
    io_config((esp_lcd_panel_io_spi_config_t)ESP_PANEL_IO_QSPI_CONFIG_DEFAULT(cs_io))
{
}

ESP_PanelBus_QSPI::ESP_PanelBus_QSPI(int cs_io, spi_host_device_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_QSPI, false),
    host_id(host_id),
    io_config((esp_lcd_panel_io_spi_config_t)ESP_PANEL_IO_QSPI_CONFIG_DEFAULT(cs_io))
{
}

ESP_PanelBus_QSPI::ESP_PanelBus_QSPI(const spi_bus_config_t &host_config, const esp_lcd_panel_io_spi_config_t &io_config,
                                   spi_host_device_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_QSPI, true),
    host_id(host_id),
    host_config(host_config),
    io_config(io_config)
{
}

ESP_PanelBus_QSPI::ESP_PanelBus_QSPI(const esp_lcd_panel_io_spi_config_t &io_config, spi_host_device_t host_id):
    ESP_PanelBus(ESP_PANEL_BUS_TYPE_QSPI, false),
    host_id(host_id),
    io_config(io_config)
{
}

ESP_PanelBus_QSPI::~ESP_PanelBus_QSPI()
{
    if (handle == NULL) {
        ESP_LOGD(TAG, "Bus is not initialized");
        return;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete panel io failed");
    }

    if (host_need_init) {
        if (spi_bus_free(host_id) != ESP_OK) {
            ESP_LOGE(TAG, "Delete host[%d] driver failed", (int)host_id);
        } else {
            ESP_LOGD(TAG, "Delete host[%d] driver", (int)host_id);
        }
    }

    ESP_LOGD(TAG, "Destory");
}

bool ESP_PanelBus_QSPI::begin(void)
{
    ENABLE_TAG_PRINT_DEBUG_LOG();

    if (host_need_init) {
        CHECK_ERR_RET(spi_bus_initialize(host_id, &host_config, SPI_DMA_CH_AUTO), false,
                      "Initializeost Host[%d] failed", (int)host_id);
        ESP_LOGD(TAG, "Init host[%d]", (int)host_id);
    }

    CHECK_ERR_RET(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)host_id, &io_config, &handle), false,
                  "Create panel io failed");
    ESP_LOGD(TAG, "Create panel io @%p", handle);

    return true;
}
