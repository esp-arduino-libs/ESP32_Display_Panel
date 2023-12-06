/**
 * The example demonstrates how to read touch point.
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
 * Setup done
 * Touch point: x 479, y 476
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

#if ESP_PANEL_USE_LCD_TOUCH
    panel = new ESP_Panel();

#if defined(ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD) || defined(ESP_PANEL_BOARD_ESP32_S3_KORVO_2)
    /**
     * These development boards require the use of an IO expander to configure the screen,
     * so it needs to be initialized in advance and registered with the panel for use.
     *
     */
    Serial.println("Initialize IO expander");
    /* Initialize IO expander */
    ESP_IOExpander *expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    expander->init();
    expander->begin();
    /* Add into panel */
    panel->addIOExpander(expander);
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

    bool touched = panel->getLcdTouch()->getLcdTouchState();
    if(touched) {
        TouchPoint point = panel->getLcdTouch()->getPoint();
        Serial.printf("Touch point: x %d, y %d\n", point.x, point.y);
    }
#endif

    usleep(30000);
}
