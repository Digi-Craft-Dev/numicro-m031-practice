# GPIO

This is a "GPIO" practice project.

## Background

`main.c` is copied and modified from M031BSP's [SampleCode/Template/main.c](https://github.com/OpenNuvoton/M031BSP/blob/master/SampleCode/StdDriver/GPIO_OutputInput/main.c).

## Expected Output

* It prints CPU clock and Pin connection information via UART0 at 115200 8N1.
* M031's pin **PB.14** outputs high->low->high->low loop. [NuMaker-M031SE](https://www.nuvoton.com/board/numaker-m031se/?index=0)'s **PB.14** connects to the onboard LED. So, the LED blinks.
