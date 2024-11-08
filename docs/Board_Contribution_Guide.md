# Board Contribution Guide

* [中文版本](./Board_Contribution_Guide_CN.md)

## Table of Contents

- [Board Contribution Guide](#board-contribution-guide)
  - [Table of Contents](#table-of-contents)
  - [Contribution Guidelines](#contribution-guidelines)
  - [File Modifications](#file-modifications)
  - [Adaptation Process](#adaptation-process)

## Contribution Guidelines

1. The development board must at least ensure its hardware schematic is open-source, providing a link or file for reference.
2. To maintain compatibility across platforms, this library only supports APIs provided by ESP-IDF. Please do not include or use headers or APIs specific to other platforms, such as Arduino's `Wire`.

**Notes**:

- Before making changes, it is recommended to add the ESP32_Display_Panel repository to your Arduino library directory to facilitate validation in an Arduino project.
- This project uses `pre-commit` to enforce commit standards. When making git commits, `pre-commit` will run automatically, so it is advised to install `pre-commit` beforehand by using the following commands:

   ```bash
   # Install pre-commit
   pip3 install pre-commit && pre-commit install

   # Run pre-commit on all files
   pre-commit run --all-files
   ```

- If you encounter a commit failure, it may be due to `pre-commit` standards. These checks automatically verify and enforce code formatting, style, and other standards. Please confirm and apply any necessary modifications, then re-commit.

## File Modifications

Using the adaption of the [`M5Stack M5DIAL`](https://github.com/esp-arduino-libs/ESP32_Display_Panel/commit/1886c668468626b9dd2ae975f7db12df5413378e) development board as an example. Following this guide, changes below will be made under the project:

```
| -ESP32_Display_Panel
   | -src
      | -board
         | -m5stack                    [A]
            | -M5DIAL.h                [A]
         | -ESP_PanelBoard.h           [M]
         | -README.md                  [M]
      | -ESP_PanelVersions.h           [M]
   | -CHANGELOG.md                     [M]
   | -ESP_Panel_Board_Supported.h      [M]
   | -library.properties               [M]
   | -README_CN.md                     [M]
   | -README.md                        [M]
```
Note: [A] stands for 'append' and [M] stands for 'modify'

## Adaptation Process

Using the adaption of `M5Stack M5DIAL` as an example, follow these steps to modify the relevant files:

1. **[M]** *[ESP_Panel_Board_Supported.h](../ESP_Panel_Board_Supported.h)*: Add a macro for the new development board, such as `BOARD_M5STACK_M5DIAL`. Ensure the macro is in uppercase. Include the manufacturer's name and link, as well as the target development board's link.
2. **[A]** *[src/board/m5stack/M5DIAL.h](../src/board/m5stack/M5DIAL.h)*: Use the *ESP_Panel_Board_Custom.h* file in the root directory as a template to create a new configuration header file for the development board. Follow the naming conventions of existing development boards.
3. **[M]** *[src/board/ESP_PanelBoard.h](../src/board/ESP_PanelBoard.h)*: Add the macro check for the new development board by referring to the existing boards in the file, and modify the file to use the correct header file for this development board.

   **Note**: At this point, you can verify the above steps:

      - Choose an example, such as *[examples/Panel/PanelTest](../examples/Panel/PanelTest/)*.
      - modify the macro `ESP_PANEL_USE_SUPPORTED_BOARD` to enable this header file. Define the development board macro, such as `BOARD_M5STACK_M5DIAL`, to enable the new header file *[src/board/m5stack/M5DIAL.h](../src/board/m5stack/M5DIAL.h)*.
      - Verify the example's ino file. If successful, proceed to the following steps.

4. **[M]** *[src/board/m5stack/M5DIAL.h](../src/board/m5stack/M5DIAL.h)*: Modify the configuration header file for the new development board:
   - Review the hardware schematics of the development board, focusing on the BUS type used for the LCD screen, the LCD driver name, the BUS type used for touch, the touch driver name, and the chip pin numbers used for each interface of the LCD and touch.
   - The best practice is to first get the screen working, then work on the touch functionality.
   - If the new development board's driver is compatible with an existing driver, there is no need to add a new driver. Simply note in the comments that this driver is compatible with an existing one and use the existing driver.
   - If the driver used by the new development board is not compatible with existing drivers or has other special configurations, you can achieve this by modifying the macro functions at the end of the new development board's configuration header file, such as `ESP_PANEL_BEGIN_BACKLIGHT_START_FUNCTION`, `ESP_PANEL_BEGIN_END_FUNCTION`, etc. Refer to *[src/board/espressif/ESP32_S3_BOX_3.h](../src/board/espressif/ESP32_S3_BOX_3.h)* or *[src/board/m5stack/M5DIAL.h](../src/board/m5stack/M5DIAL.h)* for specific implementations.
   - Run examples other than *[examples/LCD](../examples/LCD/)* and *[examples/Touch](../examples/Touch/)*, and continuously adjust the configuration header file to ensure correct settings.

5. **[M]** *[ESP_Panel_Board_Supported](../ESP_Panel_Board_Supported.h)*, *[library.properties](../library.properties)*, *[docs/Board_Instructions.md](../docs/Board_Instructions.md)*, *[README_CN.md](../README_CN.md)*, *[README.md](../README.md)*: Update the supported development boards information in these files.
6. **[M]** *[docs/Board_Instructions.md](../docs/Board_Instructions.md)*: Update the recommended configuration for the new development board.
7. **[M]** *[src/ESP_PanelVersions.h](../src/ESP_PanelVersions.h)*: Ensure that the version under `Library Version` should be ahead of the latest tag version in terms of the tag version; when changes occur to *[ESP_Panel_Board_Custom.h](../ESP_Panel_Board_Custom.h)*, *[ESP_Panel_Board_Supported.h](../ESP_Panel_Board_Supported.h)*, and *[ESP_Panel_Conf.h](../ESP_Panel_Conf.h)* in the root directory, ensure that the version number at the end of the corresponding file and at the beginging of *[src/ESP_PanelVersions.h](../src/ESP_PanelVersions.h)* should be ahead of the latest tag version in terms of the minor version.
8. **[M]** *[CHANGELOG.md](../CHANGELOG.md)*: Update the changelog.
