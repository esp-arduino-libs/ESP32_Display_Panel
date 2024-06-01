# SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0

import os
import sys
import shutil
import re

exclude_dirs = ["./build"]
file_version_macros = [
    {
        "major": "ESP_PANEL_CONF_FILE_VERSION_MAJOR",
        "minor": "ESP_PANEL_CONF_FILE_VERSION_MINOR",
        "patch": "ESP_PANEL_CONF_FILE_VERSION_PATCH"
    },
    {
        "major": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR",
        "minor": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR",
        "patch": "ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH"
    },
    {
        "major": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR",
        "minor": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR",
        "patch": "ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH"
    },
]


def is_in_directory(file_path, directory):
    directory = os.path.realpath(directory)
    file_path = os.path.realpath(file_path)

    return file_path.startswith(directory)


def is_same_file(file1, file2):
    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        file1_content = f1.read()
        file2_content = f2.read()

    return file1_content == file2_content


def replace_files(search_directory, file_path):
    if os.path.dirname(file_path) == "":
        file_path = os.path.join(search_directory, file_path)

    filename = os.path.basename(file_path)
    src_file = os.path.join(search_directory, filename)

    if file_path == src_file:
        for root, dirs, files in os.walk(search_directory):
            need_exclude = False
            for exclude_dir in exclude_dirs:
                if is_in_directory(root, exclude_dir):
                    need_exclude = True
                    break

            if root == search_directory or need_exclude:
                continue

            for file in files:
                if file == filename:
                    dst_file = os.path.join(root, file)
                    if not is_same_file(src_file, dst_file):
                        print(f"Replacing '{dst_file}' with '{src_file}'...")
                        shutil.copy(src_file, dst_file)
    else:
        if not is_same_file(src_file, file_path):
            print(f"Restoring '{file_path}' with '{src_file}'...")
            shutil.copy(src_file, file_path)


def extract_version(file_path):
    if os.path.dirname(file_path) == "":
        file_path = os.path.join(search_directory, file_path)

    print(f"Extracting version from '{file_path}'...")

    file_contents = []
    with open(file_path, 'r') as file:
        file_contents.append(file.readlines())
        for content in file_contents:
            content_str = ''.join(content)

    for version_dict in file_version_macros:
        print(f"Version macros: {version_dict['major']}, {version_dict['minor']}, {version_dict['patch']}")

        major_version = re.search(r'#define ' + version_dict["major"] + r' (\d+)', content_str)
        minor_version = re.search(r'#define ' + version_dict["minor"] + r' (\d+)', content_str)
        patch_version = re.search(r'#define ' + version_dict["patch"] + r' (\d+)', content_str)

        if major_version and minor_version and patch_version:
            return f"{major_version.group(1)}.{minor_version.group(1)}.{patch_version.group(1)}"

    return None


if __name__ == "__main__":
    if len(sys.argv) >= 3:
        search_directory = sys.argv[1]

        for i in range(2, len(sys.argv)):
            file_path = sys.argv[i]
            replace_files(search_directory, file_path)
            versions = extract_version(file_path)
            if versions:
                print(f"Version extracted from '{file_path}': {versions}")

        print("Replacement completed.")
