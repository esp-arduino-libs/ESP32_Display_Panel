/**
 * | Supported ESP SoCs | ESP32-S3 |
 * | ------------------ | -------- |
 *
 * | Supported LCD Controllers | ST7262 |
 * | ------------------------- | ------ |
 *
 * # Single RGB LCD Example
 *
 * The example demonstrates how to develop different model LCDs with RGB (without 3-wire SPI) interface using standalone drivers and test them by displaying color bars.
 *
 * ## How to use
 *
 * 1. [Configure drivers](https://github.com/esp-arduino-libs/ESP32_Display_Panel#configuring-drivers) if needed.
 * 2. Modify the macros in the example to match the parameters according to your hardware.
 * 3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters, please refter to [Configuring Supported Development Boards](https://github.com/esp-arduino-libs/ESP32_Display_Panel#configuring-supported-development-boards)
 * 4. Verify and upload the example to your ESP board.
 *
 * ## Serial Output
 *
 * ```
 * ...
 * RGB LCD example start
 * Create RGB LCD bus
 * Create LCD device
 * Draw color bar from top left to bottom right, the order is B - G - R
 * RGB LCD example end
 * RGB refresh rate: 0
 * RGB refresh rate: 0
 * RGB refresh rate: 31
 * RGB refresh rate: 31
 * ...
 * ```
 *
 * ## Troubleshooting
 *
 * Please check the [FAQ](https://github.com/esp-arduino-libs/ESP32_Display_Panel#faq) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
 *
 */

#include <Arduino.h>
#include <ESP_Panel_Library.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Currently, the library supports the following RGB (without 3-wire SPI) LCDs:
 *      - ST7262
 */
#define EXAMPLE_LCD_NAME                        ST7262
#define EXAMPLE_LCD_WIDTH                       (800)
#define EXAMPLE_LCD_HEIGHT                      (480)
#define EXAMPLE_LCD_COLOR_BITS                  (24)
#define EXAMPLE_LCD_RGB_DATA_WIDTH              (16)
#define EXAMPLE_LCD_RGB_TIMING_FREQ_HZ          (16 * 1000 * 1000)
#define EXAMPLE_LCD_RGB_TIMING_HPW              (40)
#define EXAMPLE_LCD_RGB_TIMING_HBP              (40)
#define EXAMPLE_LCD_RGB_TIMING_HFP              (48)
#define EXAMPLE_LCD_RGB_TIMING_VPW              (23)
#define EXAMPLE_LCD_RGB_TIMING_VBP              (32)
#define EXAMPLE_LCD_RGB_TIMING_VFP              (13)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIN_NUM_RGB_DISP            (-1)
#define EXAMPLE_LCD_PIN_NUM_RGB_VSYNC           (3)
#define EXAMPLE_LCD_PIN_NUM_RGB_HSYNC           (46)
#define EXAMPLE_LCD_PIN_NUM_RGB_DE              (17)
#define EXAMPLE_LCD_PIN_NUM_RGB_PCLK            (9)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA0           (10)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA1           (11)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA2           (12)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA3           (13)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA4           (14)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA5           (21)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA6           (47)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA7           (48)
#if EXAMPLE_LCD_RGB_DATA_WIDTH > 8
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA8           (45)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA9           (38)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA10          (39)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA11          (40)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA12          (41)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA13          (42)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA14          (2)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA15          (1)
#endif
#define EXAMPLE_LCD_PIN_NUM_RST                 (-1)
#define EXAMPLE_LCD_PIN_NUM_BK_LIGHT            (-1)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL           (1)

#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

/* Enable or disable printing RGB refresh rate */
#define EXAMPLE_ENABLE_PRINT_LCD_FPS            (1)

#define _EXAMPLE_LCD_CLASS(name, ...) ESP_PanelLcd_##name(__VA_ARGS__)
#define EXAMPLE_LCD_CLASS(name, ...)  _EXAMPLE_LCD_CLASS(name, ##__VA_ARGS__)

#if EXAMPLE_ENABLE_PRINT_LCD_FPS
#define EXAMPLE_LCD_FPS_COUNT_MAX               (100)

DRAM_ATTR int frame_count = 0;
DRAM_ATTR int fps = 0;
DRAM_ATTR long start_time = 0;

IRAM_ATTR bool onVsyncEndCallback(void *user_data)
{
    long frame_start_time = *(long *)user_data;
    if (frame_start_time == 0) {
        (*(long *)user_data) = millis();

        return false;
    }

    frame_count++;
    if (frame_count >= EXAMPLE_LCD_FPS_COUNT_MAX) {
        fps = EXAMPLE_LCD_FPS_COUNT_MAX * 1000 / (millis() - frame_start_time);
        frame_count = 0;
        (*(long *)user_data) = millis();
    }

    return false;
}
#endif

void setup()
{
    Serial.begin(115200);
    Serial.println("RGB LCD example start");

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Initialize backlight control pin and turn it off");
    ESP_PanelBacklight *backlight = new ESP_PanelBacklight(EXAMPLE_LCD_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL, true);
    backlight->begin();
    backlight->off();
#endif

    Serial.println("Create RGB LCD bus");
#if EXAMPLE_LCD_RGB_DATA_WIDTH == 8
    ESP_PanelBus_RGB *panel_bus = new ESP_PanelBus_RGB(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA0, EXAMPLE_LCD_PIN_NUM_RGB_DATA1,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA2, EXAMPLE_LCD_PIN_NUM_RGB_DATA3,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA4, EXAMPLE_LCD_PIN_NUM_RGB_DATA5,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA6, EXAMPLE_LCD_PIN_NUM_RGB_DATA7,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_HSYNC, EXAMPLE_LCD_PIN_NUM_RGB_VSYNC,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_PCLK, EXAMPLE_LCD_PIN_NUM_RGB_DE,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DISP);
#elif EXAMPLE_LCD_RGB_DATA_WIDTH == 16
    ESP_PanelBus_RGB *panel_bus = new ESP_PanelBus_RGB(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA0, EXAMPLE_LCD_PIN_NUM_RGB_DATA1,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA2, EXAMPLE_LCD_PIN_NUM_RGB_DATA3,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA4, EXAMPLE_LCD_PIN_NUM_RGB_DATA5,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA6, EXAMPLE_LCD_PIN_NUM_RGB_DATA7,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA8, EXAMPLE_LCD_PIN_NUM_RGB_DATA9,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA10, EXAMPLE_LCD_PIN_NUM_RGB_DATA11,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA12, EXAMPLE_LCD_PIN_NUM_RGB_DATA13,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DATA14, EXAMPLE_LCD_PIN_NUM_RGB_DATA15,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_HSYNC, EXAMPLE_LCD_PIN_NUM_RGB_VSYNC,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_PCLK, EXAMPLE_LCD_PIN_NUM_RGB_DE,
                                                       EXAMPLE_LCD_PIN_NUM_RGB_DISP);
#endif
    panel_bus->configRgbTimingFreqHz(EXAMPLE_LCD_RGB_TIMING_FREQ_HZ);
    panel_bus->configRgbTimingPorch(EXAMPLE_LCD_RGB_TIMING_HPW, EXAMPLE_LCD_RGB_TIMING_HBP, EXAMPLE_LCD_RGB_TIMING_HFP,
                                    EXAMPLE_LCD_RGB_TIMING_VPW, EXAMPLE_LCD_RGB_TIMING_VBP, EXAMPLE_LCD_RGB_TIMING_VFP);
    // panel_bus->configRgbBounceBufferSize(EXAMPLE_LCD_WIDTH * 10); // Set bounce buffer to avoid screen drift
    panel_bus->begin();

    Serial.println("Create LCD device");
    ESP_PanelLcd *lcd = new EXAMPLE_LCD_CLASS(EXAMPLE_LCD_NAME, panel_bus, EXAMPLE_LCD_COLOR_BITS, EXAMPLE_LCD_PIN_NUM_RST);
    lcd->init();
    lcd->reset();
    lcd->begin();
#if EXAMPLE_LCD_PIN_NUM_RGB_DISP >= 0
    lcd->displayOn();
#endif
#if EXAMPLE_ENABLE_PRINT_LCD_FPS
    lcd->attachRefreshFinishCallback(onVsyncEndCallback, (void *)&start_time);
#endif

    Serial.println("Draw color bar from top left to bottom right, the order is B - G - R");
    lcd->colorBarTest(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT);

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Turn on the backlight");
    backlight->on();
#endif

    Serial.println("RGB LCD example end");
}

void loop()
{
    delay(1000);
#if EXAMPLE_ENABLE_PRINT_LCD_FPS
    Serial.println("RGB refresh rate: " + String(fps));
#else
    Serial.println("IDLE loop");
#endif
}
