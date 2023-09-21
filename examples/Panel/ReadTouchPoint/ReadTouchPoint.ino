/**
 * The example demonstrates how to read touch point.
 *
 * ## How to Use
 *
 * To use this example, please firstly install esp-display-panel and its dependent libraries,
 * then follow the steps to configure:
 *
 * ### Configure esp-display-panel
 *
 * esp-display-panel has its own configuration file called `ESP_Panel_Conf.h`. After installing esp-display-panel,
 * follow these configuration steps:
 *
 * 1. Navigate to the directory where Arduino libraries are installed.
 * 2. Navigate to the `esp-display-panel` folder, copy `ESP_Panel_Conf_Template.h` and place the copy outside the
 *    `esp-display-panel` folder at the same directory level. Then rename the copied file as `ESP_Panel_Conf.h`.
 * 3. Finally, the layout of the Arduino Libraries folder with `ESP_Panel_Conf.h` appear as follows:
 *
 * ```
 * Arduino
 *     |-libraries
 *         |-esp-display-panel
 *         |-other_lib_1
 *         |-other_lib_2
 *         |-ESP_Panel_Conf.h
 * ```
 *
 * 4. Please refer to [Supported Boards List](https://github.com/esp-arduino-libs/esp-display-panel#supported-boards--drivers)
 *    to check if the current board is compatible. If it is compatible, please navigate to the "For Supported Boards" section;
 *    Otherwise, navigate to the "For Unsupported Boards" section.
 *
 * #### For Supported Board
 *
 * 1. Open `ESP_Panel_Conf.h` file. First, set the macro `ESP_PANEL_USE_SUPPORTED_BOARD` to `1` (default is `1`).
 *    Then, according to the name of your target development board, uncomment the macro definitions in the format
 *    `ESP_PANEL_BOARD_<NAME>` below,
 * 2. The following code takes *ESP32_S3_BOX* development board as an example:
 *
 *     ```c
 *     ...
 *     // #define ESP_PANEL_BOARD_ESP32_C3_LCDKIT
 *     #define ESP_PANEL_BOARD_ESP32_S3_BOX
 *     // #define ESP_PANEL_BOARD_ESP32_S3_BOX_LITE
 *     ...
 *     ```
 *
 * #### For Unsupported Board
 *
 * Please refer to the [document](https://github.com/esp-arduino-libs/esp-display-panel#for-unsupported-board) for the details.
 *
 * ### Configure Board
 *
 * Below are recommended configurations for developing GUI applications on various development boards.
 * These settings can be adjusted based on specific requirements. Go to the `Tools` in Arduino IDE to configure the following settings:
 *
 * |    Supported Boards     |  Selected Board:   |  PSRAM:  | Flash Mode: | Flash Size: | USB CDC On Boot: |    Partition Scheme:    | Core Debug Level: |
 * | :---------------------: | :----------------: | :------: | :---------: | :---------: | :--------------: | :---------------------: | :---------------: |
 * |     ESP32-C3-LCDkit     | ESP32C3 Dev Module | Disabled |     QIO     | 4MB (32Mb)  |     Enabled      | Default 4MB with spiffs |       Info        |
 * |      ESP32-S3-Box       |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
 * |      ESP32-S3-Box-3     |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
 * |    ESP32-S3-Box-Lite    |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
 * |      ESP32-S3-EYE       | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |     8MB     |     Enabled      |     8M with spiffs      |       Info        |
 * |    ESP32-S3-Korvo-2     | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |     Disabled     |     16M Flash (3MB)     |       Info        |
 * |  ESP32-S3-LCD-EV-Board  | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
 * | ESP32-S3-LCD-EV-Board-2 | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
 * |    ESP32-S3-USB-OTG     |  ESP32-S3-USB-OTG  |    -     |      -      |      -      |        -         |     8M with spiffs      |       Info        |
 *
 * **Note:**
 *      1. "USB CDC On Boot" should be enabled according to the using port:
 *          * Disable it if using **UART port**, enable it if using **USB port**.
 *          * If it is different in the previous flashing, should enable `Erase All Flash Before Sketch Upload` first, then disable it after flashing.
 *
 * ## Example Output
 *
 * ```bash
 * ...
 * Hello Arduino!
 * I am esp-display-panel.
 * Initialize panel
 * Setup done
 * Touch point: x 479, y 476
 * ...
 * ```
 */

#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <ESP_IOExpander_Library.h>

ESP_Panel *panel;

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println("Hello Arduino!");
    Serial.println("I am esp-display-panel.");

#if ESP_PANEL_USE_LCD_TOUCH
    panel = new ESP_Panel();

    /* There are some extral initialization for ESP32-S3-LCD-EV-Board */
#ifdef ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD
    /* Initialize IO expander */
    ESP_IOExpander *expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST_ID, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    expander->init();
    expander->begin();
    /* Add into panel for 3-wire SPI */
    panel->addIOExpander(expander);
    /* For the newest version sub board, need to set `ESP_PANEL_LCD_RGB_IO_VSYNC` to low before initialize LCD */
    pinMode(ESP_PANEL_LCD_RGB_IO_VSYNC, OUTPUT);
    digitalWrite(ESP_PANEL_LCD_RGB_IO_VSYNC, HIGH);
#endif

    /* There are some extral initialization for ESP32-S3-Korvo-2 */
#ifdef ESP_PANEL_BOARD_ESP32_S3_KORVO_2
    /* Initialize IO expander */
    ESP_IOExpander *expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST_ID, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    expander->init();
    expander->begin();
    /* Reset LCD */
    expander->pinMode(2, OUTPUT);
    expander->digitalWrite(2, LOW);
    usleep(20000);
    expander->digitalWrite(2, LOW);
    usleep(120000);
    expander->digitalWrite(2, HIGH);
    /* Turn on backlight */
    expander->pinMode(1, OUTPUT);
    expander->digitalWrite(1, HIGH);
    /* Keep CS low */
    expander->pinMode(3, OUTPUT);
    expander->digitalWrite(3, LOW);
#endif

    Serial.println("Initialize panel");
    /* Initialize bus and device of panel */
    panel->init();
    /* Start panel */
    panel->begin();
#else
    Serial.println("No touch panel");
#endif
    Serial.println("Setup done");
}

void loop()
{
#if ESP_PANEL_USE_LCD_TOUCH
    panel->getLcdTouch()->readData();

    bool touched = panel->getLcdTouch()->getTouchState();
    if(touched) {
        TouchPoint point = panel->getLcdTouch()->getPoint();
        Serial.printf("Touch point: x %d, y %d\n", point.x, point.y);
    }
#endif

    usleep(30000);
}
