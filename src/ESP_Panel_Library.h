/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "ESP_Panel.h"
#include "ESP_PanelBacklight.h"
#include "ESP_PanelBus.h"
#include "ESP_PanelLcd.h"
#include "ESP_Panel_Conf_Internal.h"

/* Bus */
#include "bus/I2C.h"
#include "bus/SPI.h"
#include "bus/RGB.h"
#include "bus/QSPI.h"

/* LCD */
#include "lcd/GC9503.h"
#include "lcd/GC9A01.h"
#include "lcd/ILI9341.h"
// #include "lcd/ST7262.h"
#include "lcd/ST7701.h"
// #include "lcd/ST7789.h"
#include "lcd/ST77916.h"
#include "lcd/SPD2010.h"
#include "lcd/SH8601.h"
#include "lcd/GC9B71.h"

/* LCD Touch */
#include "lcd_touch/CST816S.h"
#include "lcd_touch/FT5x06.h"
#include "lcd_touch/GT1151.h"
#include "lcd_touch/GT911.h"
#include "lcd_touch/STMPE610.h"
#include "lcd_touch/TT21100.h"
