# 在 micropython 中使用

这是一个如何将此库集成到 micropython 的示例。它是为 ESP32-S3 编写的，但应该很容易适应不同的受支持的 ESP 芯片。请注意，您至少需要 4MB 的闪存。

## 逐步说明

1. 安装 IDF 和 micropython

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

2. 确保您可以构建一个可工作的固件

    ```bash
    make BOARD=ESP32_GENERIC_S3 BOARD_VARIANT=SPIRAM_OCT -C ports/esp32
    pushd ports/esp32
    python -m esptool --port /dev/ttyACM0 --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 4MB --flash_freq 80m 0x0 build-ESP32_GENERIC_S3-SPIRAM_OCT/bootloader/bootloader.bin 0x8000 build-ESP32_GENERIC_S3-SPIRAM_OCT/partition_table/partition-table.bin 0x10000 build-ESP32_GENERIC_S3-SPIRAM_OCT/micropython.bin
    popd
    popd
    ```
    现在，测试开发板并确保您构建的 micropython 可以正常工作。

3. 下载 ESP32_Display_Panel 及其依赖项

    ```bash
    git clone https://github.com/esp-arduino-libs/ESP32_Display_Panel.git
    git clone https://github.com/esp-arduino-libs/esp-lib-utils.git
    git clone https://github.com/esp-arduino-libs/ESP32_IO_Expander.git
    ```

4. 创建自定义用户模块定义

    ```bash
    cat > micropython.cmake << EOF
    include(~/esp32build/ESP32_Display_Panel/micropython.cmake)
    include(~/esp32build/esp-lib-utils/micropython.cmake)
    include(~/esp32build/ESP32_IO_Expander/micropython.cmake)
    EOF
    ```

5. 复制一些头文件

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

6. 重新构建 micropython 以包含新模块

    ```bash
    pushd micropython
    make BOARD=ESP32_GENERIC_S3 BOARD_VARIANT=SPIRAM_OCT USER_C_MODULES=~/esp32build/micropython.cmake -C ports/esp32
    pushd ports/esp32
    python -m esptool --port /dev/ttyACM0 --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 4MB --flash_freq 80m 0x0 build-ESP32_GENERIC_S3-SPIRAM_OCT/bootloader/bootloader.bin 0x8000 build-ESP32_GENERIC_S3-SPIRAM_OCT/partition_table/partition-table.bin 0x10000 build-ESP32_GENERIC_S3-SPIRAM_OCT/micropython.bin
    popd
    popd
    ```

    如果您的芯片闪存大小较小，这可能会失败，在这种情况下，您必须增加应用程序分区的大小。例如，对于 4MB 闪存芯片，编辑 *micropython/ports/esp32/partitions-4MiB.csv* 并将最后两行从：

    ```csv
    factory,  app,  factory, 0x10000, 0x1F0000,
    vfs,      data, fat,     0x200000, 0x200000,
    ```

    改为：

    ```csv
    factory,  app,  factory, 0x10000, 0x2F0000,
    vfs,      data, fat,     0x300000, 0x100000,
    ```

7. 测试模块

    连接到您的开发板并运行：

    ```python
    from esp_panel import Board
    board = Board()
    board.init()
    ```

    `board.init()` 应该返回 False，因为我们还需要定义一个开发板。

8. 定义您的开发板

   编辑 *ESP32_Display_Panel/mpy_support/esp_panel_mp_board.cpp*。添加一个开发板定义：

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

    然后替换构造函数

    ```c++
    self->board = utils::make_shared<Board>()
    ```

    为

    ```c++
    self->board = utils::make_shared<Board>(BOARD_EXTERNAL_CONFIG);
    ```

9. 编辑 esp_panel_drivers_conf.h

    编辑 *ESP32_Display_Panel/esp_panel_drivers_conf.h* 并确保在您的开发板配置中引用的驱动程序被构建。
    **警告**：`ESP_PANEL_DRIVERS_BUS_USE_ALL` 似乎不起作用。设置为 0 并手动包含您需要的总线驱动程序。
    `ESP_PANEL_DRIVERS_BUS_COMPILE_UNUSED_DRIVERS` 也是如此。

10. 重复 **步骤 6** 重新构建 micropython

11. 测试您的显示屏

    连接到您的开发板并运行：

    ```python
    from esp_panel import Board
    board = Board()
    board.init()
    board.begin()
    board.color_bar_test()
    ```

12. 大功告成！:)

    要包含触摸支持，请参阅 *ESP32_Display_Panel/examples/arduino/board/board_dynamic_config/board_external_config.cpp* 获取触摸定义示例。

## 已知陷阱

1. 当 `board.init()` 返回 false 时，很可能是您在 *esp_panel_drivers_conf.h* 中的驱动程序定义不匹配。
2. 如果您依赖 `ESP_PANEL_DRIVERS_BUS_USE_ALL`，`board.begin()` 会崩溃。
3. 如果您编辑 *ESP32_Display_Panel/esp_panel_drivers_conf.h*，还需要修改 *ESP32_Display_Panel/mpy_support/esp_panel_mp_board.cpp*（比如添加或删除一个空行）。否则，对 *esp_panel_drivers_conf.h* 的更改将不会被识别。
