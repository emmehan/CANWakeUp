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

#################
# Configuration	#
#################

#Executable file
MAIN = CANWakeUp

#Output directory
OUT_DIR = build

# Documentation directory
DOC_DIR = doc

HAL_SRC_DIR				= ./Drivers/STM32F1xx_HAL_Driver/Src
HAL_INC_DIR				= ./Drivers/STM32F1xx_HAL_Driver/Inc

CMSIS_SRC_DIR 			= ./Drivers/CMSIS/Device/ST/STM32F1xx/Source
CMSIS_DEVICE_INC_DIR	= ./Drivers/CMSIS/Device/ST/STM32F1xx/Include 
CMSIS_INC_DIR			= ./Drivers/CMSIS/Include

BSP_SRC_DIR				= ./Drivers/BSP/CANWakeUp/src
BSP_INC_DIR				= ./Drivers/BSP/CANWakeUp/inc

FREERTOS_SRC_DIR		= ./Middleware/FreeRTOS/Source
FREERTOS_INC_DIR		= ./Middleware/FreeRTOS/Source/include
CMSIS_RTOS_SRC_DIR		= ./Middleware/FreeRTOS/Source/CMSIS_RTOS
CMSIS_RTOS_INC_DIR		= ./Middleware/FreeRTOS/Source/CMSIS_RTOS
PORTABLE_COMMON_RTOS_SRC_DIR	= ./Middleware/FreeRTOS/Source/portable/Common
PORTABLE_GCC_RTOS_SRC_DIR	= ./Middleware/FreeRTOS/Source/portable/GCC/ARM_CM3
PORTABLE_GCC_RTOS_INC_DIR	= ./Middleware/FreeRTOS/Source/portable/GCC/ARM_CM3

TRACE_SRC_DIR			= ./Trace/src
TRACE_INC_DIR			= ./Trace/inc

APP_SRC_DIR				= ./src
APP_INC_DIR				= ./inc


#######################
# Build options/flags #
#######################

#Compiler
CC = arm-none-eabi-gcc

#ARM objcopy
OBJCPY = arm-none-eabi-objcopy

#General compiler flags
CFLAGS = -Wall -mcpu=cortex-m3 -mlittle-endian -mthumb -DSTM32F103xB -g

#Include directories
INCLUDES = -I$(HAL_INC_DIR) -I$(CMSIS_DEVICE_INC_DIR) -I$(CMSIS_INC_DIR) -I$(BSP_INC_DIR) -I$(FREERTOS_INC_DIR) -I$(CMSIS_RTOS_INC_DIR) -I$(PORTABLE_GCC_RTOS_INC_DIR) -I$(TRACE_INC_DIR) -I$(APP_INC_DIR)

#directory for linker file
LFILE = ld/STM32F103XB_FLASH.ld

#Linker flags
LFLAGS = -mcpu=cortex-m3 -mlittle-endian -mthumb -DSTM32F103xB -T$(LFILE) -g -Wl,--gc-sections

#################
# Build rules   #
# Utility stuff #
#################
MKDIR_CMD = mkdir -p
CREATE_DOC = doxygen ./$(DOC_DIR)/Doxyfile

clean:
	rm -rf ./$(OUT_DIR)
	rm -rf ./$(DOC_DIR)/html

create_out_directory: $(OUT_DIR)

.PHONY: doc

doc:
	$(CREATE_DOC)

$(OUT_DIR):
	$(MKDIR_CMD) $(OUT_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(HAL_SRC_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(CMSIS_SRC_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(BSP_SRC_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(FREERTOS_SRC_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(CMSIS_RTOS_SRC_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(PORTABLE_COMMON_RTOS_SRC_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(PORTABLE_GCC_RTOS_SRC_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(TRACE_SRC_DIR)
	$(MKDIR_CMD) $(OUT_DIR)/$(APP_SRC_DIR)

#################
# Build rules	#
# STM32F1xx HAL	#
#################
HAL_SOURCES := $(wildcard $(HAL_SRC_DIR)/*.c)
HAL_OBJECTS := $(HAL_SOURCES:$(HAL_SRC_DIR)/%.c=$(OUT_DIR)/$(HAL_SRC_DIR)/%.o)

$(HAL_OBJECTS):	$(OUT_DIR)/$(HAL_SRC_DIR)/%.o : $(HAL_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#################
# Build rules	#
# Startup/CMSIS	#
#################
STARTUP_SOURCES := $(wildcard $(CMSIS_SRC_DIR)/*.s)
STARTUP_OBJECTS := $(STARTUP_SOURCES:$(CMSIS_SRC_DIR)/%.s=$(OUT_DIR)/$(CMSIS_SRC_DIR)/%.o)

$(STARTUP_OBJECTS): $(OUT_DIR)/$(CMSIS_SRC_DIR)/%.o : $(CMSIS_SRC_DIR)/%.s
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#################
# Build rules	#
# BSP           #
#################
BSP_SOURCES := $(wildcard $(BSP_SRC_DIR)/*.c)
BSP_OBJECTS := $(BSP_SOURCES:$(BSP_SRC_DIR)/%.c=$(OUT_DIR)/$(BSP_SRC_DIR)/%.o)

$(BSP_OBJECTS): $(OUT_DIR)/$(BSP_SRC_DIR)/%.o : $(BSP_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#################
# Build rules	#
# FreeRTOS      #
#################
FREERTOS_SOURCES := $(wildcard $(FREERTOS_SRC_DIR)/*.c)
FREERTOS_OBJECTS := $(FREERTOS_SOURCES:$(FREERTOS_SRC_DIR)/%.c=$(OUT_DIR)/$(FREERTOS_SRC_DIR)/%.o)

$(FREERTOS_OBJECTS): $(OUT_DIR)/$(FREERTOS_SRC_DIR)/%.o : $(FREERTOS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

CMSIS_RTOS_SOURCES := $(wildcard $(CMSIS_RTOS_SRC_DIR)/*.c)
CMSIS_RTOS_OBJECTS := $(CMSIS_RTOS_SOURCES:$(CMSIS_RTOS_SRC_DIR)/%.c=$(OUT_DIR)/$(CMSIS_RTOS_SRC_DIR)/%.o)

$(CMSIS_RTOS_OBJECTS): $(OUT_DIR)/$(CMSIS_RTOS_SRC_DIR)/%.o : $(CMSIS_RTOS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

PORTABLE_COMMON_RTOS_SOURCES := $(wildcard $(PORTABLE_COMMON_RTOS_SRC_DIR)/*.c)
PORTABLE_COMMON_RTOS_OBJECTS := $(PORTABLE_COMMON_RTOS_SOURCES:$(PORTABLE_COMMON_RTOS_SRC_DIR)/%.c=$(OUT_DIR)/$(PORTABLE_COMMON_RTOS_SRC_DIR)/%.o)

$(PORTABLE_COMMON_RTOS_OBJECTS): $(OUT_DIR)/$(PORTABLE_COMMON_RTOS_SRC_DIR)/%.o : $(PORTABLE_COMMON_RTOS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

PORTABLE_GCC_RTOS_SOURCES := $(wildcard $(PORTABLE_GCC_RTOS_SRC_DIR)/*.c)
PORTABLE_GCC_RTOS_OBJECTS := $(PORTABLE_GCC_RTOS_SOURCES:$(PORTABLE_GCC_RTOS_SRC_DIR)/%.c=$(OUT_DIR)/$(PORTABLE_GCC_RTOS_SRC_DIR)/%.o)

$(PORTABLE_GCC_RTOS_OBJECTS): $(OUT_DIR)/$(PORTABLE_GCC_RTOS_SRC_DIR)/%.o : $(PORTABLE_GCC_RTOS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#################
# Build rules	#
# Tracing       #
#################
TRACE_SOURCES := $(wildcard $(TRACE_SRC_DIR)/*.c)
TRACE_OBJECTS := $(TRACE_SOURCES:$(TRACE_SRC_DIR)/%.c=$(OUT_DIR)/$(TRACE_SRC_DIR)/%.o)

$(TRACE_OBJECTS): $(OUT_DIR)/$(TRACE_SRC_DIR)/%.o : $(TRACE_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#################
# Build rules	#
# Application	#
#################
APP_SOURCES := $(wildcard $(APP_SRC_DIR)/*.c)
APP_OBJECTS := $(APP_SOURCES:$(APP_SRC_DIR)/%.c=$(OUT_DIR)/$(APP_SRC_DIR)/%.o)

$(APP_OBJECTS): $(OUT_DIR)/$(APP_SRC_DIR)/%.o : $(APP_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

###############
# Build rules #
# Main		  #
###############
all: create_out_directory $(MAIN)
	echo Done!

$(MAIN): $(HAL_OBJECTS) $(STARTUP_OBJECTS) $(BSP_OBJECTS) $(FREERTOS_OBJECTS) $(CMSIS_RTOS_OBJECTS) $(PORTABLE_COMMON_RTOS_OBJECTS) $(PORTABLE_GCC_RTOS_OBJECTS) $(TRACE_OBJECTS) $(APP_OBJECTS)
	$(CC) $(INCLUDES) $(HAL_OBJECTS) $(STARTUP_OBJECTS) $(BSP_OBJECTS) $(LFLAGS) $(FREERTOS_OBJECTS) $(CMSIS_RTOS_OBJECTS) $(PORTABLE_COMMON_RTOS_OBJECTS) $(PORTABLE_GCC_RTOS_OBJECTS) $(TRACE_OBJECTS) $(APP_OBJECTS) -o $(OUT_DIR)/$(MAIN).elf
	$(OBJCPY) -Oihex $(OUT_DIR)/$(MAIN).elf $(OUT_DIR)/$(MAIN).hex
