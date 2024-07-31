[![GitHub Release](https://img.shields.io/github/v/release/esp-arduino-libs/ESP32_Display_Panel)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/releases) [![Arduino Lint](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/arduino_lint.yml/badge.svg)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/arduino_lint.yml) [![pre-commit](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/pre-commit.yml/badge.svg)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/pre-commit.yml)

# ESP Display Panel

* [中文版本](./README_CN.md)

ESP32_Display_Panel is an Arduino library designed for ESP SoCs to drive display panels and facilitate rapid GUI development. Users can develop directly for a variety of [supported development boards](docs/Board_Instructions.md) or create custom ones through simple adaptation. Additionally, ESP32_Display_Panel is compatible with various LCD and touch drivers, allowing users to develop using standalone drivers as needed.

ESP32_Display_Panel encapsulates various components from the [Espressif Components Registry](https://components.espressif.com/), requiring development based on [arduino-esp32](https://github.com/espressif/arduino-esp32), and can be directly downloaded from the Arduino IDE.

## Table of Contents

- [ESP Display Panel](#esp-display-panel)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Supported Development Boards and Drivers](#supported-development-boards-and-drivers)
    - [Development Boards](#development-boards)
    - [LCD Controllers](#lcd-controllers)
    - [Touch Controllers](#touch-controllers)
  - [Dependencies and Versions](#dependencies-and-versions)
  - [How to Use](#how-to-use)
    - [Configuration Instructions](#configuration-instructions)
      - [Configuring Drivers](#configuring-drivers)
      - [Using Supported Development Boards](#using-supported-development-boards)
      - [Using Custom Development Boards](#using-custom-development-boards)
    - [Usage Examples](#usage-examples)
      - [Arduino IDE](#arduino-ide)
        - [LCD](#lcd)
        - [Touch](#touch)
        - [Panel](#panel)
        - [LVGL v8](#lvgl-v8)
        - [SquareLine](#squareline)
      - [PlatformIO](#platformio)
  - [Other Relevant Instructions](#other-relevant-instructions)
    - [Configuring Supported Development Boards](#configuring-supported-development-boards)
    - [Configuring LVGL](#configuring-lvgl)
    - [Porting SquareLine Project](#porting-squareline-project)
  - [FAQ](#faq)
    - [Where is the directory for Arduino libraries?](#where-is-the-directory-for-arduino-libraries)
    - [How to Install ESP32\_Display\_Panel in Arduino IDE?](#how-to-install-esp32_display_panel-in-arduino-ide)
    - [Where are the installation directory for arduino-esp32 and the SDK located?](#where-are-the-installation-directory-for-arduino-esp32-and-the-sdk-located)
    - [How to fix screen drift issue when driving RGB LCD with ESP32-S3?](#how-to-fix-screen-drift-issue-when-driving-rgb-lcd-with-esp32-s3)
    - [How to Use ESP32\_Display\_Panel on PlatformIO?](#how-to-use-esp32_display_panel-on-platformio)

## Overview

The functional block diagram of ESP32_Display_Panel is as follows, mainly comprising the following features:

- Supports a variety of official Espressif development boards and third-party boards.
- Supports adaptation for custom development boards.
- Supports the use of standalone device drivers.
- Supports various types of device drivers, including interface buses, LCDs, touch screens, and backlight control.
- Supports dynamic configuration of drivers, such as enabling debug logs.

<div align="center"><img src="docs/_static/block_diagram.png" alt="Block Diagram" width="600"></div>

## Supported Development Boards and Drivers

### Development Boards

Below is a list of [supported development boards](docs/Board_Instructions.md):

| **Manufacturer** | **Board Model** |
| --------------- | --------------- |
| [Espressif](docs/Board_Instructions.md#espressif) | ESP32-C3-LCDkit, ESP32-S3-BOX, ESP32-S3-BOX-3, ESP32-S3-BOX-3B, ESP32-S3-BOX-3(beta), ESP32-S3-BOX-Lite, ESP32-S3-EYE, ESP32-S3-Korvo-2, ESP32-S3-LCD-EV-Board, ESP32-S3-LCD-EV-Board-2, ESP32-S3-USB-OTG |
| [Elecrow](docs/Board_Instructions.md#elecrow) | CrowPanel 7.0" |
| [M5Stack](docs/Board_Instructions.md#m5stack) | M5STACK-M5CORE2, M5STACK-M5DIAL, M5STACK-M5CORES3 |
| [Jingcai](docs/Board_Instructions.md#shenzhen-jingcai-intelligent) | ESP32-4848S040C_I_Y_3 |
| [Waveshare](docs/Board_Instructions.md#waveshare) | ESP32-S3-Touch-LCD-4.3 |

Developers and manufacturers are welcomed to contribute PRs to add more development boards. For detailed instructions, please refer to the [`Board Development Guide`](./docs/Board_Contribution_Guide.md).

### LCD Controllers

Below is a list of [supported LCD controllers](docs/LCD_Controllers.md):

| **Manufacturer** | **Model** |
| --------------- | --------- |
| Fitipower | EK9716B |
| GalaxyCore | GC9A01, GC9B71, GC9503 |
| Ilitek | ILI9341 |
| NewVision | NV3022B |
| Sitronix | ST7262, ST7701, ST7789, ST7796, ST77916, ST77922 |

### Touch Controllers

Below is a list of [supported touch controllers](docs/Touch_Controllers.md):

| **Manufacturer** | **Model** |
| --------------- | --------- |
| Hynitron | CST816S |
| FocalTech | FT5x06 |
| GOODiX | GT911, GT1151 |
| Sitronix | ST7123 |
| Parade | TT21100 |
| Xptek | XPT2046 |

## Dependencies and Versions

| **Dependency** | **Version** |
| -------------- | ----------- |
| [arduino-esp32](https://github.com/espressif/arduino-esp32) | >= v3.0.0-alpha3 |
| [ESP32_IO_Expander](https://github.com/esp-arduino-libs/ESP32_IO_Expander) | >= 0.0.1 && < 0.1.0 |

## How to Use

For installation of the ESP32_Display_Panel library, refer to [How to Install ESP32_Display_Panel in Arduino IDE](#how-to-install-esp32_display_panel-in-arduino-ide).

### Configuration Instructions

Below are detailed instructions on how to configure ESP32_Display_Panel, mainly including [Configuring Drivers](#configuring-drivers), [Using Supported Development Boards](#using-supported-development-boards), and [Using Custom Development Boards](#using-custom-development-boards). These are all optional operations and are configured through specified header files. Users can choose to use them according to their needs, with the following characteristics:

1. The path sequence for ESP32_Display_Panel to search for configuration files is: `Current Project Directory` > `Arduino Library Directory` > `ESP32_Display_Panel Directory`.
2. All examples in ESP32_Display_Panel include their required configuration files by default, which users can directly modify macro definitions.
3. For projects without configuration files, users can copy them from the root directory or examples of ESP32_Display_Panel to their own projects.
4. If multiple projects need to use the same configuration, users can place the configuration files in the [Arduino Library Directory](#where-is-the-directory-for-arduino-libraries), so that all projects can share the same configuration.

> [!WARNING]
> * The same directory can simultaneously contain both `ESP_Panel_Board_Supported.h` and `ESP_Panel_Board_Custom.h` configuration files, but they cannot be enabled at the same time, meaning `ESP_PANEL_USE_SUPPORTED_BOARD` and `ESP_PANEL_USE_CUSTOM_BOARD` can only have one set to `1`.
> * If neither of the above two configuration files is enabled, users cannot use the `ESP_Panel` driver and can only use other standalone device drivers, such as `ESP_PanelBus`, `ESP_PanelLcd`, etc.
> * Since the configurations within these files might change, such as adding, deleting, or renaming, to ensure the compatibility of the program, the library manages the versions of these files independently and checks whether the configuration files currently used by the user are compatible with the library during compilation. Detailed version information and checking rules can be found at the end of the file.

#### Configuring Drivers

ESP32_Display_Panel configures driver functionality and parameters based on the [ESP_Panel_Conf.h](./ESP_Panel_Conf.h) file. Users can update the behavior or default parameters of the driver by modifying macro definitions in this file. For example, to enable debug log output, here is a snippet of the modified `ESP_Panel_Conf.h` file:

```c
...
/* Set to 1 if print log message for debug */
#define ESP_PANEL_ENABLE_LOG                (1)         // 0/1
...
```

#### Using Supported Development Boards

ESP32_Display_Panel configures `ESP_Panel` as the driver for the target development board based on the [ESP_Panel_Board_Supported.h](./ESP_Panel_Board_Supported.h) file. Users can select supported development boards by modifying macro definitions in this file. For example, to use the *ESP32-S3-BOX-3* development board, follow these steps:

1. Set the `ESP_PANEL_USE_SUPPORTED_BOARD` macro definition in the `ESP_Panel_Board_Supported.h` file to `1`.
2. Uncomment the corresponding macro definition for the target development board model.

Here is a snippet of the modified `ESP_Panel_Board_Supported.h` file:

```c
...
/* Set to 1 if using a supported board */
#define ESP_PANEL_USE_SUPPORTED_BOARD       (1)         // 0/1

#if ESP_PANEL_USE_SUPPORTED_BOARD
...
// #define BOARD_ESP32_C3_LCDKIT
// #define BOARD_ESP32_S3_BOX
#define BOARD_ESP32_S3_BOX_3
// #define BOARD_ESP32_S3_BOX_3_BETA
...
#endif /* ESP_PANEL_USE_SUPPORTED_BOARD */
```

#### Using Custom Development Boards

ESP32_Display_Panel configures `ESP_Panel` as the driver for custom development boards based on the [ESP_Panel_Board_Custom.h](./ESP_Panel_Board_Custom.h) file. Users need to modify this file according to the actual parameters of the custom development board. For example, to use a custom development board with a *480x480 RGB ST7701 LCD + I2C GT911 Touch*, follow these steps:

1. Set the `ESP_PANEL_USE_CUSTOM_BOARD` macro definition in the `ESP_Panel_Board_Custom.h` file to `1`.
2. Set the LCD-related macro definitions:
   a. Set `ESP_PANEL_USE_LCD` to `1`.
   b. Set `ESP_PANEL_LCD_WIDTH` and `ESP_PANEL_LCD_HEIGHT` to `480`.
   c. Set `ESP_PANEL_LCD_BUS_TYPE` to `ESP_PANEL_BUS_TYPE_RGB`.
   d. Set LCD signal pins and other parameters below `ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB`.
   e. Uncomment and modify the `ESP_PANEL_LCD_VENDOR_INIT_CMD` macro definition according to the initialization command parameters provided by the screen vendor.
   f. Modify other LCD configurations as needed.
3. Set the Touch-related macro definitions:
   a. Set `ESP_PANEL_USE_TOUCH` to `1`.
   b. Set Touch signal pins and other parameters below `ESP_PANEL_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C`.
   c. Modify other Touch configurations as needed.
4. Enable other driver macro definitions as needed, such as `ESP_PANEL_USE_BACKLIGHT`, `ESP_PANEL_USE_EXPANDER`, etc.

Here is a snippet of the modified `ESP_Panel_Board_Custom.h` file:

```c
...
/* Set to 1 if using a custom board */
#define ESP_PANEL_USE_CUSTOM_BOARD  (1)         // 0/1

/* Set to 1 when using an LCD panel */
#define ESP_PANEL_USE_LCD           (1)     // 0/1

#if ESP_PANEL_USE_LCD
/**
 * LCD Controller Name
 */
#define ESP_PANEL_LCD_NAME          ST7701

/* LCD resolution in pixels */
#define ESP_PANEL_LCD_WIDTH         (480)
#define ESP_PANEL_LCD_HEIGHT        (480)
...
/**
 * LCD Bus Type.
 */
#define ESP_PANEL_LCD_BUS_TYPE      (ESP_PANEL_BUS_TYPE_RGB)
/**
 * LCD Bus Parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html and
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html for more details.
 *
 */
#if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
...
#endif /* ESP_PANEL_LCD_BUS_TYPE */
...
/**
 * LCD Vendor Initialization Commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formater: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
#define ESP_PANEL_LCD_VENDOR_INIT_CMD() \
    { \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC2, {0x31, 0x05}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xCD, {0x00}), \
        ...
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x11), \
    }
...
#endif /* ESP_PANEL_USE_LCD */

/* Set to 1 when using a touch panel */
#define ESP_PANEL_USE_TOUCH         (1)         // 0/1
#if ESP_PANEL_USE_TOUCH
/**
 * Touch controller name
 */
#define ESP_PANEL_TOUCH_NAME        GT911
...
/**
 * Touch panel bus type
 */
#define ESP_PANEL_TOUCH_BUS_TYPE    (ESP_PANEL_BUS_TYPE_I2C)
/* Touch panel bus parameters */
#if ESP_PANEL_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
...
#endif /* ESP_PANEL_TOUCH_BUS_TYPE */
...
#endif /* ESP_PANEL_USE_TOUCH */
...
#define ESP_PANEL_USE_BACKLIGHT     (1)         // 0/1
#if ESP_PANEL_USE_BACKLIGHT
...
#endif /* ESP_PANEL_USE_BACKLIGHT */
...
#endif /* ESP_PANEL_USE_CUSTOM_BOARD */
```

### Usage Examples

The following are some examples of using ESP32_Display_Panel on different development platforms.

#### Arduino IDE

You can access them in the Arduino IDE by navigating to `File` > `Examples` > `ESP32_Display_Panel`. If you cannot find the `ESP32_Display_Panel` option, please check if the library has been installed correctly and ensure that an ESP development board is selected.

##### LCD

The following examples demonstrate how to develop different interface and model LCDs using standalone drivers and test them by displaying color bars:

* [SPI](examples/LCD/SPI/)
* [QSPI](examples/LCD/QSPI/)
* [Single RGB](examples/LCD/RGB/)
* [3-wire SPI + RGB](examples/LCD/3wireSPI_RGB/)

##### Touch

The following example demonstrates how to develop touch screens of different interfaces and models using standalone drivers and test them by printing touch point coordinates:

* [I2C](examples/Touch/I2C/)
* [SPI](examples/Touch/SPI/)

##### Panel

The following example demonstrates how to develop built-in or custom development boards using the `ESP_Panel` driver:

* [Panel Test](examples/Panel/PanelTest/): This example tests by displaying color bars and printing touch point coordinates.

##### LVGL v8

For configuring LVGL (v8.3.x), please refer to [here](#configuring-lvgl) for more detailed information.

* [Porting](examples/LVGL/v8/Porting/): This example demonstrates how to port LVGL (v8.3.x). And for RGB LCD, it can enable the avoid tearing function.
* [Rotation](examples/LVGL/v8/Rotation/): This example demonstrates how to use LVGL to rotate the display.

> [!WARNING]
> Currently, the anti-tearing feature is only supported for RGB LCD and requires LVGL version >= v8.3.9. If you are using a different type of LCD or an LVGL version that does not meet the requirements, please do not enable this feature.

##### SquareLine

To port the SquareLine project (v1.3.x), please refer to [here](#porting-squareline-project) for more detailed information.

- [Porting](examples/SquareLine/v8/Porting/): This example demonstrates how to port the SquareLine project.
- [WiFiClock](examples/SquareLine/v8/WiFiClock/): This example implements a simple Wi-Fi clock and can display weather information.

#### PlatformIO

- [PlatformIO](examples/PlatformIO/): This example demonstrates how to use ESP32_Display_Panel in PlatformIO. By default, it is suitable for the **ESP32-S3-LCD-EV-Board** and **ESP32-S3-LCD-EV-Board-2** development boards. Users need to modify the [boards/ESP-LCD.json](examples/PlatformIO/boards/ESP-LCD.json) file according to the actual situation.

## Other Relevant Instructions

### Configuring Supported Development Boards

For details on how to configure the supported development boards in the Arduino IDE, see [Board_Instructions - Recommended Configurations in the Arduino IDE](./docs/Board_Instructions.md#recommended-configurations-in-the-arduino-ide).

### Configuring LVGL

The functionality and parameters of LVGL can be configured by editing the `lv_conf.h` file, where users can modify macro definitions to update the behavior or default parameters of the driver. Here are some features for configuring LVGL:

1. When using arduino-esp32 v3.x.x version, LVGL will search for the configuration file in the following order: `current project directory` > `Arduino library directory`. If the configuration file is not found, a compilation error indicating the absence of the configuration file will be prompted. Therefore, users need to ensure that at least one directory contains the `lv_conf.h` file.

2. If multiple projects need to use the same configuration, users can place the configuration file in the [Arduino library directory](#where-is-the-directory-for-arduino-libraries), so that all projects can share the same configuration.

Below are detailed steps for sharing the same LVGL configuration:

1. Navigate to the [Arduino library directory](#where-is-the-directory-for-arduino-libraries).

2. Enter the `lvgl` folder, copy the `lv_conf_template.h` file, and place the copy at the same level as the `lvgl` folder. Then, rename the copied file to `lv_conf.h`.

3. Finally, the layout of the Arduino library folder should look like this:

   ```
   Arduino
       |-libraries
           |-lv_conf.h
           |-lvgl
           |-other_lib_1
           |-other_lib_2
   ```

4. Open the `lv_conf.h` file, and change the first `#if 0` to `#if 1` to enable the contents of the file.

5. Set other configurations according to requirements. Here are some examples of common configuration options for LVGL v8:

   ```c
   #define LV_COLOR_DEPTH          16  // Typically use 16-bit color depth (RGB565),
                                       // but can also set it to `32` to support 24-bit color depth (RGB888)
   #define LV_COLOR_16_SWAP        0   // If using SPI/QSPI LCD (e.g., ESP32-C3-LCDkit), set this to `1`
   #define LV_COLOR_SCREEN_TRANSP  1
   #define LV_MEM_CUSTOM           1
   #define LV_MEMCPY_MEMSET_STD    1
   #define LV_TICK_CUSTOM          1
   #define LV_ATTRIBUTE_FAST_MEM   IRAM_ATTR
                                      // Get higher performance but use more SRAM
   #define LV_FONT_MONTSERRAT_N    1  // Enable all internal fonts needed (`N` should be replaced with font size)
   ```

6. For more information, please refer to the [LVGL official documentation](https://docs.lvgl.io/8.3/get-started/platforms/arduino.html).

### Porting SquareLine Project

SquareLine Studio (v1.3.x) allows for the rapid design of beautiful UIs through visual editing. If you want to use UI source files exported from SquareLine in the Arduino IDE, you can follow these steps for porting:

1. First, create a new project in SquareLine Studio. Go to `Create` -> `Arduino`, select `Arduino with TFT-eSPI` as the project template, then configure the LCD properties for the target development board in the `PROJECT SETTINGS` section, such as `Resolution` and `Color depth`. Finally, click the `Create` button to create the project.

2. For existing projects, you can also click on `File` -> `Project Settings` in the navigation bar to enter the project settings. Then, in the `BOARD PROPERTIES` section, configure `Board Group` as `Arduino` and `Board` as `Arduino with TFT-eSPI`. Additionally, configure the LCD properties for the target development board in the `DISPLAY PROPERTIES` section. Finally, click the `Save` button to save the project settings.

3. Once the UI design is complete and the export path is configured, click on `Export` -> `Create Template Project` and `Export UI Files` buttons in the menu bar to export the project and UI source files. The layout of the project directory will be as follows:

    ```
    Project
        |-libraries
            |-lv_conf.h
            |-lvgl
            |-readme.txt
            |-TFT_eSPI
            |-ui
        |-README.md
        |-ui
    ```

4. Copy the `lv_conf.h`, `lvgl`, and `ui` folders from the `libraries` folder in the project directory to the Arduino library directory. If you need to use a locally installed `lvgl`, skip copying `lvgl` and `lv_conf.h`, then refer to the steps in the [LVGL Configuration](#configuring-lvgl) section to configure LVGL. The layout of the Arduino library folder will be as follows:

    ```
    Arduino
        |-libraries
            |-ESP32_Display_Panel
            |-ESP_Panel_Conf.h (optional)
            |-lv_conf.h (optional)
            |-lvgl
            |-ui
            |-other_lib_1
            |-other_lib_2
    ```

## FAQ

### Where is the directory for Arduino libraries?

You can find and modify the directory path for Arduino libraries by selecting `File` > `Preferences` > `Settings` > `Sketchbook location` from the menu bar in the Arduino IDE.

### How to Install ESP32_Display_Panel in Arduino IDE?

- If you want to install online, navigate to `Sketch` > `Include Library` > `Manage Libraries...` in the Arduino IDE, then search for `ESP32_Display_Panel` and click the `Install` button to install it.
- If you want to install manually, download the required version of the `.zip` file from [ESP32_Display_Panel](https://github.com/esp-arduino-libs/ESP32_Display_Panel), then navigate to `Sketch` > `Include Library` > `Add .ZIP Library...` in the Arduino IDE, select the downloaded `.zip` file, and click `Open` to install it.
- You can also refer to the guides on library installation in the [Arduino IDE v1.x.x](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries) or [Arduino IDE v2.x.x](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library) documentation.

### Where are the installation directory for arduino-esp32 and the SDK located?

The default installation path for arduino-esp32 depends on your operating system:

- Windows: `C:\Users\<user name>\AppData\Local\Arduino15\packages\esp32`
- Linux: `~/.arduino15/packages/esp32`

The SDK for arduino-esp32 v3.x.x version is located in the `tools/esp32-arduino-libs/idf-release_x` directory within the default installation path.

### How to fix screen drift issue when driving RGB LCD with ESP32-S3?

When encountering screen drift issue when driving RGB LCD with ESP32-S3, you can follow these steps to resolve them:

1. **Refer to Documentation**: Understand the issue description in detail, you can refer to [this document](https://docs.espressif.com/projects/esp-faq/en/latest/software-framework/peripherals/lcd.html#why-do-i-get-drift-overall-drift-of-the-display-when-esp32-s3-is-driving-an-rgb-lcd-screen).

2. **Enable `Bounce Buffer + XIP on PSRAM` Feature**: To resolve the issue, it's recommended to enable the `Bounce Buffer + XIP on PSRAM` feature. Follow these steps:

   - **Step 1**: Download the "high_perf" version of the SDK from [arduino-esp32-sdk](https://github.com/esp-arduino-libs/arduino-esp32-sdk) and replace it in the [installation directory of arduino-esp32](#where-are-the-installation-directory-for-arduino-esp32-and-the-sdk-located).

   - **Step 2**: If you're using supported development boards, usually there's no need to modify the code as they set `ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE` to `(ESP_PANEL_LCD_WIDTH * 10)` by default. If the issue persists, refer to the example code below to increase the size of the `Bounce Buffer`.

   - **Step 3**: If you're using a custom board, confirm in the `ESP_Panel_Board_Custom.h` file whether `ESP_PANEL_LCD_RGB_BOUNCE_BUF_SIZE` is set to non-zero. If the issue persists, increase the size of the `Bounce Buffer`.

   - **Step 4**: If you're using an independent driver, refer to the example code below to set the size of the `Bounce Buffer`.

3. **Example Code**: The following example code demonstrates how to modify the size of the `Bounce Buffer` using `ESP_Panel` driver or independent driver:

   **Example 1**: Modify the `Bounce Buffer` size using the `ESP_Panel` driver:

    ```c
    ...
    ESP_Panel *panel = new ESP_Panel();
    panel->init();
    // Start
    ESP_PanelBus_RGB *rgb_bus = static_cast<ESP_PanelBus_RGB *>(panel->getLcd()->getBus());
    // The size of the bounce buffer must satisfy `width_of_lcd * height_of_lcd = size_of_buffer * N`, where N is an even number.
    rgb_bus->configRgbBounceBufferSize((ESP_PANEL_LCD_WIDTH * 20));
    // End
    panel->begin();
    ...
    ```

   **Example 2**: Modify the `Bounce Buffer` size using an independent driver:

    ```c
    ...
    ESP_PanelBus_RGB *lcd_bus = new ESP_PanelBus_RGB(...);
    // Start
    // The size of the bounce buffer must satisfy `width_of_lcd * height_of_lcd = size_of_buffer * N`, where N is an even number.
    lcd_bus->configRgbBounceBufferSize(EXAMPLE_LCD_WIDTH * 10);
    // End
    lcd_bus->begin();
    ...
    ```

### How to Use ESP32_Display_Panel on PlatformIO?

You can refer to the example [PlatformIO](examples/PlatformIO/) to use the ESP32_Display_Panel library in PlatformIO. By default, it is suitable for the **ESP32-S3-LCD-EV-Board** and **ESP32-S3-LCD-EV-Board-2** development boards. You need to modify the [boards/ESP-LCD.json](examples/PlatformIO/boards/ESP-LCD.json) file according to the actual situation.
