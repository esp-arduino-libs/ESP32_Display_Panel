# Supported Touch Controllers

|                                         **Name**                                         | **Version** | **I2C** | **SPI** |
| :--------------------------------------------------------------------------------------: | :---------: | :-----: | :-----: |
|   [esp_lcd_touch](https://components.espressif.com/components/espressif/esp_lcd_touch)   |    1.1.2    |         |         |
|                                        AXS15231B                                         |      -      |    ✅    |         |
|                                         CHSC6540                                         |      -      |    ✅    |         |
|  [CST816S](https://components.espressif.com/components/espressif/esp_lcd_touch_cst816s)  |   1.0.3~1   |    ✅    |         |
|                                          CST820                                          |      -      |    ✅    |         |
| [CST9220 (CST9217)](https://components.espressif.com/components/espressif/esp_lcd_touch_cst9220) | 0.1.2 | ✅ | |
|   [FT5x06](https://components.espressif.com/components/espressif/esp_lcd_touch_ft5x06)   |   1.0.6~1   |    ✅    |         |
|                                         GSL3680                                          |    1.0.0    |    ✅    |         |
|    [GT911](https://components.espressif.com/components/espressif/esp_lcd_touch_gt911)    |   1.1.1~1   |    ✅    |         |
|   [GT1151](https://components.espressif.com/components/espressif/esp_lcd_touch_gt1151)   |   1.0.5~2   |    ✅    |         |
|  [SPD2010](https://components.espressif.com/components/espressif/esp_lcd_touch_spd2010)  |    1.0.0    |    ✅    |         |
|                                          ST1633                                          |    0.1.0    |    ✅    |         |
|   [ST7123](https://components.espressif.com/components/espressif/esp_lcd_touch_st7123)   |    1.0.0    |    ✅    |         |
| [STMPE610](https://components.espressif.com/components/espressif/esp_lcd_touch_stmpe610) |    1.0.6    |         |    ✅    |
|  [TT21100](https://components.espressif.com/components/espressif/esp_lcd_touch_tt21100)  |   1.1.0~1   |    ✅    |         |
|  [XPT2046](https://components.espressif.com/components/atanisoft/esp_lcd_touch_xpt2046)  |    1.0.5    |         |    ✅    |

The CST9220 port requires ESP-IDF 5.3 or newer. It also provides coordinates for the CST9217 in ESP-Mosaico. The Panel touch API exposes coordinates and strength.

For a custom board with GSL3680, enable `ESP_PANEL_DRIVERS_TOUCH_USE_GSL3680` and set
`ESP_PANEL_BOARD_TOUCH_CONTROLLER` to `GSL3680`. Set `ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS` to `0` to select the default
address of `0x40`.
