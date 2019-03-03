/**************************************************************************//**
 * @file     timer.c
 * @brief    Implementation file for TIMER module
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

#include "timer.h"
#include "stm32f103xb.h"

/** @addtogroup TIMER
  * @{
  */

#define TIMER_RELOAD_VALUE_1MS      ( 0x7D00 ) /*!< Auto-Reload value for timer TIM2 to generate a periodic interrupt @ 1ms. */

/**
  \brief    Initializes the timer peripherals.
  \details  Initializes Timer TIM2 to generate an overflow interrupt every 1ms.
 */
void timer_init(void)
{
    /***************************************
     * Initialize Timer TIM2 for 1ms period
     ***************************************/

    /* Enable clock for Timer TIM2 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* No prescaling for Timer TIM2 clock */
    TIM2->CR1 &= ~(TIM_CR1_CKD_1 | TIM_CR1_CKD_0);

    /* Enable auto-preload preload */
    TIM2->CR1 |= TIM_CR1_ARPE;

    /* Edge-aligned mode */
    TIM2->CR1 &=  ~(TIM_CR1_CMS_1 | TIM_CR1_CMS_0);

    /* Upcounting timer */
    TIM2->CR1 &= ~(TIM_CR1_DIR);

    /* Disable One-Pulse mode; don't stop at update event */
    TIM2->CR1 &= ~(TIM_CR1_OPM);

    /* Set update resource so interrupt is only generated at overflow event */
    TIM2->CR1 |= TIM_CR1_URS;

    /* Enable Update event */
    TIM2->CR1 &= ~(TIM_CR1_UDIS);

    /* Set Auto-Reload value for 1ms period */
    TIM2->ARR = TIMER_RELOAD_VALUE_1MS;

    /* Enable Update Interrupt */
    TIM2->DIER |= TIM_DIER_UIE;

}

/**
  \details   Enable timer TIM2.
*/
void timer_1ms_start(void)
{
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
  \details   Disable timer TIM2.
*/
void timer_1ms_stop(void)
{
    TIM2->CR1 &= ~(TIM_CR1_CEN);
}

/**
  * @}
  */
