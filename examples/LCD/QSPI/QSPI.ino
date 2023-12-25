#include <Arduino.h>
#include <ESP_IOExpander_Library.h>
#include <ESP_Panel_Library.h>

/**
 * Currently, the library supports the following QSPI LCDs:
 *      - GC9B71
 *      - SPD2010
 *      - SH8601
 *      - ST77916
 *
 */
#define TEST_LCD_NAME               ST77916
#define TEST_LCD_H_RES              (360)
#define TEST_LCD_V_RES              (360)
#define TEST_LCD_BIT_PER_PIXEL      (16)

#define TEST_PIN_NUM_LCD_CS         (GPIO_NUM_39)
#define TEST_PIN_NUM_LCD_PCLK       (GPIO_NUM_38)
#define TEST_PIN_NUM_LCD_DATA0      (GPIO_NUM_40)
#define TEST_PIN_NUM_LCD_DATA1      (GPIO_NUM_41)
#define TEST_PIN_NUM_LCD_DATA2      (GPIO_NUM_42)
#define TEST_PIN_NUM_LCD_DATA3      (GPIO_NUM_2)
#define TEST_PIN_NUM_LCD_RST        (GPIO_NUM_1)

#define _TEST_LCD_CLASS(name, ...)  ESP_PanelLcd_##name(__VA_ARGS__)
#define TEST_LCD_CLASS(name, ...)   _TEST_LCD_CLASS(name, ##__VA_ARGS__)

ESP_PanelLcd *lcd = nullptr;

/**
 * Vendor specific initialization can be different between manufacturers,
 * should consult the LCD supplier for initialization sequence code.
 *
 */
// const esp_lcd_panel_vendor_init_cmd_t example_init_cmd[] = {
// //  {cmd, { data }, data_size, delay_ms}
//     {0xF0, (uint8_t[]){0x08}, 1, 0},
//     {0xF2, (uint8_t[]){0x08}, 1, 0},
//     {0x9B, (uint8_t[]){0x51}, 1, 0},
//     {0x86, (uint8_t[]){0x53}, 1, 0},
//     {0xF2, (uint8_t[]){0x80}, 1, 0},
//     {0xF0, (uint8_t[]){0x00}, 1, 0},
//     {0xF0, (uint8_t[]){0x28}, 1, 0},
//     {0xF2, (uint8_t[]){0x28}, 1, 0},
//     {0x83, (uint8_t[]){0xE0}, 1, 0},
//     {0x84, (uint8_t[]){0x61}, 1, 0},
//     {0xF2, (uint8_t[]){0x82}, 1, 0},
//     {0xF0, (uint8_t[]){0x00}, 1, 0},
//     ...
// };

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println("LCD Tset");

    Serial.println("Create LCD Bus");
    ESP_PanelBus *lcd_bus = new ESP_PanelBus_QSPI(TEST_PIN_NUM_LCD_CS, TEST_PIN_NUM_LCD_PCLK,
                                                  TEST_PIN_NUM_LCD_DATA0, TEST_PIN_NUM_LCD_DATA1,
                                                  TEST_PIN_NUM_LCD_DATA2, TEST_PIN_NUM_LCD_DATA3);
    lcd_bus->init();
    lcd_bus->begin();

    Serial.println("Create LCD");
    lcd = new TEST_LCD_CLASS(TEST_LCD_NAME, lcd_bus, TEST_LCD_BIT_PER_PIXEL, TEST_PIN_NUM_LCD_RST);
    /* A more common way, need to uncomment `example_init_cmd` array */
    // lcd = new TEST_LCD_CLASS(TEST_LCD_NAME, lcd_bus, TEST_LCD_BIT_PER_PIXEL, TEST_PIN_NUM_LCD_RST, example_init_cmd,
    //                          sizeof(example_init_cmd) / sizeof(esp_lcd_panel_vendor_init_cmd_t));
    lcd->init();
    lcd->reset();
    lcd->begin();
    lcd->displayOn();

    Serial.println("Draw color bar from top to bottom, the order is B - G - R");
    lcd->drawColorBar(TEST_LCD_H_RES, TEST_LCD_V_RES);

    Serial.println("Setup done");
}

void loop()
{
    Serial.println("Idle loop");
    delay(1000);
}
