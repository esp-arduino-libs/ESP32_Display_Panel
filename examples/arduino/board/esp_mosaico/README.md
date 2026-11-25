# ESP-Mosaico display preview

Use an ESP-Mosaico board with an ESP32-S31. The example supports hardware v1.0 and v1.2, with v1.1 using the v1.2 pin map. It reads the board revision from eFuse and stops if the revision is unknown.

1. Install Arduino-ESP32 4.0.0 and the [preview dependencies](../../../../docs/envs/use_with_arduino.md#arduino-esp32-40-preview), then select **ESP32S31 Dev Module**.
2. Open `esp_mosaico.ino` with its `esp_panel_drivers_conf.h` and match the flash and PSRAM options to the board.
3. Upload and open the serial monitor at 115200 baud. Check that the 480 × 480 CO5300 display shows color bars at 50% brightness and that touching the screen prints coordinates.

This example uses the board's QSPI display, CST9217 touch through the CST9220 driver, and shared 3.3 V supply. Keep low-speed LEDC timer 1 and channel 1 free for the cold-start ramp until `Board::begin()` returns. The display configuration uses normal QSPI transfers without the BSP's optional tearing-effect synchronization.

On Arduino, use this example to initialize the display peripherals. For applications built on the complete ESP-IDF board package, use the [ESP-Mosaico BSP](https://github.com/esp-mosaico/esp-mosaico-bsp/tree/d1eef04db4c96f027891cdb05aa6c1c5f2cbc5e8) with ESP-IDF 6.2 or newer.

The example was tested on an ESP-Mosaico board with Arduino-ESP32 4.0.0. The test confirmed color bars and touch coordinates and exercised the brightness control API.
