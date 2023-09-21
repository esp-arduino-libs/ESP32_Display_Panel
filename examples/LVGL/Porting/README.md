# LVGL - Porting

The example demonstrates how to port LVGL.

## How to Use

To use this example, please firstly install esp-display-panel (includes its dependent libraries) and lvgl (v8.3.x) library, then follow the steps to configure them.

### Configure esp-display-panel

esp-display-panel has its own configuration file called `ESP_Panel_Conf.h`. After installing esp-display-panel, follow these configuration steps:

1. Navigate to the directory where Arduino libraries are installed.
2. Navigate to the `esp-display-panel` folder, copy `ESP_Panel_Conf_Template.h` and place the copy outside the `esp-display-panel` folder at the same directory level. Then rename the copied file as `ESP_Panel_Conf.h`.
3. Finally, the layout of the Arduino Libraries folder with `ESP_Panel_Conf.h` appear as follows:

    ```
    Arduino
        |-libraries
            |-esp-display-panel
            |-other_lib_1
            |-other_lib_2
            |-ESP_Panel_Conf.h
    ```

4. Please refer to [Supported Boards List](https://github.com/esp-arduino-libs/esp-display-panel#supported-boards--drivers) to check if the current board is compatible. If it is compatible, please navigate to the "For Supported Boards" section; Otherwise, navigate to the "For Unsupported Boards" section.

#### For Supported Board

1. Open `ESP_Panel_Conf.h` file. First, set the macro `ESP_PANEL_USE_SUPPORTED_BOARD` to `1` (default is `1`). Then, according to the name of your target development board, uncomment the macro definitions in the format `ESP_PANEL_BOARD_<NAME>` below,
2. The following code takes *ESP32_S3_BOX* development board as an example:

    ```c
    ...
    // #define ESP_PANEL_BOARD_ESP32_C3_LCDKIT
    #define ESP_PANEL_BOARD_ESP32_S3_BOX
    // #define ESP_PANEL_BOARD_ESP32_S3_BOX_LITE
    ...
    ```

#### For Unsupported Board

Please refer to the [document](https://github.com/esp-arduino-libs/esp-display-panel#for-unsupported-board) for the details.

### Configure LVGL

1. Navigate to the directory where Arduino libraries are installed.
2. Navigate to the `lvgl` folder, copy `lv_conf_template.h` and place the copy outside the `lvgl` folder at the same directory level. Then rename the copied file as `lv_conf.h`.
3. Finally, the layout of the Arduino Libraries folder with `lv_conf.h` appear as follows:

    ```
    Arduino
        |-libraries
            |-lv_conf.h
            |-lvgl
            |-other_lib_1
            |-other_lib_2
    ```

4. Open `lv_conf.h` and change the first `#if 0` to `#if 1` to enable the content of the file.
5. Set the following configurations:

    ```c
    #define LV_COLOR_DEPTH          16
    #define LV_COLOR_16_SWAP        1   // This configuration is not for RGB LCD.
                                        // Don't set it if using ESP32-S3-LCD-Ev-Board or ESP32-S3-LCD-Ev-Board-2
    #define LV_MEM_CUSTOM           1
    #define LV_MEMCPY_MEMSET_STD    1
    #define LV_TICK_CUSTOM          1
    #define LV_FONT_MONTSERRAT_12   1
    #define LV_FONT_MONTSERRAT_16   1
    #define LV_USE_DEMO_WIDGETS     1
    #define LV_USE_DEMO_BENCHMARK   1
    #define LV_USE_DEMO_STRESS      1
    #define LV_USE_DEMO_MUSIC       1
    ```

6. For more information, please refer to [LVGL document](https://docs.lvgl.io/8.3/get-started/platforms/arduino.html).

### Configure Board

Below are recommended configurations for developing GUI applications on various development boards. These settings can be adjusted based on specific requirements.

Go to the `Tools` in Arduino IDE to configure the following settings:

|    Supported Boards     |  Selected Board:   |  PSRAM:  | Flash Mode: | Flash Size: | USB CDC On Boot: |    Partition Scheme:    | Core Debug Level: |
| :---------------------: | :----------------: | :------: | :---------: | :---------: | :--------------: | :---------------------: | :---------------: |
|     ESP32-C3-LCDkit     | ESP32C3 Dev Module | Disabled |     QIO     | 4MB (32Mb)  |     Enabled      | Default 4MB with spiffs |       Info        |
|      ESP32-S3-Box       |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
|      ESP32-S3-Box-3     |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
|    ESP32-S3-Box-Lite    |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
|      ESP32-S3-EYE       | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |     8MB     |     Enabled      |     8M with spiffs      |       Info        |
|    ESP32-S3-Korvo-2     | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |     Disabled     |     16M Flash (3MB)     |       Info        |
|  ESP32-S3-LCD-EV-Board  | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
| ESP32-S3-LCD-EV-Board-2 | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
|    ESP32-S3-USB-OTG     |  ESP32-S3-USB-OTG  |    -     |      -      |      -      |        -         |     8M with spiffs      |       Info        |

**Note:**
   1. "USB CDC On Boot" should be enabled according to the using port:
      * Disable it if using **UART port**, enable it if using **USB port**.
      * If it is different in the previous flashing, should enable `Erase All Flash Before Sketch Upload` first, then disable it after flashing.

## Example Output

```bash
...
Hello LVGL! V8.3.8
I am esp-display-panel
Starting LVGL task
Setup done
Loop
Loop
Loop
Loop
...
```
