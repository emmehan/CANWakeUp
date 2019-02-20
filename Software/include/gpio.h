#include "stm32f103xb.h"

typedef enum 
{
    BITACTION_SET,
    BITACTION_RESET
} BITACTION_t;

void gpio_init(void);

void gpio_set_led_red(BITACTION_t);
void gpio_set_led_green(BITACTION_t);
BITACTION_t gpio_read_button(void);
