# SquareLine Porting Example

The example demonstrates how to port SquareLine (v1.3.x) project. And for RGB LCD, it can enable the avoid tearing function.

## How to Use

To use this example, please firstly install the following dependent libraries:

- lvgl (>= v8.3.9, < v9)

Then follow the steps below to configure:

1. For **ESP32_Display_Panel**:

   - [Configure drivers](../../../../README.md#configuring-drivers) if needed.
   - If using a supported development board, follow the [steps](../../../../README.md#using-supported-development-boards) to configure it.
   - If using a custom board, follow the [steps](../../../../README.md#using-custom-development-boards) to configure it.

2. For **lvgl**:

    - Follow the [steps](../../README.md#configuring-lvgl) to add *lv_conf.h* file and change the configurations.
    - Modify the macros in the [lvgl_port_v8.h](./lvgl_port_v8.h) file to configure the LVGL porting parameters.

3. To directly use the example, please copy the [ui](./libraries/ui/) folder from `libraries` to [Arduino Library directory](../../../../README.md#where-is-the-directory-for-arduino-libraries). What's more, you can follow the [steps](../../../../README.md#porting-squareline-project) to port your own **SquareLine** project.
4. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters. For supported boards, please refter to [Configuring Supported Development Boards](../../README.md#configuring-supported-development-boards)
5. Verify and upload the example to your ESP board.

## Serial Output

```bash
...
Squareline porting example start
Initialize panel device
Initialize LVGL
Create UI
Squareline porting example end
IDLE loop
IDLE loop
IDLE loop
...
```

## Troubleshooting

Please check the [FAQ](../../../../README.md#faq) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
