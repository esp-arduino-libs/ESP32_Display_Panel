| Supported ESP SoCs | ESP32 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ------------------ | ----- | -------- | -------- | -------- | -------- | -------- |

| Supported Touch Controllers | CST816S | FT5x06 | GT911 | GT1151 | ST7123 | TT21100 |
| --------------------------- | ------- | ------ | ----- | ------ | ------ | ------- |

# SPI Touch Example

The example demonstrates how to develop different model touches with SPI interface using standalone drivers and test them by printing touch point coordinates.

## How to use

1. [Configure drivers](../../../README.md#configuring-drivers) if needed.
2. Modify the macros in the example to match the parameters according to your hardware.
3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters, please refter to [Configuring Supported Development Boards](../../../README.md#configuring-supported-development-boards)
4. Verify and upload the example to your ESP board.

## Serial Output

```
...
SPI touch example start
Create SPI bus
Create touch device
SPI touch example end
Touch point(0): x 134, y 169, strength 50
Touch point(1): x 154, y 301, strength 51
Touch point(2): x 245, y 379, strength 30
Touch point(3): x 290, y 75, strength 26
Touch point(4): x 353, y 391, strength 35
...
```

## Troubleshooting

Please check the [FAQ](../../../README.md#faq) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
