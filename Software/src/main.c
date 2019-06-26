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

#define LOG(x)  BSP_COM_Print(&UART1_Handle, x)

void SystemClock_Config(void);

GPIO_InitTypeDef GPIO_Clk_Out = 
{
  .Pin  = GPIO_PIN_8,
  .Mode = GPIO_MODE_AF_PP,
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

const CAN_InitTypeDef CAN1_Init = 
{
  .Prescaler = 4,
  .Mode = CAN_MODE_NORMAL,
  .SyncJumpWidth = CAN_SJW_1TQ,
  .TimeSeg1 = CAN_BS1_15TQ,
  .TimeSeg2 = CAN_BS1_2TQ,
  .TimeTriggeredMode = DISABLE,
  .AutoBusOff = DISABLE,
  .AutoWakeUp = ENABLE,
  .AutoRetransmission = DISABLE,
  .ReceiveFifoLocked = DISABLE,
  .TransmitFifoPriority = ENABLE,
};

int main(void)
{
  uint8_t uart1_tx_buffer[100];
  uint8_t uart1_rx_buffer[100];

  UART_HandleTypeDef UART1_Handle = 
  {
    .Init         = UART1_Init,
    .pTxBuffPtr   = uart1_tx_buffer,
    .TxXferSize   = 100,
    .TxXferCount  = 1,
    .pRxBuffPtr   = uart1_rx_buffer,
    .RxXferSize   = 100,
    .RxXferCount  = 1
  };

  CAN_HandleTypeDef CAN1_Handle = 
  {
    .Init = CAN1_Init
  };

  CAN_TxHeaderTypeDef msg_can_tx_header = {
    .StdId  = 0x12,
    .IDE    = CAN_ID_STD,
    .RTR    = CAN_RTR_DATA,
    .DLC    = 8,
    .TransmitGlobalTime = DISABLE
  };

  uint8_t msg_can_tx_data[] = 
  {
    0x11, 0x22, 0x33, 0x44,
    0x55, 0x66, 0x77, 0x88
  };

  HAL_StatusTypeDef status_can_tx = HAL_ERROR;
  uint32_t can_tx_mailbox;

  SystemClock_Config();
  HAL_Init();

  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_GREEN);

  BSP_COM_Init(COM0, &UART1_Handle);

  BSP_CAN_COM_Init(CAN_COM0, &CAN1_Handle);

  status_can_tx = HAL_CAN_Start(&CAN1_Handle);

  status_can_tx = HAL_CAN_AddTxMessage(&CAN1_Handle, &msg_can_tx_header, msg_can_tx_data, &can_tx_mailbox);

  /* Infinite loop */
  while (1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED_GREEN);
    BSP_LED_Toggle(LED_RED);
    if(GPIO_PIN_SET == BSP_SW_GetState(SWITCH_USER))
    {
      LOG("ON \r");
      status_can_tx = HAL_CAN_AddTxMessage(&CAN1_Handle, &msg_can_tx_header, msg_can_tx_data, &can_tx_mailbox);
    }
    else
    {
      LOG("OFF\r");
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