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

#include "gpio.h"

void gpio_init()
{

    /* enable clock for port A */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;


    /************************************************
     * Configure Pin PA8 as alternate function (MCO)
     ************************************************/
    GPIOA->CRH  |= (GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0);
    GPIOA->CRH &= ~(GPIO_CRH_CNF8);
    GPIOA->CRH |= (GPIO_CRH_CNF8_1);

    /* set MCO clock output to SYSCLK = 32MHz */
    RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;

    /************************************************
     * Configure Pin PA0 as GPIO output (LED RED)
     ************************************************/
    GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
    GPIOA->CRL &= ~(GPIO_CRL_CNF0);

    /* set initially to high -> LED off */
    GPIOA->BSRR |= GPIO_BSRR_BS0;

    /************************************************
     * Configure Pin PA1 as GPIO output (LED GREEN)
     ************************************************/
    GPIOA->CRL |= (GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);
    GPIOA->CRL &= ~(GPIO_CRL_CNF1);
    
    /* set initially to high -> LED off */
    GPIOA->BSRR |= GPIO_BSRR_BS1;

    /************************************************
     * Configure Pin PA0 as GPIO input (SWITCH)
     ************************************************/
    GPIOA->CRL &= ~(GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);
    GPIOA->CRL &= ~(GPIO_CRL_CNF1); 
    GPIOA->CRL |= GPIO_CRL_CNF0;

    /*****************************************************
     * Configure Pin PA10 as GPIO output (CAN SILENT_MODE)
     *****************************************************/
    GPIOA->CRH |= (GPIO_CRH_MODE10_1 | GPIO_CRH_MODE10_0);
    GPIOA->CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_CNF10_0);

    /*****************************************************
     * Configure Pin PA11 as alternate function (CAN RX)
     *****************************************************/
    GPIOA->CRH |= (GPIO_CRH_MODE11_1 | GPIO_CRH_MODE11_0);
    GPIOA->CRH |= (GPIO_CRH_CNF11_1);
    GPIOA->CRH &= ~(GPIO_CRH_CNF11_0);

    /*****************************************************
     * Configure Pin PA12 as alternate function (CAN TX)
     *****************************************************/
    GPIOA->CRH |= (GPIO_CRH_MODE12_1 | GPIO_CRH_MODE12_0);
    GPIOA->CRH |= (GPIO_CRH_CNF12_1);
    GPIOA->CRH &= ~(GPIO_CRH_CNF12_0);
}

void gpio_set_led_red(BITACTION_t _new_state)
{
    if(BITACTION_RESET == _new_state)
    {
        GPIOA->BSRR |= GPIO_BSRR_BR0; //bit reset (to low)
    }
    else if(BITACTION_SET == _new_state)
    {
        GPIOA->BSRR |= GPIO_BSRR_BS0; //bit set (to high)
    }
}

void gpio_set_led_green(BITACTION_t _new_state)
{
    if(BITACTION_RESET == _new_state)
    {
        GPIOA->BSRR |= GPIO_BSRR_BR1; //bit reset (to low)
    }
    else if(BITACTION_SET == _new_state)
    {
        GPIOA->BSRR |= GPIO_BSRR_BS1; //bit set (to high)
    }
}

void gpio_set_can_silent_mode(BITACTION_t _new_state)
{
    if(BITACTION_RESET == _new_state)
    {
        GPIOA->BSRR |= GPIO_BSRR_BR10; //bit reset (to low)
    }
    else if(BITACTION_SET == _new_state)
    {
        GPIOA->BSRR |= GPIO_BSRR_BS10; //bit set (to high)
    }
}

BITACTION_t gpio_read_button(void)
{
    static uint32_t portvals = 0;
    BITACTION_t retval = BITACTION_RESET;
    
    /* read button state */
    portvals = GPIOA->IDR;
    if(portvals & (0x00000004))
    {
        retval = BITACTION_SET;
    }

    return retval;
}