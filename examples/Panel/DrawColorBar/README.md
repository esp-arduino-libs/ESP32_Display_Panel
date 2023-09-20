# LCD - Draw Color Bar

The example demonstrates how to draw simple color bar.

## How to Use

To use this example, you need to install `esp-display-panel` (includes its dependent libraries) library first, then follow the steps to configure.

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
Hello Arduino!
I am esp-display-panel.
Initialize panel
Draw color bar from top to bottom, the order is B - G - R
Setup done
Loop
Loop
Loop
Loop
...
```
