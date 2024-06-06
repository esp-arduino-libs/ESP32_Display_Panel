# Modification Requirements

1. Newly added development boards must ensure the hardware schematics are open-source. Please provide a link or file.
2. This library currently only supports the APIs provided in ESP-IDF. It does not support other Arduino library APIs, such as Wire.

# Modification Content

Using the adaption of the M5Stack M5CORE2 development board as an example. Following this guide, changes below will be made under the project:

```
| -ESP32_Display_Panel
   | -src
      | -board
         | -m5stack                    [A]
            | -M5CORE2.h               [A]
         | -ESP_PanelBoard             [M]
         | -README.md                  [M]
      | -ESP_PanelVersions.h           [M]
   | -CHANGELOG.md                     [M]
   | -ESP_Panel_Board_Custom.h         
   | -ESP_Panel_Board_Supported.h      [M]
   | -library.properties               [M]
   | -README_CN.md                     [M]
   | -README.md                        [M]
```

# Modification Process

Using the adaption of M5Stack M5CORE2 as an example, follow these steps to modify the relevant files:

1. **[M] ESP_Panel_Board_Supported**: Add a macro for the new development board, such as BOARD_M5STACK_M5CORE2. Ensure the macro is in uppercase. Include the manufacturer's name and link, as well as the target development board's link.
2. **[A] src/board/m5stack/M5CORE2.h**: Use the ESP_Panel_Board_Custom.h file in the root directory as a template to create a new configuration header file for the development board. Follow the naming conventions of existing development boards.
3. **[M] src/board/ESP_PanelBoard.h**: Add the macro check for the new development board by referring to the existing boards in the file, and modify the file to use the correct header file for this development board.

   **Note**: At this point, you can verify the above steps:

      - Choose an example, such as examples/Panel/PanelTest.
      - **[M] examples/Panel/PanelTest/ESP_Panel_Board_Supported.h**: Modify the ESP_PANEL_USE_SUPPORTED_BOARD macro to enable this header file. Define the development board macro, such as BOARD_M5STACK_M5CORE2, to enable the new header file src/board/m5stack/M5CORE2.h.
      - Verify the example's ino file. If successful, proceed to the following steps.

4. Modify the configuration header file for the new development board:
   - Review the hardware schematics of the development board, focusing on the BUS type used for the LCD screen, the LCD driver name, the BUS type used for touch, the touch driver name, and the chip pin numbers used for each interface of the LCD and touch.
   - **[M] src/board/m5stack/M5CORE2.h**: The best practice is to first get the screen working, then work on the touch functionality.
   - If the new development board's driver is compatible with an existing driver, there is no need to add a new driver. Simply note in the comments that this driver is compatible with an existing one and use the existing driver.
   - If the driver used by the new development board is not compatible with existing drivers or has other special configurations, you can achieve this by modifying the macro functions at the end of the new development board's configuration header file, such as ESP_PANEL_BEGIN_BACKLIGHT_START_FUNCTION, ESP_PANEL_BEGIN_END_FUNCTION, etc. Refer to src/board/espressif/ESP32_S3_BOX_3.h or src/board/m5stack/M5CORE2.h for specific implementations.
   - Run examples other than examples/LCD and examples/Touch, and continuously adjust the configuration header file to ensure correct settings.

5. **[M] ESP_Panel_Board_Supported, library.properties, board/README.md, README_CN.md, README.md**: Update the supported development boards information in these files.
6. **[M] src/board/README.md**: Update the recommended configuration for the new development board.
7. **[M] src/ESP_PanelVersions.h**: Ensure that the version under "Library Version" should be ahead of the latest tag version in terms of the minor version; when changes occur to ESP_Panel_Board_Custom.h, ESP_Panel_Board_Supported.h, and ESP_Panel_Conf.h in the root directory, the minor version number at the end of the corresponding file should be increased by one, and it should be ensured that the File ESP_Panel_Conf.h, File ESP_Panel_Board_Custom.h, and File ESP_Panel_Board_Supported.h in src/ESP_PanelVersions.h match the versions of the corresponding files.
8. **[M] CHANGELOG.md**: Update the changelog

# Notes

1. It is recommended to use the vscode + Arduino CLI development environment.
2. Pull the ESP32_Display_Panel repository into the Arduino library directory before making modifications.
3. The project uses pre-commit to enforce commit standards. It is recommended to install the pre-commit library before committing using the following command:

```
pip3 install pre-commit
```