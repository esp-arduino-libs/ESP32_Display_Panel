# esp-display-panel

esp-display-panel is a versatile Arduino library designed to drive display panels with ESP32. It is primarily developed for various [Espressif development boards](#espressif-development-boards), and can also be used for custom boards.

esp-display-panel is based on [arduino-esp32](https://github.com/espressif/arduino-esp32) and [Espressif Components Registry](https://components.espressif.com/), with various components encapsulated using C++.

## Overview

The block diagram of esp-display-panel is shown in the figure below, mainly comprising the following features:

* Supports various Espressif development boards.
* Supports custom boards.
* Supports multiple types of drivers, including Bus, LCD, Touch, Backlight.

<div align="center"><img src="docs/_static/block_diagram.png" alt ="Block Diagram" width="600"></div>

## Dependencies Version

| **esp-display-panel** | **arduino-esp32** | **esp-io-expander** |
| :-------------------: | :---------------: | :-----------------: |
|        v0.x.x         |     >= v2.0.9     |      >= v0.0.1      |

## Supported Boards & Drivers

### Espressif Development Boards

|                                                         **Picture**                                                          |                                                              **Name**                                                              |   **LCD Bus**    | **LCD Controller** | **Touch Bus** | **Touch Controller** |
| :--------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------------: | :--------------: | :----------------: | :-----------: | :------------------: |
| <img src="https://docs.espressif.com/projects/esp-dev-kits/en/latest/_images/esp32-c3-lcdkit-isometric-raw.png" width="150"> |          [ESP32-C3-LCDkit](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32c3/esp32-c3-lcdkit/index.html)          |       SPI        |       GC9A01       |       -       |          -           |
|            <img src="https://github.com/espressif/esp-box/raw/master/docs/_static/esp32_s3_box.png" width="150">             |                                  [ESP32-S3-Box](https://github.com/espressif/esp-box/tree/master)                                  |       SPI        |      ILI9342       |      I2C      |       TT21100        |
|          <img src="https://github.com/espressif/esp-box/raw/master/docs/_static/esp32_s3_box_lite.png" width="150">          |                               [ESP32-S3-Box-Lite](https://github.com/espressif/esp-box/tree/master)                                |       SPI        |       ST7789       |       -       |          -           |
| <img src="https://github.com/espressif/esp-who/raw/master/docs/_static/get-started/ESP32-S3-EYE-isometric.png" width="100">  |     [ESP32-S3-EYE](https://github.com/espressif/esp-who/blob/master/docs/en/get-started/ESP32-S3-EYE_Getting_Started_Guide.md)     |       SPI        |       ST7789       |       -       |          -           |
|   <img src="https://docs.espressif.com/projects/esp-adf/en/latest/_images/esp32-s3-korvo-2-v3.0-overview.png" width="150">   | [ESP32-S3-Korvo-2](https://docs.espressif.com/projects/esp-adf/en/latest/design-guide/dev-boards/user-guide-esp32-s3-korvo-2.html) |       SPI        |      ILI9342       |      I2C      |       TT21100        |
| <img src="https://docs.espressif.com/projects/esp-dev-kits/en/latest/_images/ESP32-S3-LCD-EV-Board_480x480.png" width="150"> |    [ESP32-S3-LCD-EV-Board](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/index.html)    | 3-wire SPI + RGB |       GC9503       |      I2C      |        FT5X06        |
| <img src="https://docs.espressif.com/projects/esp-dev-kits/en/latest/_images/ESP32-S3-LCD-EV-Board_800x480.png" width="150"> |   [ESP32-S3-LCD-EV-Board-2](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/index.html)   |       RGB        |     ST7262E43      |      I2C      |        GT1151        |
|   <img src="https://docs.espressif.com/projects/esp-dev-kits/en/latest/_images/pic_product_esp32_s3_otg.png" width="150">    |         [ESP32-S3-USB-OTG](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-usb-otg/index.html)         |       SPI        |       ST7789       |       -       |          -           |

### Bus

|                                                **Bus**                                                 | **Version** |
| ------------------------------------------------------------------------------------------------------ | ----------- |
| I2C                                                                                                    | -           |
| SPI                                                                                                    | -           |
| [3-wire SPI + RGB]((https://components.espressif.com/components/espressif/esp_lcd_panel_io_additions)) | v1.0.0      |

### LCD Controller

|                               **LCD Controller**                               | **Version** |
| ------------------------------------------------------------------------------ | ----------- |
| ST7262                                                                         | -           |
| ST7789                                                                         | -           |
| [GC9503](https://components.espressif.com/components/espressif/esp_lcd_gc9503) | 1.0.0       |
| [GC9A01](https://components.espressif.com/components/espressif/esp_lcd_gc9a01) | 1.0.1       |

### Touch Controller

|                                   **Touch Controller**                                   | **Version** |
| ---------------------------------------------------------------------------------------- | ----------- |
| [esp_lcd_touch](https://components.espressif.com/components/espressif/esp_lcd_touch)     | 1.0.4       |
| [CST816S](https://components.espressif.com/components/espressif/esp_lcd_touch_cst816s)   | 1.0.3       |
| [FT5x06](https://components.espressif.com/components/espressif/esp_lcd_touch_ft5x06)     | 1.0.5~1     |
| [GT1151](https://components.espressif.com/components/espressif/esp_lcd_touch_gt1151)     | 1.0.5~1     |
| [GT911](https://components.espressif.com/components/espressif/esp_lcd_touch_gt911)       | 1.0.7~1     |
| [TT21100](https://components.espressif.com/components/espressif/esp_lcd_touch_tt21100)   | 1.0.7~1     |
| [STMPE610](https://components.espressif.com/components/espressif/esp_lcd_touch_stmpe610) | 1.0.6       |

## How to Use

To use a library in Arduino IDE, please refer to the [docs](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries) for IDE v1.x.x or the [docs](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library) for IDE v2.x.x.

### Examples

Here are some examples of using esp-display-panel. To access them in the Arduino IDE, navigate to `File -> Examples -> esp-display-panel`. **Each example sketch and README offer comprehensive usage instructions. Please review them before compiling.**

* Panel:
  * [Draw Color Bar](examples/Panel/DrawColorBar/): This example demonstrates how to draw simple color bar.
  * [Read Touch Point](examples/Panel/ReadTouchPoint/): This example demonstrates how to read touch point.
* LVGL:
  * [Porting](examples/LVGL/Porting/): This example demonstrates how to port LVGL(v8.3.x).

### Detailed Usage

Here are the details of how to use esp-display-panel.

### Configure esp-display-panel

esp-display-panel has its own configuration file called `ESP_Panel_Conf.h`. When esp-display-panel is installed, follow these configuration steps:

1. Go to the directory of the installed Arduino libraries.
2. Go to the `esp-display-panel` folder, copy `ESP_Panel_Conf_Template.h` and place it out of `esp-display-panel` folder. It should be at the same level as the `esp-display-panel` folder. Then rename it as `ESP_Panel_Conf.h`.

Finally, the layout of the Arduino Libraries folder with `ESP_Panel_Conf.h`  should look like this:

```
Arduino
    |-libraries
        |-esp-display-panel
        |-other_lib_1
        |-other_lib_2
        |-ESP_Panel_Conf.h
```

### Configure Supported Board

Please refer to the list of [Supported Boards](#supported-boards--drivers) to verify whether the current board is compatible.

Open `ESP_Panel_Conf.h` and uncomment one of the following macros to select an supported development board. Taking `ESP32_S3_BOX` as an example:

    ```c
    // #define ESP_PANEL_BOARD_ESP32_C3_LCDKIT
    #define ESP_PANEL_BOARD_ESP32_S3_BOX
    // #define ESP_PANEL_BOARD_ESP32_S3_BOX_LITE
    ```

After that, refer to [Use Library](#use-library) to use the library in the sketch.

### Configure Unsupported Board

Since esp-display-panel library can only utilize the internally supported drivers, please ensure that the LCD, Touch, and required Bus for the custom board are present in the list of [Supported Drivers](#supported-boards--drivers).

Open `ESP_Panel_Conf.h`. Firstly, set the macro `ESP_PANEL_USE_SUPPORTED_BOARD` to `0`, as below:

```c
#define ESP_PANEL_USE_SUPPORTED_BOARD   (0)
```

After that, modify the values of other macros as needed. They represent parameters that can be adjusted for LCD, Touch, and other devices initialization.

Here are some important macros for the **LCD**:

```c
/* Set to 0 if not using LCD */
#define ESP_PANEL_USE_LCD           (0)

/**
 * LCD IC name. Choose one of the following:
 *      - ST7262
 *      - ST7789
 *      - GC9503
 *      - GC9A01
 */
#define ESP_PANEL_LCD_NAME          ST7789

/* LCD resolution in pixels */
#define ESP_PANEL_LCD_H_RES         (320)
#define ESP_PANEL_LCD_V_RES         (240)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_BUS_SKIP_INIT_HOST        (0)
/**
 * LCD bus type. Choose one of the following:
 *      - 0: I2C (not supported yet)
 *      - 1: SPI
 *      - 2: I80 (not supported yet)
 *      - 3: RGB
 */
#define ESP_PANEL_LCD_BUS_TYPE      (1)
/**
 * LCD bus parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html for details.
 */
#define ESP_PANEL_LCD_BUS_HOST_ID           (1)
...

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_LCD_COLOR_BITS    (16)
/*
 * LCD Color Space. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_LCD_COLOR_SPACE   (0)
#define ESP_PANEL_LCD_INEVRT_COLOR  (0)

/* LCD Transformation Flags */
#define ESP_PANEL_LCD_SWAP_XY       (0)
#define ESP_PANEL_LCD_MIRROR_X      (0)
#define ESP_PANEL_LCD_MIRROR_Y      (0)

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_LCD_IO_RST        (-1)
#define ESP_PANEL_LCD_RST_LEVEL     (0)
```

Here are some important macros for the **LCD Touch**:

```c
/* Set to 0 if not using LCD touch */
#define ESP_PANEL_USE_LCD_TOUCH     (0)

/**
 * LCD Touch IC name. Choose one of the following:
 *      - CST816S
 *      - FT5x06
 *      - GT1151
 *      - GT911
 *      - TT21100
 *      - STMPE610
 */
#define ESP_PANEL_LCD_TOUCH_NAME            TT21100

/* LCD Touch resolution in pixels */
#define ESP_PANEL_LCD_TOUCH_H_RES           (ESP_PANEL_LCD_H_RES)
#define ESP_PANEL_LCD_TOUCH_V_RES           (ESP_PANEL_LCD_V_RES)

/* LCD Touch Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_LCD_TOUCH_BUS_SKIP_INIT_HOST  (0)
/**
 * LCD touch bus type. Choose one of the following:
 *      - 0: I2C
 *      - 1: SPI
 */
#define ESP_PANEL_LCD_TOUCH_BUS_TYPE            (0)
/**
 * LCD touch bus parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html for details.
 */
#define ESP_PANEL_LCD_TOUCH_BUS_HOST_ID     (0)
...

/* LCD Touch Transformation Flags */
#define ESP_PANEL_LCD_TOUCH_SWAP_XY         (0)
#define ESP_PANEL_LCD_TOUCH_MIRROR_X        (0)
#define ESP_PANEL_LCD_TOUCH_MIRROR_Y        (0)

/* LCD Touch Other Settings */
#define ESP_PANEL_LCD_TOUCH_IO_RST          (-1)
#define ESP_PANEL_LCD_TOUCH_IO_INT          (-1)
#define ESP_PANEL_LCD_TOUCH_RST_LEVEL       (0)
#define ESP_PANEL_LCD_TOUCH_INT_LEVEL       (0)
```

Here are some important macros for the **backlight**:

```c
#define ESP_PANEL_USE_BL                    (0)

/* IO num of backlight pin */
#define ESP_PANEL_LCD_IO_BL                 (45)

/* If the backlight is on when high level, set to 1; otherwise to 0 */
#define ESP_PANEL_LCD_BL_ON_LEVEL           (1)

/* Set to 1 if use PWM for backlight brightness control. */
#define ESP_PANEL_LCD_BL_USE_PWM            (0)

/**
 *  Backlight LEDC Parameters.
 *
 *  Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/ledc.html for details.
 */
#define ESP_PANEL_LCD_BL_PWM_TIMER          (0)
...
```

### Use Library

Use the library in the sketch, and the following codes show the usage of common APIs:

```
#include <ESP_Panel_Library.h>

// Create an ESP_Panel object
ESP_Panel *panel = new ESP_Panel();

// Initialize and start the ESP_Panel object
panel->init();
panel->begin();

// Get the LCD object and operate it
panel->getLcd()->setCallback(callback, NULL);
panel->getLcd()->drawBitmap(0, 0, width, height, color);

// Get the LCD touch object and operate it
panel->getLcdTouch()->readData();
bool touched = panel->getLcdTouch()->getTouchState();
if(touched) {
    TouchPoint point = panel->getLcdTouch()->getPoint();
    Serial.printf("Touch point: x %d, y %d\n", point.x, point.y);
}

// Get the backlight object and operate it
panel->getBacklight()->on();
panel->getBacklight()->off();
panel->getBacklight()->setBrightness(50);

// Release the ESP_Panel object
delete panel;
```
