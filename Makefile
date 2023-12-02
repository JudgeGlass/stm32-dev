CFLAGS  ?=  -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections \
            -I. -Iinclude -Icmsis_core/CMSIS/Core/Include -Icmsis_f4/Include \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlinker/link.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = $(wildcard src/*.c) $(wildcard src/hal/*.c) $(wildcard src/kernel/*.c)
SOURCES += cmsis_f4/Source/Templates/gcc/startup_stm32f429xx.s # ST startup file. Compiler-dependent!

CC = arm-none-eabi-gcc
OBJ = arm-none-eabi-objcopy

build: firmware.bin

firmware.elf: cmsis_core cmsis_f4 Makefile $(SOURCES) 
	if [ ! -d "./build" ]; then \
		mkdir build/; \
	fi

	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o build/$@

	rm firmware.elf.map

firmware.bin: firmware.elf
	$(OBJ) -O binary build/$< build/$@

flash: firmware.bin
	st-flash --reset write build/$< 0x8000000

cmsis_core:
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_f4:
	git clone --depth 1 -b v2.6.8 https://github.com/STMicroelectronics/cmsis_device_f4 $@

clean:
	rm -rf firmware.* cmsis_* build/