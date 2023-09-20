# LVGL - Porting

The example demonstrates how to port LVGL.

## How to Use

To use this example, you need to install `esp-display-panel` (includes its dependent libraries) and `LVGL(v8.3.x)` library first, then follow the steps to configure them.

### Configure esp-display-panel

1. Go to the directory of the installed Arduino libraries.
2. Go to the `esp-display-panel` folder, copy `ESP_Panel_Conf_Template.h` and place it out of `esp-display-panel` folder. It should be at the same level as the `esp-display-panel` folder. Then rename it as `ESP_Panel_Conf.h`. Finally, the layout of the Arduino Libraries folder with `ESP_Panel_Conf.h` should look like this:

    ```
    Arduino
        |-libraries
            |-esp-display-panel
            |-other_lib_1
            |-other_lib_2
            |-ESP_Panel_Conf.h
    ```

3. Open `ESP_Panel_Conf.h` and uncomment one of the following macros to select an supported development board. Taking `ESP32_S3_BOX` as an example:

    ```c
    // #define ESP_PANEL_BOARD_ESP32_C3_LCDKIT
    #define ESP_PANEL_BOARD_ESP32_S3_BOX
    // #define ESP_PANEL_BOARD_ESP32_S3_BOX_LITE
    ...
    ```

### Configure LVGL

1. Go to the directory of the installed Arduino libraries
2. Go to the `lvgl` folder, copy `lv_conf_template.h` and place it out of `lvgl` folder. It should be at the same level as the `lvgl` folder. Then rename it as `lv_conf.h`. Finally, the layout of the Arduino Libraries folder with `lv_conf.h` should look like this:

    ```
    Arduino
        |-libraries
            |-lv_conf.h
            |-lvgl
            |-other_lib_1
            |-other_lib_2
    ```

3. Open `lv_conf.h` and change the first `#if 0` to `#if 1` to enable the content of the file.
4. Set the following configurations:

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

5. See more details [here](https://docs.lvgl.io/8.3/get-started/platforms/arduino.html).

### Configure Board

Below are recommended configurations for developing GUI applications on various development boards. These settings can be adjusted based on specific requirements.

Go to the `Tools` in Arduino IDE to configure the following settings:

|    Supported Boards     |  Selected Board:   |  PSRAM:  | Flash Mode: | Flash Size: | USB CDC On Boot: |    Partition Scheme:    | Core Debug Level: |
| :---------------------: | :----------------: | :------: | :---------: | :---------: | :--------------: | :---------------------: | :---------------: |
|     ESP32-C3-LCDkit     | ESP32C3 Dev Module | Disabled |     QIO     | 4MB (32Mb)  |     Enabled      | Default 4MB with spiffs |       Info        |
|      ESP32-S3-Box       |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
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
