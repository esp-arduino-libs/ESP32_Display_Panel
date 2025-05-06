# Using with micropython

This is an example of how to integrate this library into micropython. It's written for an ESP32-S3, however it should be easy to adapt to different supported ESP-Chips. Please note, that you'll need at least 4mb of flash.

## Step-by-Step instructions

1. Install IDF and micropython

    ```bash
    mkdir ~/esp32build
    pushd ~/esp32build
    git clone -b v5.2.2 --recursive https://github.com/espressif/esp-idf.git
    pushd esp-idf
    ./install.sh esp32
    source export.sh
    popd
    git clone https://github.com/micropython/micropython.git
    pushd micropython
    git submodule update --init --recursive
    pushd mpy-cross
    make
    popd
    ```

2. Ensure, you can build a working firmware

    ```bash
    make BOARD=ESP32_GENERIC_S3 BOARD_VARIANT=SPIRAM_OCT -C ports/esp32
    pushd ports/esp32
    python -m esptool --port /dev/ttyACM0 --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 4MB --flash_freq 80m 0x0 build-ESP32_GENERIC_S3-SPIRAM_OCT/bootloader/bootloader.bin 0x8000 build-ESP32_GENERIC_S3-SPIRAM_OCT/partition_table/partition-table.bin 0x10000 build-ESP32_GENERIC_S3-SPIRAM_OCT/micropython.bin
    popd
    popd
    ```
    Now, test the board and ensure your build of micropython works.

3. Download ESP32_Display_Panel and it's dependencies

    ```bash
    git clone https://github.com/esp-arduino-libs/ESP32_Display_Panel.git
    git clone https://github.com/esp-arduino-libs/esp-lib-utils.git
    git clone https://github.com/esp-arduino-libs/ESP32_IO_Expander.git
    ```

4. Create a custom user-module definition

    ```bash
    cat > micropython.cmake << EOF
    include(~/esp32build/ESP32_Display_Panel/micropython.cmake)
    include(~/esp32build/esp-lib-utils/micropython.cmake)
    include(~/esp32build/ESP32_IO_Expander/micropython.cmake)
    EOF
    ```

5. Copy some header-files

    ```bash
    cp esp-idf/components/esp_lcd/include/esp_lcd_panel_commands.h ESP32_Display_Panel/mpy_support/
    cp esp-idf/components/esp_lcd/interface/esp_lcd_panel_interface.h ESP32_Display_Panel/mpy_support/
    cp esp-idf/components/esp_lcd/include/esp_lcd_panel_io.h ESP32_Display_Panel/mpy_support/
    cp esp-idf/components/esp_lcd/interface/esp_lcd_panel_io_interface.h ESP32_Display_Panel/mpy_support/
    cp esp-idf/components/esp_lcd/include/esp_lcd_panel_ops.h ESP32_Display_Panel/mpy_support/
    cp esp-idf/components/esp_lcd/include/esp_lcd_panel_rgb.h ESP32_Display_Panel/mpy_support/
    cp esp-idf/components/esp_lcd/include/esp_lcd_panel_vendor.h ESP32_Display_Panel/mpy_support/
    cp esp-idf/components/esp_lcd/include/esp_lcd_types.h ESP32_Display_Panel/mpy_support/
    ```

6. Rebuild micropython to include the new modules

    ```bash
    pushd micropython
    make BOARD=ESP32_GENERIC_S3 BOARD_VARIANT=SPIRAM_OCT USER_C_MODULES=~/esp32build/micropython.cmake -C ports/esp32
    pushd ports/esp32
    python -m esptool --port /dev/ttyACM0 --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 4MB --flash_freq 80m 0x0 build-ESP32_GENERIC_S3-SPIRAM_OCT/bootloader/bootloader.bin 0x8000 build-ESP32_GENERIC_S3-SPIRAM_OCT/partition_table/partition-table.bin 0x10000 build-ESP32_GENERIC_S3-SPIRAM_OCT/micropython.bin
    popd
    popd
    ```

    This may fail if your chip has a small flash size, in this case you have to increase the size of the application partition. E.g. for a 4mb flash chip edit *micropython/ports/esp32/partitions-4MiB.csv* and change the last two lines from:

    ```csv
    factory,  app,  factory, 0x10000, 0x1F0000,
    vfs,      data, fat,     0x200000, 0x200000,
    ```

    to

    ```csv
    factory,  app,  factory, 0x10000, 0x2F0000,
    vfs,      data, fat,     0x300000, 0x100000,
    ```

7. Test the module

    Connect to your board and run:

    ```python
    from esp_panel import Board
    board = Board()
    board.init()
    ```

    `board.init()` should return False, as we yet have to define a board.

8. Define your Board

   Edit *ESP32_Display_Panel/mpy_support/esp_panel_mp_board.cpp*. Add a Board definition:

    ```c++
    const BoardConfig BOARD_EXTERNAL_CONFIG = {
        /* General */
        .name = "ESP_PANEL_BOARD_NAME",
        .lcd = BoardConfig::LCD_Config{
            .bus_config = esp_panel::drivers::BusSPI::Config{
                .host_id = 1,
                // Host
                .host = esp_panel::drivers::BusSPI::HostPartialConfig{
                    .mosi_io_num = 6, //ESP_PANEL_BOARD_LCD_SPI_IO_MOSI,
                    .miso_io_num = 8, //ESP_PANEL_BOARD_LCD_SPI_IO_MISO,
                    .sclk_io_num = 7, //ESP_PANEL_BOARD_LCD_SPI_IO_SCK,
                },
                // Control Panel
                .control_panel = esp_panel::drivers::BusSPI::ControlPanelPartialConfig{
                    .cs_gpio_num = 5, //ESP_PANEL_BOARD_LCD_SPI_IO_CS,
                    .dc_gpio_num = 4, //ESP_PANEL_BOARD_LCD_SPI_IO_DC,
                    .spi_mode = 0, //ESP_PANEL_BOARD_LCD_SPI_MODE,
                    .pclk_hz = 40 * 1000 * 1000, //ESP_PANEL_BOARD_LCD_SPI_CLK_HZ,
                    .lcd_cmd_bits = 8, //ESP_PANEL_BOARD_LCD_SPI_CMD_BITS,
                    .lcd_param_bits = 8, //ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS,
                },
            },
            .device_name = "ILI9341",
            .device_config = {
                // Device
                .device = esp_panel::drivers::LCD::DevicePartialConfig{
                    .reset_gpio_num = 48, //ESP_PANEL_BOARD_LCD_RST_IO,
                    .rgb_ele_order = 0, //ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER,
                    .bits_per_pixel = 18, //ESP_PANEL_BOARD_LCD_COLOR_BITS, 16/18/24
                    .flags_reset_active_high = 0, //ESP_PANEL_BOARD_LCD_RST_LEVEL,
                },
                // Vendor
                .vendor = esp_panel::drivers::LCD::VendorPartialConfig{
                    .hor_res = 320, //ESP_PANEL_BOARD_WIDTH,
                    .ver_res = 480, //ESP_PANEL_BOARD_HEIGHT,
                },
            },
            .pre_process = {
                .invert_color = 0, //ESP_PANEL_BOARD_LCD_COLOR_INEVRT_BIT,
            },
        },
    };
    ```

    Then replace the constructor

    ```c++
    self->board = utils::make_shared<Board>()
    ```

    with

    ```c++
    self->board = utils::make_shared<Board>(BOARD_EXTERNAL_CONFIG);
    ```

9. Edit esp_panel_drivers_conf.h

    Edit *ESP32_Display_Panel/esp_panel_drivers_conf.h* and ensure, the drivers referenced in your board config are being
    build. **Warning**: `ESP_PANEL_DRIVERS_BUS_USE_ALL` does not seem to work. Set to 0 and manually include the bus driver
    you need. Same goes for `ESP_PANEL_DRIVERS_BUS_COMPILE_UNUSED_DRIVERS`.

10. Repeat **Step 6** to rebuild micropython

11. Test your display

    Connect to your board and run:

    ```python
    from esp_panel import Board
    board = Board()
    board.init()
    board.begin()
    board.color_bar_test()
    ```

12. Profit! :)

    To include touch support, see *ESP32_Display_Panel/examples/arduino/board/board_dynamic_config/board_external_config.cpp* for an example touch definition.

## Known Pitfalls

1. When `board.init()` returns false, likely your driver-definition in *esp_panel_drivers_conf.h* does not match.
2. `board.begin()` crashes, if you rely on `ESP_PANEL_DRIVERS_BUS_USE_ALL`
3. If you edit *ESP32_Display_Panel/esp_panel_drivers_conf.h*, also modify *ESP32_Display_Panel/mpy_support/esp_panel_mp_board.cpp* (like add or remove an empty line). Otherwise, changes to *esp_panel_drivers_conf.h* will not be recognized.
