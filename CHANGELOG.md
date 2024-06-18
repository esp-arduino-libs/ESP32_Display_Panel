# ChangeLog

## v0.1.4 - 2024-06-17

### Enhancements:

* feat(docs): add resolution column in board instructions by @lboue (#51)

### Bugfixes:

* fix(conf): fix error when include inside file (#52)
* fix(docs): switch M5Stack doc links to english by @lboue (#50)
* fix(board): fix m5stack coreS3 power issue (#54)

## v0.1.3 - 2024-06-14

### Enhancements:

* feat(board): add add new board M5CORE2 by @MacChu0315-Espressif (#40)
* feat(board): add add new board M5DIAL by @MacChu0315-Espressif (#41)
* feat(board): add add new board M5CORES3 by @MacChu0315-Espressif (#45)
* feat(example): add support for PlatformIO by @isthaison (#37)

### Bugfixes:

* fix(version): fix version mismatch
* fix(docs): update files related to version and board M5CORE2

## v0.1.2 - 2024-06-01

### Enhancements:

* feat(config): add version control for all configuration files by @lzw655 (#32)
* feat(touch): add i2c st1633 by @lzw655 (#22)
* feat(pre-commit): support to check file versions

### Bugfixes:

* fix(docs): fix broken links in 'Panel Test Example' README by @lboue (#27)
* fix(config): fix wrong header order by @lzw655 (#35)

## v0.1.1 - 2024-05-16

### Enhancements:

* feat(touch): add spi xpt2046 by @Lzw655 (#21)
* feat(config): add new IO expander CH422G

### Bugfixes:

* fix some typo by @Franck78 (#16, #17)
* fix(docs): add more details on the version by @lboue (#23)

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
