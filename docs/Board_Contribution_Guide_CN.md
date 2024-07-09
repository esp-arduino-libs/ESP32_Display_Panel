# 开发板贡献指南

1. 新添加的开发板需要确保硬件原理图开源，需要提供链接或文件。
2. 该库目前仅支持 ESP-IDF 提供的 API，不支持其他 Arduino 库 API，如 Wire。

**注意**：
1. 推荐使用 vscode + Arduino CLI 开发环境。
2. 在进行修改之前，将 ESP32_Display_Panel 仓库拉入 Arduino 库目录中。
3. 项目使用 pre-commit 来规范提交内容，因此建议在提交之前安装 pre-commit 库，使用以下命令：

```
pip3 install pre-commit && pre-commit install
```

## 需修改内容

以适配 [`M5Stack M5DIAL`](https://github.com/esp-arduino-libs/ESP32_Display_Panel/commit/1886c668468626b9dd2ae975f7db12df5413378e) 开发板为例。按照本指南，以下更改将在项目中进行：

```
| -ESP32_Display_Panel
   | -src
      | -board
         | -m5stack                    [A]
            | -M5DIAL.h                [A]
         | -ESP_PanelBoard             [M]
         | -README.md                  [M]
      | -ESP_PanelVersions.h           [M]
   | -CHANGELOG.md                     [M]
   | -ESP_Panel_Board_Custom.h
   | -ESP_Panel_Board_Supported.h      [M]
   | -library.properties               [M]
   | -README_CN.md                     [M]
   | -README.md                        [M]
```
注：[A] 代表 '添加'，[M] 代表 '修改'

## 各文件修改流程

以适配 `M5Stack M5DIAL` 为例，按照以下步骤修改相关文件：

1. **[M]** *[ESP_Panel_Board_Supported.h](../ESP_Panel_Board_Supported.h)*：为新开发板添加一个宏，例如 `BOARD_M5STACK_M5DIAL`。命名时注意宏名大写。注意附上开发板制造商的名称和链接，以及目标开发板的链接。
2. **[A]** *[src/board/m5stack/M5DIAL.h](../src/board/m5stack/M5DIAL.h)*：使用根目录中的 *ESP_Panel_Board_Custom.h* 文件作为模板，为开发板创建一个新的配置头文件。文件命名请参考已有开发板。
3. **[M]** *[src/board/ESP_PanelBoard.h](../src/board/ESP_PanelBoard.h)*：参照文件中其他开发板的写法，添加新开发板的宏名判断，并注意修改此开发板所使用的头文件。

   **注意**：此时，可以验证上述步骤：

      - 选择一个示例，例如 *[examples/Panel/PanelTest](../examples/Panel/PanelTest/)*。
      - 修改宏 `ESP_PANEL_USE_SUPPORTED_BOARD` 以启用此头文件。定义开发板宏，例如 `BOARD_M5STACK_M5DIAL`，以启用新的头文件 *[src/board/m5stack/M5DIAL.h](../src/board/m5stack/M5DIAL.h)*。
      - 验证示例的 ino 文件。如果成功，继续后续步骤。

4. **[M]** *[src/board/m5stack/M5DIAL.h](../src/board/m5stack/M5DIAL.h)*：修改新开发板的配置头文件：
   - 审查开发板的硬件原理图，重点关注LCD 屏幕所使用 BUS 类型、 LCD 驱动名、touch 所使用 BUS 类型、touch 驱动名，以及 LCD 和 touch 各接口所使用的芯片管脚号。
   - 修改技巧是先亮屏再 touch。
   - 如果新开发板的驱动程序与现有驱动程序兼容，则无需添加新驱动程序。只需在注释中注明该驱动程序与现有驱动程序兼容，并使用现有驱动程序。
   - 如果新开发板使用的驱动程序与现有驱动程序不兼容或有其他特殊配置，可以通过修改新开发板配置头文件末尾的宏函数来实现，例如 `ESP_PANEL_BEGIN_BACKLIGHT_START_FUNCTION`、`ESP_PANEL_BEGIN_END_FUNCTION` 等。具体实现请参考 *[src/board/espressif/ESP32_S3_BOX_3.h](../src/board/espressif/ESP32_S3_BOX_3.h)* 或 *[src/board/m5stack/M5DIAL.h](../src/board/m5stack/M5DIAL.h)*。
   - 运行除 *[examples/LCD](../examples/LCD/)* 和 *[examples/Touch](../examples/Touch/)* 以外的示例，并不断调整配置头文件以确保设置正确。

5. **[M]** *[ESP_Panel_Board_Supported](../ESP_Panel_Board_Supported.h)*、*[library.properties](../library.properties)*、*[docs/Board_Instructions.md](../docs/Board_Instructions.md)*、*[README_CN.md](../README_CN.md)*、*[README.md](../README.md)*：更新上述文件中“已支持开发板”说明。
6. **[M]** *[docs/Board_Instructions.md](../docs/Board_Instructions.md)*：更新新开发板的推荐配置。
7. **[M]** *[src/ESP_PanelVersions.h](../src/ESP_PanelVersions.h)*：确保 `Library Version` 下的版本应小版本领先于最新 tag 版本。当根目录下的 *[ESP_Panel_Board_Custom.h](../ESP_Panel_Board_Custom.h)*、*[ESP_Panel_Board_Supported.h](../ESP_Panel_Board_Supported.h)* 和 *[ESP_Panel_Conf.h](../ESP_Panel_Conf.h)* 发生变化时，相应文件末尾和 *[src/ESP_PanelVersions.h](../src/ESP_PanelVersions.h)* 开头的版本号应中版本领先于最新 tag 版本。
8. **[M]** *[CHANGELOG.md](../CHANGELOG.md)*：更新变更日志。