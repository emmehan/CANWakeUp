#include "stm32f103xb.h"
#include "system_stm32f1xx.h"


/*
 * 
 */
void SystemClockInit(void)
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
    /* set flash latency; if 0 < SYSCLK(=32MHz) ≤ 24 MHz => one wait state*/
    FLASH->ACR &= ~(FLASH_ACR_LATENCY_2);
    FLASH->ACR &= ~(FLASH_ACR_LATENCY_1);
    FLASH->ACR |= FLASH_ACR_LATENCY_0;

    /* set AHB prescaler; 0 on reset, so AHB_CLK=SYSCLK=32MHz */
    

    /* set APB low (APB1) clock; 0 on reset, so PCLK1=SYSCLK=32MHz */

    /* set APB high (APB2) clock; 0 on reset, so PCLK2=SYSCLK=32MHz */


}

void SystemInit(void)
{

    /*   System initialization
     *
     * - configuration of internal voltage regulator
     * - configuration of clock source
     * 
     */

    /* Configure system clock */
    SystemClockInit();

}