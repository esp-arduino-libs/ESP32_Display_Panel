/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "ESP_Panel_Conf_Internal.h"
#include "ESP_Panel_Board_Internal.h"

/* Library Version */
#define ESP_PANEL_VERSION_MAJOR 0
#define ESP_PANEL_VERSION_MINOR 1
#define ESP_PANEL_VERSION_PATCH 6

/* File `ESP_Panel_Conf.h` */
#define ESP_PANEL_CONF_VERSION_MAJOR 0
#define ESP_PANEL_CONF_VERSION_MINOR 1
#define ESP_PANEL_CONF_VERSION_PATCH 2

/* File `ESP_Panel_Board_Custom.h` */
#define ESP_PANEL_BOARD_CUSTOM_VERSION_MAJOR 0
#define ESP_PANEL_BOARD_CUSTOM_VERSION_MINOR 2
#define ESP_PANEL_BOARD_CUSTOM_VERSION_PATCH 2

/* File `ESP_Panel_Board_Supported.h` */
#define ESP_PANEL_BOARD_SUPPORTED_VERSION_MAJOR 0
#define ESP_PANEL_BOARD_SUPPORTED_VERSION_MINOR 3
#define ESP_PANEL_BOARD_SUPPORTED_VERSION_PATCH 0

/* Check if the current configuration file version is compatible with the library version */
// File `ESP_Panel_Conf.h`
// If the version is not defined, set it to `0.1.0`
#if !defined(ESP_PANEL_CONF_FILE_VERSION_MAJOR) && \
    !defined(ESP_PANEL_CONF_FILE_VERSION_MINOR) && \
    !defined(ESP_PANEL_CONF_FILE_VERSION_PATCH)
#define ESP_PANEL_CONF_FILE_VERSION_MAJOR 0
#define ESP_PANEL_CONF_FILE_VERSION_MINOR 1
#define ESP_PANEL_CONF_FILE_VERSION_PATCH 0
#endif
// Check if the current configuration file version is compatible with the library version
#if ESP_PANEL_CONF_FILE_VERSION_MAJOR != ESP_PANEL_CONF_VERSION_MAJOR
#error "The file `ESP_Panel_Conf.h` version is not compatible. Please update it with the file from the library"
#elif ESP_PANEL_CONF_FILE_VERSION_MINOR < ESP_PANEL_CONF_VERSION_MINOR
#warning "The file `ESP_Panel_Conf.h` version is outdated. Some new configurations are missing"
#elif ESP_PANEL_CONF_FILE_VERSION_PATCH > ESP_PANEL_VERSION_PATCH
#warning "The file `ESP_Panel_Conf.h` version is newer than the library. Some new configurations are not supported"
#endif /* ESP_PANEL_CONF_INCLUDE_INSIDE */

// File `ESP_Panel_Board_Custom.h` & `ESP_Panel_Board_Supported.h`
#ifdef ESP_PANEL_USE_BOARD
/* For using a supported board */
#if CONFIG_ESP_PANEL_USE_SUPPORTED_BOARD || ESP_PANEL_USE_SUPPORTED_BOARD
// If the version is not defined, set it to `0.1.0`
#if !defined(ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR) && \
    !defined(ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR) && \
    !defined(ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH)
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR 0
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR 1
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH 0
#endif
// Check if the current configuration file version is compatible with the library version
#if ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR != ESP_PANEL_BOARD_SUPPORTED_VERSION_MAJOR
#error "The file `ESP_Panel_Board_Supported.h` version is not compatible. Please update it with the file from the library"
#elif ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR < ESP_PANEL_BOARD_SUPPORTED_VERSION_MINOR
#warning "The file `ESP_Panel_Board_Supported.h` version is outdated. Some new configurations are missing"
#elif ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH > ESP_PANEL_BOARD_SUPPORTED_VERSION_MINOR
#warning "The file `ESP_Panel_Board_Supported.h` version is newer than the library. Some new configurations are not supported"
#endif

#else /* For using a custom board */

// If the version is not defined, set it to `0.1.0`
#if !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR) && \
    !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR) && \
    !defined(ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH)
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0
#endif
// Check if the current configuration file version is compatible with the library version
#if ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR != ESP_PANEL_BOARD_CUSTOM_VERSION_MAJOR
#error "The file `ESP_Panel_Board_Custom.h` version is not compatible. Please update it with the file from the library"
#elif ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR < ESP_PANEL_BOARD_CUSTOM_VERSION_MINOR
#warning "The file `ESP_Panel_Board_Custom.h` version is outdated. Some new configurations are missing"
#elif ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH > ESP_PANEL_BOARD_CUSTOM_VERSION_PATCH
#warning "The file `ESP_Panel_Board_Custom.h` version is newer than the library. Some new configurations are not supported"
#endif
#endif /* CONFIG_ESP_PANEL_USE_SUPPORTED_BOARD || ESP_PANEL_USE_SUPPORTED_BOARD */
#endif /* ESP_PANEL_USE_BOARD */
