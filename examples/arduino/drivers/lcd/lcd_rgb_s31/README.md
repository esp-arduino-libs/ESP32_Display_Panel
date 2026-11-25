# ESP32-S31 RGB888 color bars

Use Arduino-ESP32 4.0.0 and select **ESP32S31 Dev Module**. Install the [preview dependencies](../../../../../docs/envs/use_with_arduino.md#arduino-esp32-40-preview).

The example drives an 800 × 480 ST7262 panel at 20 MHz over 24 RGB data lines. Check the GPIO mapping and timings against your board before uploading. The panel and backlight use the board's existing power connections. The RGB888 framebuffer requires 1,152,000 bytes of PSRAM.

The sketch draws color bars and reports the panel size and framebuffer color depth over UART at 115200 baud. The example has been compile-tested with Arduino-ESP32 4.0.0.
