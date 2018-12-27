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
        GPIOA->BSRR |= GPIO_BSRR_BR9; //bit reset (to low)

        for(int i = 0; i < 5000000; i++)
        {
            asm("nop");
        }
        GPIOA->BSRR |= GPIO_BSRR_BS9;
        
    }
}