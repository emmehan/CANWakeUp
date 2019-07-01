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

/* UART logging macro */
//#define BSP_LOG(x)  BSP_COM_Print(&UART1_Handle, x)
#define BSP_LOG(x)

void SystemClock_Config(void);
HAL_StatusTypeDef CAN_Send(CAN_HandleTypeDef* hcan, uint32_t* can_mailbox);

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

uint8_t uart1_tx_buffer[100];
uint8_t uart1_rx_buffer[100];

UART_HandleTypeDef UART1_Handle;
CAN_HandleTypeDef CAN1_Handle;

/* FreeRTOS stuff */
void task_1(void*);
void task_2(void*);
#define STACK_SIZE 200
StackType_t xStack_1[ STACK_SIZE ];
StackType_t xStack_2[ STACK_SIZE ];

StaticTask_t xTaskBuffer_1, xTaskBuffer_2;

int main(void)
{
  UART1_Handle.Init         = UART1_Init;
  UART1_Handle.pTxBuffPtr   = uart1_tx_buffer;
  UART1_Handle.TxXferSize   = 100;
  UART1_Handle.TxXferCount  = 1;
  UART1_Handle.pRxBuffPtr   = uart1_rx_buffer;
  UART1_Handle.RxXferSize   = 100;
  UART1_Handle.RxXferCount  = 1;

  CAN1_Handle.Init = CAN1_Init;

  HAL_StatusTypeDef status_can_tx = HAL_ERROR;
  uint32_t can_tx_mailbox;

  SystemClock_Config();
  if(HAL_OK != HAL_Init())
  {
    while(1){};
  }

  PWR_PVDTypeDef pwrpvdstruct = {
    .PVDLevel = PWR_PVDLEVEL_7,
    .Mode = PWR_PVD_MODE_IT_RISING_FALLING,
  };

  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_PWR_ConfigPVD(&pwrpvdstruct);

  HAL_NVIC_SetPriority(PVD_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(PVD_IRQn);

  //__HAL_PWR_PVD_EXTI_ENABLE_IT();

  HAL_PWR_EnablePVD();

  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_GREEN);

  if(HAL_OK != BSP_COM_Init(COM0, &UART1_Handle))
  {
    while(1){};
  }

  if(HAL_OK != BSP_CAN_COM_Init(CAN_COM0, &CAN1_Handle))
  {
    while(1){};
  }

  if(HAL_OK != BSP_CAN_COM_FilterInit(CAN_COM0, &CAN1_Handle))
  {
    while(1){};
  }

  HAL_CAN_Start(&CAN1_Handle);

  HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 1, 1);
  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 1, 1);
  HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 1, 1);
  HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 1, 1);

  HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

  HAL_CAN_ActivateNotification(&CAN1_Handle,\
    CAN_IT_TX_MAILBOX_EMPTY\
    | CAN_IT_RX_FIFO0_MSG_PENDING //| CAN_IT_RX_FIFO0_FULL | CAN_IT_RX_FIFO0_OVERRUN
    | CAN_IT_RX_FIFO1_MSG_PENDING //| CAN_IT_RX_FIFO1_FULL | CAN_IT_RX_FIFO1_OVERRUN \
    // | CAN_IT_BUSOFF | CAN_IT_ERROR_WARNING | CAN_IT_ERROR_PASSIVE | CAN_IT_ERROR
  );

  status_can_tx = CAN_Send(&CAN1_Handle, &can_tx_mailbox);

  /* Create task 1 - LED Green Blinky */
  xTaskCreateStatic(task_1, "LED_GREEN", STACK_SIZE, (void*) 1, tskIDLE_PRIORITY, xStack_1, &xTaskBuffer_1);

  /* Create task 2 - LED Red Blinky */
  xTaskCreateStatic(task_2, "LED_RED", STACK_SIZE, (void*) 1, tskIDLE_PRIORITY, xStack_2, &xTaskBuffer_2);

  vTaskStartScheduler();
}

void task_1(void* args)
{
  uint32_t can_tx_mailbox;
  HAL_StatusTypeDef status_can_tx = HAL_ERROR;
  while(1)
  {
    if(GPIO_PIN_SET == BSP_SW_GetState(SWITCH_USER))
    {
      BSP_LOG("Sending \r\n");
      status_can_tx = CAN_Send(&CAN1_Handle, &can_tx_mailbox);
    }
    else
    {
      BSP_LED_Off(LED_GREEN);
    }
    
    HAL_Delay(1000);
  }
}

void task_2(void* args)
{
  while(1)
  {
    vTaskDelay(100/portTICK_PERIOD_MS);
    BSP_LED_Toggle(LED_RED);
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

HAL_StatusTypeDef CAN_Send(CAN_HandleTypeDef *hcan, uint32_t *can_mailbox)
{
  static CAN_TxHeaderTypeDef msg_can_tx_header = {
    .ExtId  = 0x14420FFF,
    .IDE    = CAN_ID_EXT,
    .RTR    = CAN_RTR_DATA,
    .DLC    = 8,
    .TransmitGlobalTime = DISABLE
  };

  static uint8_t msg_can_tx_data[] = 
  {
    0x55, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
  };

  return HAL_CAN_AddTxMessage(hcan, &msg_can_tx_header, msg_can_tx_data, can_mailbox);
}

void PVD_IRQHandler(void)
{
  HAL_PWR_PVD_IRQHandler();
}

void HAL_PWR_PVDCallback(void)
{
  BSP_LOG("Undervoltage <2.9V detected!\r\n");
  BSP_LED_On(LED_RED);
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&CAN1_Handle);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&CAN1_Handle);
}

void CAN1_RX1_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&CAN1_Handle);
}

void CAN1_SCE_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&CAN1_Handle);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
  BSP_LED_Toggle(LED_GREEN);
  BSP_LOG("CAN TX MB0 complete.\r\n");
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
  BSP_LED_Toggle(LED_GREEN);
  BSP_LOG("CAN TX MB1 complete.\r\n");
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
  BSP_LED_Toggle(LED_GREEN);
  BSP_LOG("CAN TX MB2 complete.\r\n");
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  static CAN_RxHeaderTypeDef can_rx_header;
  static CAN_TxHeaderTypeDef can_tx_header;
  static uint32_t can_tx_mailbox;
  static uint8_t can_rx_data[8];

  BSP_LOG("CAN RX FIFO 0\r\n");
  /* Read message and echo back with ID+1 */
  HAL_CAN_GetRxMessage(hcan, 0, &can_rx_header, can_rx_data);

  if(0x7FF == can_rx_header.StdId)
  {
    can_tx_header.StdId = 0;
  }
  else
  {
    can_tx_header.StdId = can_rx_header.StdId + 1;
  }
  can_tx_header.IDE    = can_rx_header.IDE;
  can_tx_header.RTR    = can_rx_header.RTR;
  can_tx_header.DLC    = can_rx_header.DLC;

  HAL_CAN_AddTxMessage(hcan, &can_tx_header, can_rx_data, &can_tx_mailbox);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  BSP_LOG("CAN RX FIFO 1\r\n");
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  BSP_LOG("CAN Error - CAN state: ");
  char can_status[2] = {(char)(48+hcan->State), '\0'};  //48 = 0x30 = '0' ASCII
  BSP_LOG(can_status);
  BSP_LOG("\r\n");

  /* On error switch on LED_RED and switch off LED_GREEN */
  BSP_LED_On(LED_RED);
  BSP_LED_Off(LED_GREEN);
}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}