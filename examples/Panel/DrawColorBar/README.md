# LCD - Draw Color Bar

The example demonstrates how to draw simple color bar.

## How to Use

To use this example, please firstly install ESP32_Display_Panel and its dependent libraries, then follow the steps to configure:

### Configure ESP32_Display_Panel

ESP32_Display_Panel has its own configuration file called `ESP_Panel_Conf.h`. After installing ESP32_Display_Panel, follow these configuration steps:

1. Navigate to the directory where Arduino libraries are installed.
2. Navigate to the `ESP32_Display_Panel` folder, copy `ESP_Panel_Conf_Template.h` and place the copy outside the `ESP32_Display_Panel` folder at the same directory level. Then rename the copied file as `ESP_Panel_Conf.h`.
3. Finally, the layout of the Arduino Libraries folder with `ESP_Panel_Conf.h` appear as follows:

    ```
    Arduino
        |-libraries
            |-ESP32_Display_Panel
            |-other_lib_1
            |-other_lib_2
            |-ESP_Panel_Conf.h
    ```

4. Please refer to [Supported Boards List](https://github.com/esp-arduino-libs/ESP32_Display_Panel#supported-boards--drivers) to check if the current board is compatible. If it is compatible, please navigate to the "For Supported Boards" section; Otherwise, navigate to the "For Unsupported Boards" section.

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

Please refer to the [document](https://github.com/esp-arduino-libs/ESP32_Display_Panel#for-unsupported-board) for the details.

### Configure Board

Below are recommended configurations for developing GUI applications on various development boards. These settings can be adjusted based on specific requirements.

Go to the `Tools` in Arduino IDE to configure the following settings:

|    Supported Boards     |  Selected Board:   |  PSRAM:  | Flash Mode: | Flash Size: | USB CDC On Boot: |    Partition Scheme:    | Core Debug Level: |
| :---------------------: | :----------------: | :------: | :---------: | :---------: | :--------------: | :---------------------: | :---------------: |
|     ESP32-C3-LCDkit     | ESP32C3 Dev Module | Disabled |     QIO     | 4MB (32Mb)  |     Enabled      | Default 4MB with spiffs |       Info        |
|      ESP32-S3-Box       |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
|      ESP32-S3-Box-3     | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
|    ESP32-S3-Box-3(beta) | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
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
I am ESP32_Display_Panel.
Initialize panel
Draw color bar from top to bottom, the order is B - G - R
Setup done
Loop
Loop
Loop
Loop
...
```
