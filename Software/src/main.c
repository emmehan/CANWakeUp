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

volatile uint64_t i = 0;

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
        for(i = 5000000; i > 0; i--){}

        gpio_set_led_red(BITACTION_SET);

        for(i = 5000000; i > 0; i--){}

        gpio_set_led_red(BITACTION_RESET);
    }
}

void TIM2_IRQHandler(void)
{
    static uint8_t  LED_FLAG = 1;
    static uint16_t interrupt_register = 0;

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
