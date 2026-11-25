/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <esp_display_panel.hpp>
#include "board/supported/mosaico/esp_panel_board_mosaico.hpp"

#if !CONFIG_IDF_TARGET_ESP32S31 || ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(6, 0, 0)
#error "Select ESP32S31 Dev Module in Arduino-ESP32 4.0"
#endif

using esp_panel::board::Board;
using esp_panel::board::getMosaicoBoardConfig;
using esp_panel::drivers::TouchPoint;

static Board *board = nullptr;

void setup()
{
    Serial.begin(115200);
    const auto config = getMosaicoBoardConfig();
    if (!config.isValid()) {
        Serial.println("ESP-Mosaico configuration unavailable");
        return;
    }

    static Board mosaico(config);
    if (!mosaico.begin()) {
        Serial.println("ESP-Mosaico initialization failed");
        mosaico.del();
        return;
    }

    board = &mosaico;
    if (!board->getLCD()->colorBarTest() || !board->getBacklight()->setBrightness(50)) {
        Serial.println("ESP-Mosaico color bars or brightness control failed");
    }
}

void loop()
{
    if (board != nullptr) {
        TouchPoint point;
        if (board->getTouch()->readPoints(&point, 1, 0) > 0) {
            Serial.printf("Touch %d, %d\n", point.x, point.y);
        }
    }
    delay(30);
}
