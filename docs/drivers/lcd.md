# Supported LCD Controllers

|                                          **Name**                                          | **Version** | **SPI** | **QSPI** | **Single RGB** | **3-wire SPI + RGB** | **MIPI-DSI** |
| :----------------------------------------------------------------------------------------: | :---------: | :-----: | :------: | :------------: | :------------------: | :----------: |
|   [AXS15231B](https://components.espressif.com/components/espressif/esp_lcd_axs15231b)     |    1.0.0    |    ✅   |    ✅    |                |                      |              |
| [CO5300](https://components.espressif.com/components/espressif/esp_lcd_co5300) | 2.2.0 (SPI/QSPI port) | ✅ | ✅ | | | |
|                                          EK9716B                                           |      -      |         |          |       ✅       |                      |              |
|      [EK79007](https://components.espressif.com/components/espressif/esp_lcd_ek79007)      |    1.0.1    |         |          |                |                      |      ✅      |
| [GC9107](https://components.espressif.com/components/espressif/esp_lcd_gc9107) | 2.0.0 | ✅ | | | | |
|       [GC9A01](https://components.espressif.com/components/espressif/esp_lcd_gc9a01)       |    2.0.0    |    ✅   |          |                |                      |              |
|       [GC9B71](https://components.espressif.com/components/espressif/esp_lcd_gc9b71)       |    1.0.2    |    ✅   |    ✅    |                |                      |              |
| [GC9D01](https://components.espressif.com/components/espressif/esp_lcd_gc9d01) | 2.0.0 | ✅ | | | | |
|       [GC9503](https://components.espressif.com/components/espressif/esp_lcd_gc9503)       |    3.0.1    |         |          |                |          ✅          |              |
|       [HX8399](https://components.espressif.com/components/espressif/esp_lcd_hx8399)       |    1.0.1    |         |          |                |                      |      ✅      |
|      [ILI9341](https://components.espressif.com/components/espressif/esp_lcd_ili9341)      |    2.0.0    |    ✅   |          |                |                      |              |
|     [ILI9881C](https://components.espressif.com/components/espressif/esp_lcd_ili9881c)     |    1.0.1    |         |          |                |                      |      ✅      |
|       [JD9165](https://components.espressif.com/components/espressif/esp_lcd_jd9165)       |    1.0.1    |         |          |                |                      |      ✅      |
|       [JD9365](https://components.espressif.com/components/espressif/esp_lcd_jd9365)       |    1.0.1    |         |          |                |                      |      ✅      |
|      [NV3022B](https://components.espressif.com/components/espressif/esp_lcd_nv3022b)      |    1.0.0    |    ✅   |          |                |                      |              |
|       [SH8601](https://components.espressif.com/components/espressif/esp_lcd_sh8601)       |    1.0.0    |    ✅   |    ✅    |                |                      |              |
|      [SPD2010](https://components.espressif.com/components/espressif/esp_lcd_spd2010)      |    1.0.2    |    ✅   |    ✅    |                |                      |              |
|                                           ST7262                                           |      -      |         |          |       ✅       |                      |              |
|       [ST7701](https://components.espressif.com/components/espressif/esp_lcd_st7701)       |    1.1.1    |         |          |                |          ✅          |      ✅      |
|       [ST7703](https://components.espressif.com/components/espressif/esp_lcd_st7703)       |    1.0.1    |         |          |                |                      |      ✅      |
|                                           ST7789                                           |      -      |    ✅   |          |                |                      |              |
|       [ST7796](https://components.espressif.com/components/espressif/esp_lcd_st7796)       |    1.2.1    |    ✅   |          |                |                      |      ✅      |
| [ST77903 (RGB)](https://components.espressif.com/components/espressif/esp_lcd_st77903_rgb) |    1.0.0    |         |          |                |          ✅          |              |
|      [ST77916](https://components.espressif.com/components/espressif/esp_lcd_st77916)      |    1.0.0    |    ✅   |    ✅    |                |                      |              |
|      [ST77922](https://components.espressif.com/components/espressif/esp_lcd_st77922)      |    1.0.2    |    ✅   |    ✅    |                |          ✅          |      ✅      |

CO5300 requires ESP-IDF 5.4 or newer. GC9107 and GC9D01 require ESP-IDF 5.3 or newer. These ports have been compile-tested.

Use `LCD::attachDrawBitmapFinishCallback()` to receive the SPI or QSPI pixel transfer completion event. RGB and
MIPI-DSI also provide a framebuffer refresh event through `LCD::attachRefreshFinishCallback()`.

>[!TIP]
> For more detailed information about LCD displays, please refer to [ESP-IoT-Solution Development Guide - LCD Introduction](https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/lcd_guide.html).
