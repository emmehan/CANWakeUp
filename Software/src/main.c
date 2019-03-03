/**************************************************************************//**
 * @file     main.c
 * @brief    Main file of project
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

#include "main.h"

static volatile uint64_t delay_counter = 0;    /*!< Counter variable used for active waiting delays. */

/**
  \brief    Called after hardware initialization.
  \details  Initializes GPIO module.
            Initializes TIMER module.
            Enabled NVIC interrupt for timer TIM2.
            Set signals LED_RED and LED_GREEN to off (BITACTION_SET).
            Toggles LED_RED in static period.
 */
int main()
{
    gpio_init();
    timer_init();

    /* Enable Timer2 interrupt */
    NVIC_EnableIRQ(TIM2_IRQn);

    /* Start 1ms timer */
    timer_1ms_start();

    gpio_set_led_green(BITACTION_SET);
    gpio_set_led_red(BITACTION_SET);

    while(1)
    {
        for(delay_counter = 5000000; delay_counter > 0; delay_counter--){}

        gpio_set_led_red(BITACTION_SET);

        for(delay_counter = 5000000; delay_counter > 0; delay_counter--){}

        gpio_set_led_red(BITACTION_RESET);
    }
}

/**
  \brief    Interrupt service routine for Timer TIM2.
  \details  Is called if a interrupt is triggered by the TIM2 Timer module. 
            Withing the service routine the interrupt cause is distinguished.
            The interrupt flag of the timer module is cleared manually.
            Toggles status of LED_GREEN signal when called.
 */
void TIM2_IRQHandler(void)
{
    static uint8_t  LED_FLAG = 1;
    uint16_t interrupt_register = 0;

    /* read TIM2 status register */
    interrupt_register = TIM2->SR;

    if(interrupt_register & TIM_SR_UIF)
    {
        /* Overflow event is interrupt source */
        if(LED_FLAG)
        {
            gpio_set_led_green(BITACTION_SET);
            LED_FLAG = 0 ;
        }
        else
        {
            gpio_set_led_green(BITACTION_RESET);
            LED_FLAG = 1;
        }
    }

    /*
    * Clear TIM2 interrupt flag
    * Writing a 1 to the status register has no effect so it's safe to do
    */
    TIM2->SR &= ~(TIM_SR_UIF);
}
