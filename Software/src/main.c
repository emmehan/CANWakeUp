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
        
        gpio_set_led_green(BITACTION_RESET);
        gpio_set_led_red(BITACTION_SET);

        for(int i = 0; i < 5000000; i++)
        {
            asm("nop");
        }

        gpio_set_led_green(BITACTION_SET);
        gpio_set_led_red(BITACTION_RESET);    
    }
}
