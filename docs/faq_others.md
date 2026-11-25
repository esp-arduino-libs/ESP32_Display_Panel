# Other FAQs

## Can't find drivers for the same model of LCD/Touch device?

For **LCD**, compare the controller’s command set, initialization sequence, pixel format, and timings with the driver. The same bus type only establishes how bytes are transferred.

For **Touch**, compare the register map, report format, interrupt behavior, and reset sequence. An I²C address identifies a device on the bus. Controller compatibility depends on its protocol.

If the above methods cannot solve your problem, you can create a [Github Issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues) to request adding a driver.
