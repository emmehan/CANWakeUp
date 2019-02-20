#
# Application for interfacing a TJA1050 CAN transceiver with a STM32F103C8T6 MCU.
# Copyright (C) 2019  Jonas Heim
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

################
# Configuration
################

#Executable file
MAIN = CANWakeUp

#Output directory
OUT_DIR = build

######################
# Build options/flags
######################

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

################
# Utility stuff
################

MKDIR_CMD = mkdir -p

#############
#Build rules
#############

all: clean create_out_directory $(MAIN)
	echo Done!

$(MAIN): startup user_files
	$(CC) $(INCLUDES) $(OUT_DIR)/main.o $(OUT_DIR)/system_stm32f10x.o $(OUT_DIR)/startup_stm32f103xb.o $(OUT_DIR)/gpio.o $(LFLAGS) -o $(OUT_DIR)/$(MAIN).elf
	$(OBJCPY) -Oihex $(OUT_DIR)/$(MAIN).elf $(OUT_DIR)/$(MAIN).hex

startup:
	$(CC) $(CFLAGS) $(INCLUDES) -c ./Drivers/CMSIS/Device/ST/STM32F1xx/Source/startup_stm32f103xb.s -o build/startup_stm32f103xb.o

user_files:
	$(CC) $(CFLAGS) $(INCLUDES) -c ./src/main.c -o $(OUT_DIR)/main.o
	$(CC) $(CFLAGS) $(INCLUDES) -c ./src/system_stm32f10x.c -o $(OUT_DIR)/system_stm32f10x.o
	$(CC) $(CFLAGS) $(INCLUDES) -c ./src/gpio.c -o $(OUT_DIR)/gpio.o
	$(CC) $(CFLAGS) $(INCLUDES) -c ./src/timer.c -o $(OUT_DIR)/timer.o

clean:
	rm -rf ./$(OUT_DIR)/*

create_out_directory: $(OUT_DIR)

$(OUT_DIR):
	$(MKDIR_CMD) $(OUT_DIR)
