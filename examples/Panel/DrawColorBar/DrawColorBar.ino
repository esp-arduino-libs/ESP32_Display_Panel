/**
 * The example demonstrates how to draw simple color bar.
 *
 * ## How to Use
 *
 * To use this example, please firstly install `ESP32_Display_Panel` (including its dependent libraries),
 * then follow the steps to configure them:
 *
 * 1. [Configure ESP32_Display_Panel](https://github.com/esp-arduino-libs/ESP32_Display_Panel#configure-esp32_display_panel)
 * 2. [Configure Board](https://github.com/esp-arduino-libs/ESP32_Display_Panel#configure-board)
 *
 * ## Example Output
 *
 * ```bash
 * ...
 * Hello Arduino!
 * I am ESP32_Display_Panel.
 * Initialize panel
 * Draw color bar from top to bottom, the order is B - G - R
 * Setup done
 * Loop
 * Loop
 * Loop
 * Loop
 * ...
 * ```
 */

#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <ESP_IOExpander_Library.h>

ESP_Panel *panel = nullptr;

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println("Hello Arduino!");
    Serial.println("I am ESP32_Display_Panel.");
    delay(2000);

    panel = new ESP_Panel();

// #if defined(ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD) || defined(ESP_PANEL_BOARD_ESP32_S3_KORVO_2)
//     /**
//      * These development boards require the use of an IO expander to configure the screen,
//      * so it needs to be initialized in advance and registered with the panel for use.
//      *
//      */
//     Serial.println("Initialize IO expander");
//     /* Initialize IO expander */
//     ESP_IOExpander *expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST,
//                                                                ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000,
//                                                                ESP_PANEL_LCD_TOUCH_I2C_IO_SCL,
//                                                                ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
//     expander->init();
//     expander->begin();
// #endif

    Serial.println("Initialize panel");
    /* Initialize bus and device of panel */
    panel->init();

    // static_cast<ESP_PanelBus_RGB *>(panel->getLcd()->getBus())->setSpiExpander(expander);

    /* Start panel */
    panel->begin();

    Serial.println("Draw color bar from top to bottom, the order is B - G - R");
    panel->getLcd()->drawColorBar(ESP_PANEL_LCD_H_RES, ESP_PANEL_LCD_V_RES);

    Serial.println("Setup done");
}

void loop()
{
    Serial.println("IDLE loop");
    delay(1000);
}
