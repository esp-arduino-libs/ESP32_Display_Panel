# LVGL Porting Example

The example demonstrates how to port LVGL(v8). And for RGB LCD, it can enable the avoid tearing function.

## How to Use

To use this example, please firstly install the following dependent libraries:

- lvgl (>= v8.3.9, < v9)

Follow the steps below to configure:

1. For **ESP32_Display_Panel**:

    - Follow the [steps](../../../../docs/How_To_Use.md#configuring-drivers) to configure drivers if needed.
    - If using a supported development board, follow the [steps](../../../../docs/How_To_Use.md#using-supported-development-boards) to configure it.
    - If using a custom board, follow the [steps](../../../../docs/How_To_Use.md#using-custom-development-boards) to configure it.

2. For **lvgl**:

    - Follow the [steps](../../../../docs/How_To_Use.md#configuring-lvgl) to add *lv_conf.h* file and change the configurations.
    - Modify the macros in the [lvgl_port_v8.h](./lvgl_port_v8.h) file to configure the LVGL porting parameters.

3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters. For supported boards, please refter to [Configuring Supported Development Boards](../../../../docs/How_To_Use.md#configuring-supported-development-boards)
4. Verify and upload the example to your ESP board.

## Serial Output

```bash
...
LVGL porting example start
Initialize panel device
Initialize LVGL
Create UI
LVGL porting example end
IDLE loop
IDLE loop
...
```

## Troubleshooting

Please check the [FAQ](../../../../docs/FAQ.md) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
