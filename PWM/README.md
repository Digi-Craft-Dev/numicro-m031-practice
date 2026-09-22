# PWM

This is a "PWM" practice project.

## Background

`main.c` is copied and modified from M031BSP's [SampleCode/Level1_Training/Level1_PWM_LED_Duty/main.c](https://github.com/OpenNuvoton/M031BSP/blob/master/SampleCode/Level1_Training/Level1_PWM_LED_Duty/main.c).

## Expected Output

* It prints PWM1 channel#1's duty cycle information via UART0 at 115200 8N1.
* M031's pin **PB.14** lights the RED LED up with the PWM duty cycle in steps, then gradually dimmed with the PWM duty cycle in steps within loop. [NuMaker-M031SE](https://www.nuvoton.com/board/numaker-m031se/?index=0)'s **PB.14** connects to the onboard LED. So, the LED lightness changes.
