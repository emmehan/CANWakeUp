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

#include "stm32f103xb.h"
#include "system_stm32f1xx.h"

void SystemClockInit(void)
{

    /* Enable power interface control clock */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    /* enable HSE - High Speed clock External - 8MHz */
    RCC->CR |= RCC_CR_HSEON;

    /* wait until HSE is enabled */
    while(!(RCC->CR & RCC_CR_HSERDY));

    /* disable PLL1 */
    RCC->CR &= ~RCC_CR_PLLON;

    /* wait until PLL is disabled */
    while((RCC->CR & RCC_CR_PLLRDY));

    /* set input for PLL clock to HSE */
    RCC->CFGR |= RCC_CFGR_PLLSRC;

    /* set PLL multiplier */
    RCC->CFGR |= RCC_CFGR_PLLMULL4;   //8MHz*4=32MHz PLL

    /* enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* wait until PLL is enabled */
    while(!(RCC->CR & RCC_CR_PLLRDY));

    /* switch system clock to PLL, after reset HSI is selected */
    RCC->CFGR |= RCC_CFGR_SW_PLL; //SYSCLK = 32MHz

    /* wait until system clock is switch to PLL1; RCC_CFGR_SWS[1:0] = 0x2 */
    while( (RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL );

    /* disable internal clock */
    RCC->CR &= ~(RCC_CR_HSION);

    /* wait until HSI is disabled */
    while((RCC->CR & RCC_CR_HSION));

    /* set Flash parameters */
    /* set flash latency; if 0 < SYSCLK(=32MHz) ≤ 24 MHz => one wait state*/
    FLASH->ACR &= ~(FLASH_ACR_LATENCY_2);
    FLASH->ACR &= ~(FLASH_ACR_LATENCY_1);
    FLASH->ACR |= FLASH_ACR_LATENCY_0;
}

void SystemInit(void)
{
    /*   System initialization
     *
     * - configuration of internal voltage regulator
     * - configuration of clock source
     * 
     */

    /* Configure system clock */
    SystemClockInit();
}