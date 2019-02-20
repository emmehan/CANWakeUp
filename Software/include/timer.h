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

void timer_init(void);
void timer_systick_start(void);
void timer_systick_stop(void);
void timer_systick_reset(void);