# Squareline Simple Wi-Fi Clock Example

This example implements a simple Wi-Fi clock demo, which UI is created by Squareline Studio.

This example can run on various LCD resolutions, but since the UI itself is designed based on a 320x240 resolution, it will look very uncoordinated if the actual resolution is too large.

## How to Use

To use this example, please firstly install the following libraries:

- lvgl (v8.3.x)
- NTPClient (v3.2.1)
- ArduinoJson (v6.21.3)

Then follow the steps below to configure the example.

1. For **ESP32_Display_Panel**:

   - [Configure drivers](../../../../README.md#configuring-drivers) if needed.
   - If using a supported development board, follow the [steps](../../../../README.md#using-supported-development-boards) to configure it.
   - If using a custom board, follow the [steps](../../../../README.md#using-custom-development-boards) to configure it.

2. Copy the [ui](./libraries/ui/) folder from `libraries` to [Arduino Library directory](../../../../README.md#where-is-the-directory-for-arduino-libraries).

3. For **lvgl**:

   - Follow the [steps](../../../../README.md#configuring-lvgl) to add *lv_conf.h* file and change the configurations. Additionally, set the following configurations to `1`:

      - `LV_FONT_MONTSERRAT_12`
      - `LV_FONT_MONTSERRAT_14`
      - `LV_FONT_MONTSERRAT_16`
      - `LV_FONT_MONTSERRAT_32`
      - `LV_FONT_MONTSERRAT_48`
      - `LV_USE_LARGE_COORD`

   - Modify the macros in the [lvgl_port_v8.h](./lvgl_port_v8.h) file to configure the LVGL porting parameters.

4. Modify the macros in the [lvgl_port_v8.h](./lvgl_port_v8.h) file to configure the LVGL porting parameters.
5. To obtain weather information after connecting to Wi-Fi, please follow these steps to configure the example:

   - Register an account on [OpenWeather](https://openweathermap.org/) and obtain an **API KEY**.
   - Fill the obtained API KEY in the macro definition `WEATHER_API_KEY`.
   - Fill the name of the city for which need to obtain weather information (such as `Shanghai`) in the macro definition `WEATHER_CITY`.

6. To obtain and calibrate time information after connecting to Wi-Fi, Please correctly fill in your time zone within the macro `TIMEZONE_OFFSET` (such as `CST-8`).
7. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters, please refter to [Configuring Supported Development Boards](../../../../README.md#configuring-supported-development-boards)
8. Verify and upload the example to your ESP board.

## Serial Output

```bash
...
Squareline WiFi clock example start
Initialize panel device
Initialize LVGL
Create UI
wifi_connected_flag: false
Squareline WiFi clock example end
Scan done
wifi_list_switch: false
Wifi list show:
wifi_list_switch: false
...
```

## Troubleshooting

Please check the [FAQ](../../../../README.md#faq) first to see if the same question exists. If not, please create a [Github issue](../../../../README.md/issues). We will get back to you as soon as possible.
