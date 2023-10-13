# ChangeLog

## v0.1.0 - 2024-03-07

### Breaking changes:

* Restructure the driver framework based on **arduino-esp32 v3** version and is not compatible with the v2 version
* Add and modify some APIs for `bus`, `LCD` and `touch` object classes
* Temporarily remove `Kconfig` and `test_apps`, which are used for ESP-IDF. Remove action `build_test`.
* Support using independent drivers instead of the entire `ESP_Panel`. In this case, users don't need to enter the *ESP32_Display_Panel* folder and copy `ESP_Panel_Conf_Template.h`.

### Enhancements:

* Add new bus type: `QSPI`
* Add new LCD controllers: `GC9B71`, `NV3022B`, `ST7701`, `ST7789V`, `ST77916`, `ST77922`
* Add new touch controllers: `ST7123`
* Add new supported boards: `ESP32-4848S040C_I_Y_3`
* Add `LCD` and `Touch` examples for using standalone drivers
* `LCD` supports to reconfigure the vendor specific initialization sequence
* `Touch` supports to use ISR pin for interruption
* `LVLG` examples support **RGB LCD avoid tearing** function
* Update all README.md files and add FAQ section

## v0.0.2 - 2023-11-09

### Enhancements:

* Move extra boards configuration into panel
* Update all README.md files
* Add Squareline porting examples

## v0.0.1 - 2023-09-20

### Enhancements:

* Supports various Espressif official development boards
* Supports custom boards
* Supports multiple types of drivers, including **Bus**, **LCD**, **Touch**, **Backlight**
