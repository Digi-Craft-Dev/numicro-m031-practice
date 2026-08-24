# Hello World

This is a "Hello World" practice project.

## Background

`main.c` is copied and modified from M031BSP's [SampleCode/Template/main.c](https://github.com/OpenNuvoton/M031BSP/blob/master/SampleCode/Template/main.c).

## Expected Output

It prints "Hello World" via UART0 at 115200 8N1.

[Nu-Link2-Me's VCOM](https://www.nuvoton.com/tool-and-software/debugger-and-programmer/1-to-1-debugger-and-programmer/nu-link2-me/?_t=20718) of NuMaker-M031SE will be enumerated as `/dev/ttyACM<n>`. Here is the configuration and usage example:
```shell
sudo timeout 3 stty -F /dev/ttyACM0 115200 raw -echo
cat /dev/ttyACM0
```
