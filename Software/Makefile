
#Compiler
CC = arm-none-eabi-gcc

#ARM objcopy
OBJCPY = arm-none-eabi-objcopy

#General compiler flags
CFLAGS = -Wall -mcpu=cortex-m3 -mlittle-endian -mthumb -DSTM32F103xB -Os -g

#Include directories
INCLUDES = -IDrivers/CMSIS/Include -IDrivers/CMSIS/Device/ST/STM32F1xx/Include -Iinclude

#directory for linker file
LFILE = ld/STM32F103XB_FLASH.ld

#Linker flags
LFLAGS = -mcpu=cortex-m3 -mlittle-endian -mthumb -DSTM32F103xB -T$(LFILE) -g -Wl,--gc-sections 

#Executable file
MAIN = CANWakeUp

all: $(MAIN)
	echo Done!

$(MAIN): startup user_files
	$(CC) $(INCLUDES) build/main.o build/system_stm32f10x.o build/startup_stm32f103xb.o build/gpio.o $(LFLAGS) -o build/$(MAIN).elf
	$(OBJCPY) -Oihex ./build/$(MAIN).elf ./build/$(MAIN).hex

startup:
	$(CC) $(CFLAGS) $(INCLUDES) -c ./Drivers/CMSIS/Device/ST/STM32F1xx/Source/startup_stm32f103xb.s -o build/startup_stm32f103xb.o

user_files:
	$(CC) $(CFLAGS) $(INCLUDES) -c ./src/main.c -o build/main.o
	$(CC) $(CFLAGS) $(INCLUDES) -c ./src/system_stm32f10x.c -o build/system_stm32f10x.o
	$(CC) $(CFLAGS) $(INCLUDES) -c ./src/gpio.c -o build/gpio.o

clean:
	rm -rf ./build/*
