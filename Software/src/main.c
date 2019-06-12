/**
  ******************************************************************************
  * @file    ADC/ADC_Regular_injected_groups/Src/main.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    14-April-2017
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral to perform conversions using the 2 ADC groups: 
  *          group regular for ADC conversions on main stream and 
  *          group injected for ADC conversions limited on specific events
  *          (conversions injected within main conversions stream). Other 
  *          peripherals used: DMA, TIM (ADC group regular conversions 
  *          triggered  by TIM, ADC group regular conversion data
  *          transfered by DMA).
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

void SystemClock_Config(void);

GPIO_InitTypeDef GPIO_Led_Red = 
{
  .Pin  = GPIO_PIN_0,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Pull = GPIO_PULLDOWN,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

GPIO_InitTypeDef GPIO_Led_Green = 
{
  .Pin  = GPIO_PIN_1,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Pull = GPIO_PULLDOWN,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

GPIO_InitTypeDef GPIO_SWITCH = 
{
  .Pin  = GPIO_PIN_2,
  .Mode = GPIO_MODE_INPUT,
  .Pull = GPIO_PULLUP,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

GPIO_InitTypeDef GPIO_Clk_Out = 
{
  .Pin  = GPIO_PIN_8,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_PULLDOWN,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

GPIO_InitTypeDef GPIO_UART_TX = 
{
  .Pin  = GPIO_PIN_9,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_PULLDOWN,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

GPIO_InitTypeDef GPIO_UART_RX = 
{
  .Pin  = GPIO_PIN_10,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_PULLDOWN,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

GPIO_InitTypeDef GPIO_CAN_TX = 
{
  .Pin  = GPIO_PIN_11,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_PULLDOWN,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

GPIO_InitTypeDef GPIO_CAN_RX = 
{
  .Pin  = GPIO_PIN_12,
  .Mode = GPIO_MODE_AF_PP,
  .Pull = GPIO_PULLDOWN,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

GPIO_InitTypeDef GPIO_CAN_S = 
{
  .Pin  = GPIO_PIN_15,
  .Mode = GPIO_MODE_OUTPUT_PP,
  .Pull = GPIO_PULLDOWN,
  .Speed  = GPIO_SPEED_FREQ_HIGH,
};

const UART_InitTypeDef UART1_Init = 
{
  .BaudRate     = 9600,
  .WordLength   = UART_WORDLENGTH_8B,
  .StopBits     = UART_STOPBITS_1,
  .Parity       = UART_PARITY_NONE,
  .Mode         = UART_MODE_TX_RX,
  .HwFlowCtl    = UART_HWCONTROL_NONE,
  .OverSampling = UART_OVERSAMPLING_16,
};

int main(void)
{
  uint8_t uart1_tx_buffer[100];
  uint8_t uart1_rx_buffer[100];
  uint8_t uart_tx_switch_on[] = { 'S', 'w', 'i', 't', 'c', 'h', ' ', 'O', 'N', ' ', '\r' };
  uint8_t uart_tx_switch_off[] = { 'S', 'w', 'i', 't', 'c', 'h', ' ', 'O', 'F', 'F', '\r'};

  UART_HandleTypeDef UART1_Handle = 
  {
    .Instance     = USART1,
    .Init         = UART1_Init,
    .pTxBuffPtr   = uart1_tx_buffer,
    .TxXferSize   = 100,
    .TxXferCount  = 1,
    .pRxBuffPtr   = uart1_rx_buffer,
    .RxXferSize   = 100,
    .RxXferCount  = 1
  };

  SystemClock_Config();
  /* STM32F1xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &GPIO_Led_Red);
  HAL_GPIO_Init(GPIOA, &GPIO_Led_Green);
  HAL_GPIO_Init(GPIOA, &GPIO_SWITCH);
  HAL_GPIO_Init(GPIOA, &GPIO_Clk_Out);
  HAL_GPIO_Init(GPIOA, &GPIO_UART_TX);
  HAL_GPIO_Init(GPIOA, &GPIO_UART_RX);
  HAL_GPIO_Init(GPIOA, &GPIO_CAN_TX);
  HAL_GPIO_Init(GPIOA, &GPIO_CAN_RX);
  HAL_GPIO_Init(GPIOA, &GPIO_CAN_S);

  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

  __HAL_RCC_USART1_CLK_ENABLE();
  HAL_UART_Init(&UART1_Handle);
  
  /* Infinite loop */
  while (1)
  {
    HAL_Delay(500);
    
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    if( GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) )
    {
      HAL_UART_Transmit(&UART1_Handle, uart_tx_switch_off, sizeof(uart_tx_switch_off)/sizeof(uint8_t), 100000);
    }
    else
    {
      HAL_UART_Transmit(&UART1_Handle, uart_tx_switch_on, sizeof(uart_tx_switch_on)/sizeof(uint8_t), 100000);
    }
  }
}

/*
 * Set SYSCLK   = 72MHz
 * Set APB1CLK  = 36MHz
 * Set APB2CLK  = 72MHz
 */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Configure PLLs------------------------------------------------------*/
  /* PLL2 configuration: PLL2CLK=(HSE/HSEPrediv2Value)*PLL2MUL=(25/5)*8=40 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLL2CLK / HSEPredivValue = 40 / 5 = 8 MHz */
  /* PLL configuration: PLLCLK = PREDIV1CLK * PLLMUL = 8 * 9 = 72 MHz */
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
  
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  { /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks
  dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  { /* Initialization Error */
    while(1);
  }
}