/***************************************äinbclude***********************************//**
 * @file     main.h
 * @brief    Main header file.
 * @version  V1.0
 * @date     03.03.2019
 ******************************************************************************/

/*
Application for interfacing a TJA1050 CAN transceiver with a STM32F103C8T6 MCU.
Copyright (C) 2019  Jonas Heim
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stm32f1xx_hal.h"
#include "bsp_canwakeup.h"
#include "FreeRTOS.h"
#include "task.h"
