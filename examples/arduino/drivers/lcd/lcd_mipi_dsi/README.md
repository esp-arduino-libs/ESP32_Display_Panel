| Supported ESP SoCs | ESP32-P4 |
| ------------------ | -------- |

| Supported LCD Controllers | EK79007 | HX8399 | ILI9881C | JD9365 | JD9165 | ST7701 | ST7703 | ST7796 | ST77922 |
| ------------------------- | ------- | ------ | -------- | ------ | ------ | ------ | ------ | ------ | ------- |

# MIPI-DSI LCD Example

## Overview

This example demonstrates how to drive different model LCDs with `MIPI-DSI` interface bus and test them by displaying color bars.

On Arduino-ESP32 4.0, the default DSI path copies draw buffers on the CPU and can call the draw completion callback in task context. Framebuffer refresh continues in hardware.

## How to Use

### Step 1. Install SDK and dependencies

- Install the following SDK and library dependencies:

  - See [SDK & Dependencies](../../../../../docs/envs/use_with_arduino.md#sdk--dependencies) and [Installing Libraries](../../../../../docs/envs/use_with_arduino.md#installing-libraries) sections for more information

### Step 2. Configure the libraries

- [Optional] `ESP32_Display_Panel`:

  - This example already has the [esp_panel_drivers_conf.h](./esp_panel_drivers_conf.h) configuration file in the project directory. Edit this file as needed
  - See [Configuring Guide](../../../../../docs/envs/use_with_arduino.md#configuration-guide) section for more information

- [Optional] `esp-lib-utils` :

  - This example already has the [esp_utils_conf.h](./esp_utils_conf.h) configuration file in the project directory. Edit this file as needed
  - See [Configuring esp-lib-utils](../../../../../docs/envs/use_with_arduino.md#configuring-esp-lib-utils) section for more information

### Step 3. Configure the example

- [Mandatory] Edit the macro definitions in the [lcd_mipi_dsi.ino](./lcd_mipi_dsi.ino) file

### Step 4. Configure Arduino IDE

- Navigate to the `Tools` menu
- Select the target board in the `Board` menu
- Set `PSRAM` to `Enabled` so the ESP32-P4 can allocate the DSI framebuffer
- Change the `USB CDC On Boot` option to `Enabled` if using `USB` port, or `Disabled` if using `UART` port. Reselect the port after uploading if its name changes.
- Change other configurations as needed
- see [Configuring Arduino IDE](../../../../../docs/envs/use_with_arduino.md#configuring-arduino-ide) for more information

### Step 5. Compile and upload the project

- Connect the board to your computer
- Select the correct serial port
- Click the `upload` button

### Step 6. Check the serial output

- Open the serial monitor and select the correct baud rate (like `115200`)
- Check the output logs

## Serial Output

The following are the logs output when using the `EK79007` LCD. The logs content may vary with different LCDs or different configurations, and it is provided for reference only.

```bash
...
nitializing backlight and turn it on
Initializing "MIPI-DSI" LCD without config
[I][Panel][esp_lcd_ek79007.c:0065](esp_lcd_new_panel_ek79007): version: 1.0.1
Show MIPI-DSI color bar patterns
Draw color bar from top left to bottom right, the order is B - G - R
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
LCD draw finish callback
IDLE loop
LCD FPS: 13
IDLE loop
LCD FPS: 69
IDLE loop
LCD FPS: 69
...
```

## Troubleshooting

Please check the [FAQ](../../../../../docs/envs/use_with_arduino.md#faq) first to see if the same question exists. If not, please create a [Github Issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
