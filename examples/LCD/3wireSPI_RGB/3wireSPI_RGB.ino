/**
 * | Supported ESP SoCs | ESP32-S3 |
 * | ------------------ | -------- |
 *
 * | Supported LCD Controllers | GC9503 | ST7701 |
 * | ------------------------- | ------ | ------ |
 *
 * # 3-wire SPI + RGB LCD Example
 *
 * The example demonstrates how to develop different model LCDs with "3-wire SPI + RGB" interface using standalone drivers and test them by displaying color bars.
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
 * 3-wire SPI + RGB LCD example start
 * Initialize backlight and turn it off
 * Create 3-wire SPI + RGB LCD bus
 * Create LCD device
 * Draw color bar from top left to bottom right, the order is B - G - R
 * Turn on the backlight
 * 3-wire SPI + RGB LCD example end
 * RGB refresh rate: 0
 * RGB refresh rate: 60
 * RGB refresh rate: 60
 * RGB refresh rate: 60
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
 * Currently, the library supports the following "3-wire SPI + RGB" LCDs:
 *      - GC9503
 *      - ST7701
 */
#define EXAMPLE_LCD_NAME                        ST7701
#define EXAMPLE_LCD_WIDTH                       (480)
#define EXAMPLE_LCD_HEIGHT                      (480)
#define EXAMPLE_LCD_COLOR_BITS                  (18)
#define EXAMPLE_LCD_RGB_DATA_WIDTH              (16)
#define EXAMPLE_LCD_RGB_TIMING_FREQ_HZ          (16 * 1000 * 1000)
#define EXAMPLE_LCD_RGB_TIMING_HPW              (10)
#define EXAMPLE_LCD_RGB_TIMING_HBP              (10)
#define EXAMPLE_LCD_RGB_TIMING_HFP              (20)
#define EXAMPLE_LCD_RGB_TIMING_VPW              (10)
#define EXAMPLE_LCD_RGB_TIMING_VBP              (10)
#define EXAMPLE_LCD_RGB_TIMING_VFP              (10)
/**
 * LCD initialization commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code.
 *
 * Please uncomment and change the following macro definitions, then use `configVendorCommands()` to pass them in the
 * same format if needed. Otherwise, the LCD driver will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formater: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
// const esp_lcd_panel_vendor_init_cmd_t lcd_init_cmd[] = {
//     {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
//     {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},
//     {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},
//     {0x29, (uint8_t []){0x00}, 0, 120},
//     // or
//     ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}),
//     ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),
//     ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),
//     ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),
// };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIN_NUM_RGB_DISP            (-1)
#define EXAMPLE_LCD_PIN_NUM_RGB_VSYNC           (17)
#define EXAMPLE_LCD_PIN_NUM_RGB_HSYNC           (16)
#define EXAMPLE_LCD_PIN_NUM_RGB_DE              (18)
#define EXAMPLE_LCD_PIN_NUM_RGB_PCLK            (21)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA0           (4)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA1           (5)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA2           (6)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA3           (7)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA4           (15)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA5           (8)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA6           (20)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA7           (3)
#if EXAMPLE_LCD_RGB_DATA_WIDTH > 8
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA8           (46)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA9           (9)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA10          (10)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA11          (11)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA12          (12)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA13          (13)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA14          (14)
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA15          (0)
#endif
#define EXAMPLE_LCD_PIN_NUM_SPI_CS              (39)
#define EXAMPLE_LCD_PIN_NUM_SPI_SCK             (48)
#define EXAMPLE_LCD_PIN_NUM_SPI_SDA             (47)
#define EXAMPLE_LCD_PIN_NUM_RST                 (-1)
#define EXAMPLE_LCD_PIN_NUM_BK_LIGHT            (38)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL           (1)
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL          !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

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
    Serial.println("3-wire SPI + RGB LCD example start");

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Initialize backlight and turn it off");
    ESP_PanelBacklight *backlight = new ESP_PanelBacklight(EXAMPLE_LCD_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL, true);
    backlight->begin();
    backlight->off();
#endif

    Serial.println("Create 3-wire SPI + RGB LCD bus");
#if EXAMPLE_LCD_RGB_DATA_WIDTH == 8
    ESP_PanelBus_RGB *lcd_bus = new ESP_PanelBus_RGB(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT,
                                                     EXAMPLE_LCD_PIN_NUM_SPI_CS, EXAMPLE_LCD_PIN_NUM_SPI_SCK,
                                                     EXAMPLE_LCD_PIN_NUM_SPI_SDA,
                                                     EXAMPLE_LCD_PIN_NUM_RGB_DATA0, EXAMPLE_LCD_PIN_NUM_RGB_DATA1,
                                                     EXAMPLE_LCD_PIN_NUM_RGB_DATA2, EXAMPLE_LCD_PIN_NUM_RGB_DATA3,
                                                     EXAMPLE_LCD_PIN_NUM_RGB_DATA4, EXAMPLE_LCD_PIN_NUM_RGB_DATA5,
                                                     EXAMPLE_LCD_PIN_NUM_RGB_DATA6, EXAMPLE_LCD_PIN_NUM_RGB_DATA7,
                                                     EXAMPLE_LCD_PIN_NUM_RGB_HSYNC, EXAMPLE_LCD_PIN_NUM_RGB_VSYNC,
                                                     EXAMPLE_LCD_PIN_NUM_RGB_PCLK, EXAMPLE_LCD_PIN_NUM_RGB_DE,
                                                     EXAMPLE_LCD_PIN_NUM_RGB_DISP);
#elif EXAMPLE_LCD_RGB_DATA_WIDTH == 16
    ESP_PanelBus_RGB *lcd_bus = new ESP_PanelBus_RGB(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT,
                                                     EXAMPLE_LCD_PIN_NUM_SPI_CS, EXAMPLE_LCD_PIN_NUM_SPI_SCK,
                                                     EXAMPLE_LCD_PIN_NUM_SPI_SDA,
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
    lcd_bus->configRgbTimingFreqHz(EXAMPLE_LCD_RGB_TIMING_FREQ_HZ);
    lcd_bus->configRgbTimingPorch(EXAMPLE_LCD_RGB_TIMING_HPW, EXAMPLE_LCD_RGB_TIMING_HBP, EXAMPLE_LCD_RGB_TIMING_HFP,
                                  EXAMPLE_LCD_RGB_TIMING_VPW, EXAMPLE_LCD_RGB_TIMING_VBP, EXAMPLE_LCD_RGB_TIMING_VFP);
    // lcd_bus->configRgbBounceBufferSize(EXAMPLE_LCD_WIDTH * 10); // Set bounce buffer to avoid screen drift
    lcd_bus->begin();

    Serial.println("Create LCD device");
    ESP_PanelLcd *lcd = new EXAMPLE_LCD_CLASS(EXAMPLE_LCD_NAME, lcd_bus, EXAMPLE_LCD_COLOR_BITS, EXAMPLE_LCD_PIN_NUM_RST);
    // lcd->configVendorCommands(lcd_init_cmd, sizeof(lcd_init_cmd)/sizeof(lcd_init_cmd[0]));
    lcd->init();
    lcd->reset();
    lcd->begin();
    lcd->displayOn();
#if EXAMPLE_ENABLE_PRINT_LCD_FPS
    lcd->attachRefreshFinishCallback(onVsyncEndCallback, (void *)&start_time);
#endif

    Serial.println("Draw color bar from top left to bottom right, the order is B - G - R");
    lcd->colorBarTest(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT);

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Turn on the backlight");
    backlight->on();
#endif

    Serial.println("3-wire SPI + RGB LCD example end");
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
