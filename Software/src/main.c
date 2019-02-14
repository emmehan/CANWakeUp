#include "main.h"
#include "gpio.h"

int main()
{

    gpio_init();

    /* Toggle LED on Pin A9 */
    while(1)
    {
        for(int i = 0; i < 5000000; i++)
        {
            asm("nop");
        }
        GPIOA->BSRR |= GPIO_BSRR_BR0; //bit reset (to low)
        GPIOA->BSRR |= GPIO_BSRR_BS1; //bit set (to high)

        for(int i = 0; i < 5000000; i++)
        {
            asm("nop");
        }
        GPIOA->BSRR |= GPIO_BSRR_BS0;
        GPIOA->BSRR |= GPIO_BSRR_BR1;       
    }
}