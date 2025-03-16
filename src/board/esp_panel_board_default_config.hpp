/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "board/esp_panel_board_config.hpp"

extern const esp_panel::board::BoardConfig ESP_PANEL_BOARD_DEFAULT_CONFIG;

#define ESP_PANEL_BOARD_USE_AUDIO 1
#define ESP_PANEL_BOARD_AUDIO_SAMPLE_RATE 44100
#define ESP_PANEL_BOARD_AUDIO_CHANNELS 2
#define ESP_PANEL_BOARD_AUDIO_I2S_BCK_PIN 26
#define ESP_PANEL_BOARD_AUDIO_I2S_WS_PIN 25
#define ESP_PANEL_BOARD_AUDIO_I2S_DATA_OUT_PIN 22
