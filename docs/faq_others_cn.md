# 其他 FAQ

## 找不到相同型号的 LCD/Touch 设备驱动？

对于 **LCD**，请对照控制器的命令集、初始化序列、像素格式和时序。相同的总线类型只说明字节传输方式相同。

对于 **Touch**，请对照寄存器映射、坐标报文格式、中断及复位时序。I²C 地址用于选中设备，驱动兼容性取决于控制器协议。

如果上述方法无法解决问题，您可以创建 [Github Issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues) 请求添加驱动。
