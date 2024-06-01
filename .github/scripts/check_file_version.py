# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import sys
import re

internal_version_file = "src/ESP_PanelVersions.h"
internal_version_macros = [
    {
        "file": "ESP_Panel_Conf.h",
        "macro": {
            "major": "ESP_PANEL_CONF_FILE_VERSION_MAJOR",
            "minor": "ESP_PANEL_CONF_FILE_VERSION_MINOR",
            "patch": "ESP_PANEL_CONF_FILE_VERSION_PATCH"
        },
    },
    {
        "file": "ESP_Panel_Board_Custom.h",
        "macro": {
            "major": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR",
            "minor": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR",
            "patch": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH"
        },
    },
    {
        "file": "ESP_Panel_Board_Supported.h",
        "macro": {
            "major": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR",
            "minor": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR",
            "patch": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH"
        },
    },
]
file_version_macros = [
    {
        "file": "ESP_Panel_Conf.h",
        "macro": {
            "major": "ESP_PANEL_CONF_FILE_VERSION_MAJOR",
            "minor": "ESP_PANEL_CONF_FILE_VERSION_MINOR",
            "patch": "ESP_PANEL_CONF_FILE_VERSION_PATCH"
        },
    },
    {
        "file": "ESP_Panel_Board_Custom.h",
        "macro": {
            "major": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR",
            "minor": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR",
            "patch": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH"
        },
    },
    {
        "file": "ESP_Panel_Board_Supported.h",
        "macro": {
            "major": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR",
            "minor": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR",
            "patch": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH"
        },
    },
]


def extract_version(file_path, version_dict):
    if os.path.dirname(file_path) == "":
        file_path = os.path.join(search_directory, file_path)

    print(f"Extracting version from '{file_path}'...")

    file_contents = []
    content_str = ""
    with open(file_path, 'r') as file:
        file_contents.append(file.readlines())
        for content in file_contents:
            content_str = ''.join(content)

    major_version = re.search(r'#define ' + version_dict["major"] + r' (\d+)', content_str)
    minor_version = re.search(r'#define ' + version_dict["minor"] + r' (\d+)', content_str)
    patch_version = re.search(r'#define ' + version_dict["patch"] + r' (\d+)', content_str)

    filename = os.path.basename(file_path)
    if major_version and minor_version and patch_version:
        return {"name": filename, "version": [{major_version.group(1)}, {minor_version.group(1)}, {patch_version.group(1)}]}

    return None


if __name__ == "__main__":
    if len(sys.argv) >= 3:
        search_directory = sys.argv[1]

        internal_version_path = os.path.join(search_directory, internal_version_file)
        print(f"Searching for version in '{internal_version_path}'...")
        internal_versions = []
        for internal_version_macro in internal_version_macros:
            version = extract_version(internal_version_path, internal_version_macro["macro"])
            if version:
                print(f"Version extracted from '{internal_version_path}': {version}")
                internal_versions.append(version)
            else:
                print(f"Version not found in '{internal_version_path}'")

        # for i in range(2, len(sys.argv)):
        #     file_path = sys.argv[i]

        #     for version_dict in version_macros:
        #         versions = extract_version(file_path)
        #         if versions:
        #             print(f"Version extracted from '{file_path}': {versions}")

        print("Replacement completed.")
