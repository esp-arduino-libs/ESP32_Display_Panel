/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "board/esp_panel_board_config.hpp"

#if CONFIG_IDF_TARGET_ESP32S31 && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(6, 0, 0)
namespace esp_panel::board {

/**
 * @brief Load the ESP-Mosaico display configuration for the revision stored in eFuse.
 *
 * @return A valid configuration for v1.0, v1.1 and v1.2, or an empty configuration on error.
 * @note Enable the QSPI and I2C buses, CO5300, CST9220 and Custom backlight in the driver factory.
 */
BoardConfig getMosaicoBoardConfig();

} // namespace esp_panel::board
#endif
