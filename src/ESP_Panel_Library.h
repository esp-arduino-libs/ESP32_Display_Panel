/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

/* Drivers */
#include "ESP_Panel_Conf_Internal.h"
#include "ESP_Panel_Board_Internal.h"
#include "ESP_PanelLog.h"
#include "ESP_PanelTypes.h"
#include "ESP_PanelVersions.h"

/* Host */
#include "host/ESP_PanelHost.h"

/* Bus */
#include "bus/ESP_PanelBus.h"
#include "bus/I2C.h"
#include "bus/SPI.h"
#include "bus/RGB.h"
#include "bus/QSPI.h"

/* LCD */
#include "lcd/ESP_PanelLcd.h"
#include "lcd/EK9716B.h"
#include "lcd/GC9503.h"
#include "lcd/GC9A01.h"
#include "lcd/GC9B71.h"
#include "lcd/ILI9341.h"
#include "lcd/SH8601.h"
#include "lcd/SPD2010.h"
#include "lcd/ST7262.h"
#include "lcd/ST7701.h"
#include "lcd/ST7789.h"
#include "lcd/ST77916.h"
#include "lcd/ST77922.h"

/* Touch */
#include "touch/ESP_PanelTouch.h"
#include "touch/CST816S.h"
#include "touch/FT5x06.h"
#include "touch/GT1151.h"
#include "touch/GT911.h"
#include "touch/ST1633.h"
#include "touch/ST7123.h"
#include "touch/TT21100.h"
#include "touch/XPT2046.h"

/* Backlight */
#include "backlight/ESP_PanelBacklight.h"

/* 3rd-party Libraries */
#include "ESP_IOExpander_Library.h"

/* Panel */
#include "ESP_Panel.h"
