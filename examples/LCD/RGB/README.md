| Supported ESP SoCs | ESP32-S3 |
| ------------------ | -------- |

| Supported LCD Controllers | ST7262 |
| ------------------------- | ------ |

# Single RGB LCD Example

The example demonstrates how to develop different model LCDs with RGB (without 3-wire SPI) interface using standalone drivers and test them by displaying color bars.

## How to use

1. [Configure drivers](../../../README.md#configuring-drivers) if needed.
2. Modify the macros in the example to match the parameters according to your hardware.
3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters, please refter to [Configuring Supported Development Boards](../../../README.md#configuring-supported-development-boards)
4. Verify and upload the example to your ESP board.

## Serial Output

```
...
RGB LCD example start
Create RGB LCD bus
Create LCD device
Draw color bar from top left to bottom right, the order is B - G - R
RGB LCD example end
RGB refresh rate: 0
RGB refresh rate: 0
RGB refresh rate: 31
RGB refresh rate: 31
...
```

## Troubleshooting

Please check the [FAQ](../../../README.md#faq) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
