| Supported ESP SoCs | ESP32-P4 |
| ------------------ | -------- |

| Supported LCD Controllers | EK79007 | ILI9881C | JD9365 |
| ------------------------- | ------- | -------- | ------ |

# MIPI-DSI LCD Example

The example demonstrates how to develop different model LCDs with MIPI-DSI interface using standalone drivers and test them by displaying color bars.

## How to use

1. [Configure drivers](../../../docs/How_To_Use.md#configuring-drivers) if needed.
2. Modify the macros in the example to match the parameters according to your hardware.
3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters, please refter to [Configuring Supported Development Boards](../../../docs/How_To_Use.md#configuring-supported-development-boards)
4. Verify and upload the example to your ESP board.

## Serial Output

```
...
MIPI-DSI LCD example start
Initialize backlight control pin and turn it on
Create MIPI-DSI LCD bus
Create LCD device
Show MIPI-DSI patterns
Draw color bar from top left to bottom right, the order is B - G - R
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
Draw bitmap finish callback
MIPI-DSI LCD example end
MIPI-DSI refresh rate: 0
MIPI-DSI refresh rate: 69
MIPI-DSI refresh rate: 69
MIPI-DSI refresh rate: 69
...
```

## Troubleshooting

Please check the [FAQ](../../../docs/FAQ.md) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
