#include <Arduino.h>
#include <ESP_IOExpander_Library.h>
#include <ESP_Panel_Library.h>

#define USE_IO_EXPANDER             (1)

#define TEST_LCD_H_RES              (480)
#define TEST_LCD_V_RES              (480)
#define TEST_LCD_BIT_PER_PIXEL      (18)
#define TEST_RGB_BIT_PER_PIXEL      (16)

#define TEST_LCD_PIN_NUM_RGB_DISP        (-1)
#define TEST_LCD_PIN_NUM_RGB_VSYNC       (3)
#define TEST_LCD_PIN_NUM_RGB_HSYNC       (46)
#define TEST_LCD_PIN_NUM_RGB_DE          (17)
#define TEST_LCD_PIN_NUM_RGB_PCLK        (9)
#define TEST_LCD_PIN_NUM_RGB_DATA0       (10)
#define TEST_LCD_PIN_NUM_RGB_DATA1       (11)
#define TEST_LCD_PIN_NUM_RGB_DATA2       (12)
#define TEST_LCD_PIN_NUM_RGB_DATA3       (13)
#define TEST_LCD_PIN_NUM_RGB_DATA4       (14)
#define TEST_LCD_PIN_NUM_RGB_DATA5       (21)
#define TEST_LCD_PIN_NUM_RGB_DATA6       (47)
#define TEST_LCD_PIN_NUM_RGB_DATA7       (48)
#define TEST_LCD_PIN_NUM_RGB_DATA8       (45)
#define TEST_LCD_PIN_NUM_RGB_DATA9       (38)
#define TEST_LCD_PIN_NUM_RGB_DATA10      (39)
#define TEST_LCD_PIN_NUM_RGB_DATA11      (40)
#define TEST_LCD_PIN_NUM_RGB_DATA12      (41)
#define TEST_LCD_PIN_NUM_RGB_DATA13      (42)
#define TEST_LCD_PIN_NUM_RGB_DATA14      (2)
#define TEST_LCD_PIN_NUM_RGB_DATA15      (1)
#if USE_IO_EXPANDER
#define TEST_LCD_PIN_NUM_SPI_CS          (IO_EXPANDER_PIN_NUM_1)     // Set to `IO_EXPANDER_PIN_NUM_*` to use IO expander,
                                                                // then call `enableSpixxxUseExpander()` to enable it
#else
#define TEST_LCD_PIN_NUM_SPI_CS          (0)
#endif
#define TEST_LCD_PIN_NUM_SPI_SCL         (TEST_LCD_PIN_NUM_RGB_DATA14)    // `SDA` and `SCL` can share the same pin with RGB, then
#define TEST_LCD_PIN_NUM_SPI_SDA         (TEST_LCD_PIN_NUM_RGB_DATA15)    // should call `enableAutoReleaseBus()` to release the bus
#define TEST_LCD_PIN_NUM_RST             (-1)

#define TEST_EXPANDER_I2C_HOST      ((i2c_port_t)0)
#define TEST_EXPANDER_I2C_ADDR      (ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000)
#define TEST_EXPANDER_IO_I2C_SCL    (18)
#define TEST_EXPANDER_IO_I2C_SDA    (8)

ESP_PanelLcd_ST7701 *lcd = nullptr;

/**
 * Vendor specific initialization can be different between manufacturers,
 * should consult the LCD supplier for initialization sequence code.
 *
 */
const esp_lcd_panel_vendor_init_cmd_t example_init_cmd[] = {
//  {cmd, { data }, data_size, delay_ms}
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x13}, 5, 0},
    {0xEF, (uint8_t []){0x08}, 1, 0},
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},
    {0xC1, (uint8_t []){0x10, 0x02}, 2, 0},
    {0xC2, (uint8_t []){0x20, 0x06}, 2, 0},
    {0xCC, (uint8_t []){0x10}, 1, 0},
    {0xB0, (uint8_t []){0x00, 0x13, 0x5A, 0x0F, 0x12, 0x07, 0x09, 0x08, 0x08, 0x24, 0x07, 0x13, 0x12, 0x6B, 0x73, 0xFF}, 16, 0},
    {0xB1, (uint8_t []){0x00, 0x13, 0x5A, 0x0F, 0x12, 0x07, 0x09, 0x08, 0x08, 0x24, 0x07, 0x13, 0x12, 0x6B, 0x73, 0xFF}, 16, 0},
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x11}, 5, 0},
    {0xB0, (uint8_t []){0x8D}, 1, 0},
    {0xB1, (uint8_t []){0x48}, 1, 0},
    {0xB2, (uint8_t []){0x89}, 1, 0},
    {0xB3, (uint8_t []){0x80}, 1, 0},
    {0xB5, (uint8_t []){0x49}, 1, 0},
    {0xB7, (uint8_t []){0x85}, 1, 0},
    {0xB8, (uint8_t []){0x32}, 1, 0},
    {0xC1, (uint8_t []){0x78}, 1, 0},
    {0xC2, (uint8_t []){0x78}, 1, 0},
    {0xD0, (uint8_t []){0x88}, 1, 100},
    {0xE0, (uint8_t []){0x00, 0x00, 0x02}, 3, 0},
    {0xE1, (uint8_t []){0x05, 0xC0, 0x07, 0xC0, 0x04, 0xC0, 0x06, 0xC0, 0x00, 0x44, 0x44}, 11, 0},
    {0xE2, (uint8_t []){0x00, 0x00, 0x33, 0x33, 0x01, 0xC0, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00}, 13, 0},
    {0xE3, (uint8_t []){0x00, 0x00, 0x11, 0x11}, 4, 0},
    {0xE4, (uint8_t []){0x44, 0x44}, 2, 0},
    {0xE5, (uint8_t []){0x0D, 0xF1, 0x10, 0x98, 0x0F, 0xF3, 0x10, 0x98, 0x09, 0xED, 0x10, 0x98, 0x0B, 0xEF, 0x10, 0x98}, 16, 0},
    {0xE6, (uint8_t []){0x00, 0x00, 0x11, 0x11}, 4, 0},
    {0xE7, (uint8_t []){0x44, 0x44}, 2, 0},
    {0xE8, (uint8_t []){0x0C, 0xF0, 0x10, 0x98, 0x0E, 0xF2, 0x10, 0x98, 0x08, 0xEC, 0x10, 0x98, 0x0A, 0xEE, 0x10, 0x98}, 16, 0},
    {0xEB, (uint8_t []){0x00, 0x01, 0xE4, 0xE4, 0x44, 0x88, 0x00}, 7, 0},
    {0xED, (uint8_t []){0xFF, 0x04, 0x56, 0x7F, 0xBA, 0x2F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF2, 0xAB, 0xF7, 0x65, 0x40, 0xFF}, 16, 0},
    {0xEF, (uint8_t []){0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F}, 6, 0},
    {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x00}, 5, 0},
    {0x11, (uint8_t []){0x00}, 0, 120},
    {0x29, (uint8_t []){0x00}, 0, 0},
};

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println("LCD Tset");

#if USE_IO_EXPANDER
    /* Initialize IO expander */
    Serial.println("Create IO expander");
    ESP_IOExpander *expander = new ESP_IOExpander_TCA95xx_8bit(TEST_EXPANDER_I2C_HOST,
                                                               TEST_EXPANDER_I2C_ADDR, TEST_EXPANDER_IO_I2C_SCL,
                                                               TEST_EXPANDER_IO_I2C_SDA);
    expander->init();
    expander->begin();
#endif

    Serial.println("Create LCD Bus");
    ESP_PanelBus_RGB *lcd_bus = new ESP_PanelBus_RGB(
                                        TEST_LCD_H_RES, TEST_LCD_V_RES, TEST_LCD_PIN_NUM_SPI_CS, TEST_LCD_PIN_NUM_SPI_SCL,
                                        TEST_LCD_PIN_NUM_SPI_SDA, TEST_LCD_PIN_NUM_RGB_HSYNC, TEST_LCD_PIN_NUM_RGB_VSYNC,
                                        TEST_LCD_PIN_NUM_RGB_PCLK, TEST_LCD_PIN_NUM_RGB_DATA0, TEST_LCD_PIN_NUM_RGB_DATA1,
                                        TEST_LCD_PIN_NUM_RGB_DATA2, TEST_LCD_PIN_NUM_RGB_DATA3, TEST_LCD_PIN_NUM_RGB_DATA4,
                                        TEST_LCD_PIN_NUM_RGB_DATA5, TEST_LCD_PIN_NUM_RGB_DATA6, TEST_LCD_PIN_NUM_RGB_DATA7,
                                        TEST_LCD_PIN_NUM_RGB_DATA8, TEST_LCD_PIN_NUM_RGB_DATA9, TEST_LCD_PIN_NUM_RGB_DATA10,
                                        TEST_LCD_PIN_NUM_RGB_DATA11, TEST_LCD_PIN_NUM_RGB_DATA12, TEST_LCD_PIN_NUM_RGB_DATA13,
                                        TEST_LCD_PIN_NUM_RGB_DATA14, TEST_LCD_PIN_NUM_RGB_DATA15, TEST_LCD_PIN_NUM_RGB_DE,
                                        TEST_LCD_PIN_NUM_RGB_DISP);
#if USE_IO_EXPANDER
    lcd_bus->configSpiLine(true, false, false, expander);
#endif
    lcd_bus->init();
    lcd_bus->begin();

    Serial.println("Create LCD");
    lcd = new ESP_PanelLcd_ST7701(lcd_bus, TEST_LCD_BIT_PER_PIXEL, TEST_LCD_PIN_NUM_RST, example_init_cmd,
                                  sizeof(example_init_cmd) / sizeof(esp_lcd_panel_vendor_init_cmd_t));
    lcd->enableAutoReleaseBus();    // This function is used for the case that the SPI shares pins with RGB,
                                    // and the bus will be released after calling `begin()`
    lcd->init();
    lcd->reset();
    lcd->begin();

    Serial.println("Draw color bar from top to bottom, the order is B - G - R");
    lcd->drawColorBar(TEST_LCD_H_RES, TEST_LCD_V_RES);

    Serial.println("Setup done");
}

void loop()
{
    Serial.println("Idle loop");
    delay(1000);
}
