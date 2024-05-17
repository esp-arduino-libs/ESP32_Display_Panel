# Panel Test Example

The example demonstrates how to develop built-in or custom development boards using the `ESP_Panel` driver and tests by displaying color bars and printing touch point coordinates.

## How to Use

Follow the steps below to configure:

1. [Configure drivers](../../../README.md#configuring-drivers) if needed.
2. If using a supported development board, follow the [steps](../../../README.md#using-supported-development-boards) to configure it.
3. If using a custom board, follow the [steps](../../../README.md#using-custom-development-boards) to configure it.
4. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters, please refter to [Configuring Supported Development Boards](../../../README.md#configuring-supported-development-boards)
5. Verify and upload the example to your ESP board.

## Serial Output

```bash
...
Panel test example start
Initialize display panel
Turn off the backlight
Draw color bar from top to bottom, the order is B - G - R
Turn on the backlight
Panel test example end
Touch point(0): x 141, y 168, strength 47
Touch point(1): x 165, y 288, strength 45
Touch point(2): x 258, y 343, strength 33
Touch point(3): x 371, y 317, strength 24
...
```

## Troubleshooting

Please check the [FAQ](../../../README.md#faq) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
