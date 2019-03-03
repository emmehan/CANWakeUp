/**************************************************************************//**
 * @file     system_stm32f10x.c
 * @brief    Implementation file for initial MCU configuration after power up.
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

#include "stm32f103xb.h"
#include "system_stm32f1xx.h"

/**
  \brief    Initializes the system clocks.
  \details  Initializes HSE as clock source (external 8MHz quartz).
            Initializes PLL to 32MHz system clock with HSE as source.
            Configures internal flash parameters.
            Configures AHB prescaler (no prescaler, AHB clock 32MHz)
            Configures APB1 prescaler (no prescaler, APB1 clock 32MHz)
            Configures APB2 prescaler (no prescaler, APB2 clock 32MHz)
 */
void SystemCoreClockUpdate(void)
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

    /* enable Flash prefetch buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* set flash latency; if 0 < SYSCLK(=32MHz) ≤ 24 MHz => one wait state*/
    FLASH->ACR &= ~(FLASH_ACR_LATENCY_2);
    FLASH->ACR &= ~(FLASH_ACR_LATENCY_1);
    FLASH->ACR |= FLASH_ACR_LATENCY_0;

    /* set AHB prescaler to 0 = no scaling = 32MHz */
    RCC->CFGR &= ~(RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_0 );

    /* set APB1 prescaler to 0 = no scaling  = 32MHz*/
    RCC->CFGR &= ~( RCC_CFGR_PPRE1_2 | RCC_CFGR_PPRE1_1 | RCC_CFGR_PPRE1_0);

    /* set APB2 prescaler to 0 = no scaling  = 32MHz*/
    RCC->CFGR &= ~( RCC_CFGR_PPRE2_2 | RCC_CFGR_PPRE2_1 | RCC_CFGR_PPRE2_0);

    /* Vector Table reolocation to internal flash */
    SCB->VTOR = FLASH_BASE;

}

/**
  \brief    Initializes the system hardware.
  \details   Calls function for clock configuration.
 */
void SystemInit(void)
{
     /* Configure system clock */
    SystemCoreClockUpdate();
}
