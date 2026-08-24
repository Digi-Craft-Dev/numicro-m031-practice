PROJECT = helloworld

# -------------------------------------------------------------------------
# MCU Selection (Default: NuMaker-M031SE with M031SE3AE)
# Override via command line: make MCU=M031KG6AE or make MCU=M031FB0AE
# -------------------------------------------------------------------------
MCU ?= M031SE3AE

# -------------------------------------------------------------------------
# Parse Flash / SRAM size according to M031 Series Selection Guide
# The 5th character of the part number denotes Flash capacity:
# B = 16KB Flash / 2KB SRAM
# C = 32KB Flash / 4KB SRAM
# D = 64KB Flash / 8KB SRAM
# E = 128KB Flash / 16KB SRAM
# G = 256KB Flash / 32KB SRAM
# I = 512KB Flash / 64KB SRAM
# -------------------------------------------------------------------------
FLASH_ID = $(shell echo $(MCU) | cut -c6)

ifeq ($(FLASH_ID), B)
    FLASH_SIZE = 16K
    SRAM_SIZE  = 2K
    LD_SUFFIX  = 16k
else ifeq ($(FLASH_ID), C)
    FLASH_SIZE = 32K
    SRAM_SIZE  = 4K
    LD_SUFFIX  = 32k
else ifeq ($(FLASH_ID), D)
    FLASH_SIZE = 64K
    SRAM_SIZE  = 8K
    LD_SUFFIX  = 64k
else ifeq ($(FLASH_ID), E)
    FLASH_SIZE = 128K
    SRAM_SIZE  = 16K
    LD_SUFFIX  = 128k
else ifeq ($(FLASH_ID), G)
    FLASH_SIZE = 256K
    SRAM_SIZE  = 32K
    LD_SUFFIX  = 256k
else ifeq ($(FLASH_ID), I)
    FLASH_SIZE = 512K
    SRAM_SIZE  = 64K
    LD_SUFFIX  = 512k
else
    $(error Unknown Flash/SRAM configuration for MCU '$(MCU)' (Flash ID: $(FLASH_ID)))
endif

# -------------------------------------------------------------------------
# Toolchain & Debugger Definitions
# -------------------------------------------------------------------------
CROSS_COMPILE ?= arm-none-eabi-
CC             = $(CROSS_COMPILE)gcc
GDB            = $(CROSS_COMPILE)gdb
OBJCOPY        = $(CROSS_COMPILE)objcopy
SIZE           = $(CROSS_COMPILE)size

OPENOCD        ?= openocd
OPENOCD_CFG    ?= -f interface/cmsis-dap.cfg -c "transport select swd" -f target/numicro.cfg

BSP_DIR = ${HOME}/M031BSP
LIB_DIR = $(BSP_DIR)/Library
GCC_DIR = $(LIB_DIR)/Device/Nuvoton/M031/Source/GCC

# Prefer capacity-specific linker script if available, fallback to gcc_arm.ld
LDSCRIPT_SPECIFIC = $(GCC_DIR)/gcc_arm_$(LD_SUFFIX).ld
LDSCRIPT = $(if $(wildcard $(LDSCRIPT_SPECIFIC)),$(LDSCRIPT_SPECIFIC),$(GCC_DIR)/gcc_arm.ld)

# -------------------------------------------------------------------------
# Source Files
# -------------------------------------------------------------------------
C_SRCS += main.c
C_SRCS += $(LIB_DIR)/Device/Nuvoton/M031/Source/system_M031Series.c
C_SRCS += $(LIB_DIR)/StdDriver/src/sys.c
C_SRCS += $(LIB_DIR)/StdDriver/src/clk.c
C_SRCS += $(LIB_DIR)/StdDriver/src/uart.c
C_SRCS += $(LIB_DIR)/StdDriver/src/retarget.c

ASM_SRCS += $(GCC_DIR)/startup_M031Series.S

# -------------------------------------------------------------------------
# Compiler and Linker Flags (Arm Cortex-M0)
# -------------------------------------------------------------------------
MCU_FLAGS = -mcpu=cortex-m0 -mthumb

INCLUDES  = -I.
INCLUDES += -isystem $(LIB_DIR)/CMSIS/Core/Include
INCLUDES += -isystem $(LIB_DIR)/CMSIS/Include
INCLUDES += -isystem $(LIB_DIR)/Device/Nuvoton/M031/Include
INCLUDES += -isystem $(LIB_DIR)/StdDriver/inc

CFLAGS  = $(MCU_FLAGS) -Wall -Wextra -Wno-unused-parameter -Os -g3 -ffunction-sections -fdata-sections
CFLAGS += -D$(MCU)
CFLAGS += $(INCLUDES)

LDFLAGS  = $(MCU_FLAGS) -T$(LDSCRIPT)
LDFLAGS += -specs=nano.specs -specs=nosys.specs
LDFLAGS += -Wl,-Map=$(PROJECT).map,--gc-sections
LDFLAGS += -Wl,--defsym=__FLASH_SIZE__=$(FLASH_SIZE)
LDFLAGS += -Wl,--defsym=__SRAM_SIZE__=$(SRAM_SIZE)

# -------------------------------------------------------------------------
# Build Targets
# -------------------------------------------------------------------------
OBJS = $(C_SRCS:.c=.o) $(ASM_SRCS:.S=.o)

all: info $(PROJECT).bin $(PROJECT).hex

info:
	@echo "=========================================="
	@echo " Project       : $(PROJECT)"
	@echo " Target MCU    : $(MCU)"
	@echo " Flash Size    : $(FLASH_SIZE)"
	@echo " SRAM Size     : $(SRAM_SIZE)"
	@echo " Linker Script : $(LDSCRIPT)"
	@echo "=========================================="

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

$(PROJECT).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@

# -------------------------------------------------------------------------
# Flash & Debug Targets (OpenOCD)
# -------------------------------------------------------------------------
flash: $(PROJECT).bin
	$(OPENOCD) $(OPENOCD_CFG) -c "program $(PROJECT).bin 0x00000000 verify reset exit"

debug: $(PROJECT).elf
	$(GDB) $(PROJECT).elf \
		-ex "target extended-remote | $(OPENOCD) $(OPENOCD_CFG) -c 'gdb_port pipe'" \
		-ex "monitor reset halt" \
		-ex "load" \
		-ex "monitor reset halt"

clean:
	rm -f $(OBJS) $(PROJECT).elf $(PROJECT).bin $(PROJECT).hex $(PROJECT).map

.PHONY: all clean info flash debug
