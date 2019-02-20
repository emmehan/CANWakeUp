#include "gpio.h"

void gpio_init()
{
    /* configure MCO pin PA8 (alternate function) */

    /* enable clock for port A */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* set output mode; max 50MHz */
    GPIOA->CRH  |= (GPIO_CRH_MODE8_1 | GPIO_CRH_MODE8_0);

    /* set alternate function */
    GPIOA->CRH &= ~(GPIO_CRH_CNF8);
    GPIOA->CRH |= (GPIO_CRH_CNF8_1);

    /* set MCO clock output to SYSCLK = 32MHz */
    RCC->CFGR |= RCC_CFGR_MCO_SYSCLK;

    /* configure Pin A0 LED */
    GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
    GPIOA->CRL &= ~(GPIO_CRL_CNF0);

    /* set initially to high -> LED off */
    GPIOA->BSRR |= GPIO_BSRR_BS0;

    /* configure Pin A1 LED */
    GPIOA->CRL |= (GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);
    GPIOA->CRL &= ~(GPIO_CRL_CNF1);
    
    /* set initially to high -> LED off */
    GPIOA->BSRR |= GPIO_BSRR_BS1;

    /* configure Pin A2 Button */
    GPIOA->CRL &= ~(GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);   //input mode
    GPIOA->CRL &= ~(GPIO_CRL_CNF1); 
    GPIOA->CRL |= GPIO_CRL_CNF0;
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
