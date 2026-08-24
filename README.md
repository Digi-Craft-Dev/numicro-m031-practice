# NuMicro M031 Practices

This is a personal practice repository for the [NuMicro M031 MCU](https://www.nuvoton.com/products/microcontrollers/arm-cortex-m0-mcus/m031-series/). To quickly get started, you can use the [NuMaker-M031SE](https://direct.nuvoton.com/numaker-m031se) development board.

## Prerequisites

### Install Toolchain

I use [Arch Linux](https://archlinux.org/) as my host, so package installation instructions follow Arch Linux. However, most Linux distributions provide corresponding packages.

Install the toolchain:
```shell
# pacman -S arm-none-eabi-binutils arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-newlib make openocd
```

### Get M031 BSP & This Repository

From your `$HOME` directory:
```shell
git clone https://github.com/OpenNuvoton/M031BSP.git
git clone https://github.com/Digi-Craft-Dev/numicro-m031-practice.git
```

## Usage

Navigate to any subfolder in `numicro-m031-practice`. Each folder is a self-contained practice project:
* `make`: Build the project to generate build artifacts (`*.bin`, `*.elf`, and `*.hex`).
* `make flash`: Flash `*.bin` to the M031 MCU using `openocd` via the [Nu-Link2-Me](https://www.nuvoton.com/tool-and-software/debugger-and-programmer/1-to-1-debugger-and-programmer/nu-link2-me/) debugger/programmer. The [NuMaker-M031SE](https://direct.nuvoton.com/numaker-m031se) board includes an onboard Nu-Link2-Me.
* `make debug`: Start interactive debugging with `arm-none-eabi-gdb` and `openocd` via [Nu-Link2-Me](https://www.nuvoton.com/tool-and-software/debugger-and-programmer/1-to-1-debugger-and-programmer/nu-link2-me/).
* `make clean`: Remove the built artifacts.
