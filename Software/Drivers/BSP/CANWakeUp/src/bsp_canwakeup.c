#include "bsp_canwakeup.h"

GPIO_TypeDef*   LED_PORT[LEDn] = {LED0_GPIO_PORT,
                                  LED1_GPIO_PORT};

const uint16_t  LED_PIN[LEDn]  = {LED0_PIN,
                                  LED1_PIN};


GPIO_TypeDef*   SWITCH_PORT[SWITCHn]  = {USER_SWITCH_GPIO_PORT};
const uint16_t  SWITCH_PIN[SWITCHn]   = {USER_SWITCH_PIN};
const uint8_t   SWITCH_IRQn[SWITCHn]  = {USER_SWITCH_EXTI_IRQn};

/**
 * @brief COM variables
 */
USART_TypeDef* COM_USART[COMn]   = {USER_COM0}; 

GPIO_TypeDef* COM_TX_PORT[COMn]   = {USER_COM0_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn]   = {USER_COM0_RX_GPIO_PORT};

const uint16_t COM_TX_PIN[COMn]   = {USER_COM0_TX_PIN};

const uint16_t COM_RX_PIN[COMn]   = {USER_COM0_RX_PIN};

/**
 * @brief CAN variables
 */
CAN_TypeDef* CAN_COM[CAN_COMn]   = {USER_CAN_COM0};

GPIO_TypeDef* CAN_COM_TX_PORT[CAN_COMn]   = {USER_CAN_COM0_TX_GPIO_PORT};
 
GPIO_TypeDef* CAN_COM_RX_PORT[CAN_COMn]   = {USER_CAN_COM0_RX_GPIO_PORT};

GPIO_TypeDef* CAN_COM_S_PORT[CAN_COMn]   = {USER_CAN_COM0_S_GPIO_PORT};

const uint16_t CAN_COM_TX_PIN[CAN_COMn]   = {USER_CAN_COM0_TX_PIN};

const uint16_t CAN_COM_RX_PIN[CAN_COMn]   = {USER_CAN_COM0_RX_PIN};

const uint16_t CAN_COM_S_PIN[CAN_COMn]   = {USER_CAN_COM0_S_PIN};

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED0/LED_RED
  *     @arg LED1/LED_GREEN
  */
void BSP_LED_Init(Led_TypeDef Led)
{
    GPIO_InitTypeDef  gpioinitstruct = {0};

    /* Enable the GPIO_LED Clock */
    LEDx_GPIO_CLK_ENABLE(Led);

    /* Configure the GPIO_LED pin */
    gpioinitstruct.Pin    = LED_PIN[Led];
    gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpioinitstruct.Pull   = GPIO_NOPULL;
    gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED_PORT[Led], &gpioinitstruct);

    /* Reset PIN to switch off the LED */
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

void BSP_LED_On(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

void BSP_LED_Off(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

void BSP_LED_Toggle(Led_TypeDef Led)
{
    HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

void BSP_SW_Init(Switch_TypeDef Switch, SwitchMode_TypeDef Mode)
{
    GPIO_InitTypeDef  gpioinitstruct = {0};

    /* Enable the GPIO_SWITCH Clock */
    SWITCHx_GPIO_CLK_ENABLE(Switch);

    /* Configure the GPIO_LED pin */
    gpioinitstruct.Pin    = SWITCH_PIN[Switch];
    gpioinitstruct.Mode   = GPIO_MODE_INPUT;
    gpioinitstruct.Pull   = GPIO_NOPULL;
    gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SWITCH_PORT[Switch], &gpioinitstruct);
}

GPIO_PinState BSP_SW_GetState(Switch_TypeDef Switch)
{
    return HAL_GPIO_ReadPin(SWITCH_PORT[Switch], SWITCH_PIN[Switch]);
}

void BSP_COM_Init(COM_TypeDef Com, UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  gpioinitstruct = {0};

    COMx_TX_GPIO_CLK_ENABLE(Com);
    COMx_RX_GPIO_CLK_ENABLE(Com);

    COMx_CLK_ENABLE(Com);

    /* Configure UART Tx as AF PP */
    gpioinitstruct.Pin      = COM_TX_PIN[Com];
    gpioinitstruct.Mode     = GPIO_MODE_AF_PP;
    gpioinitstruct.Speed    = GPIO_SPEED_FREQ_HIGH;
    gpioinitstruct.Pull     = GPIO_PULLUP;
    HAL_GPIO_Init(COM_TX_PORT[Com], &gpioinitstruct);

    /* Configure UART Rx as AF PP */
    gpioinitstruct.Pin      = COM_RX_PIN[Com];
    gpioinitstruct.Mode     = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(COM_RX_PORT[Com], &gpioinitstruct);

    huart->Instance         = COM_USART[Com];
    HAL_UART_Init(huart);
}

size_t strlen(char* string)
{
    size_t retval = 0;

    if(string)
    {
        for(int i = 0; ;i++)
        {
            if(string[i] == '\0')
            {
                //found string termination
                break;
            }
            retval++;
        }
    }
    return retval;
}

void BSP_COM_Print(UART_HandleTypeDef *huart, char *pData)
{
    if(pData)
    {
        if(strlen(pData))
        {
            HAL_UART_Transmit(huart, (unsigned char*) pData, strlen(pData), 1000);
        }
    }
}

void BSP_CAN_COM_Init(CAN_COM_TypeDef Can, CAN_HandleTypeDef *hcan)
{
    HAL_StatusTypeDef status_CAN_init;
    GPIO_InitTypeDef  gpioinitstruct = {0};
    
    CAN_COMx_TX_GPIO_CLK_ENABLE(Can);
    CAN_COMx_RX_GPIO_CLK_ENABLE(Can);
    CAN_COMx_S_GPIO_CLK_ENABLE(Can);

    CAN_COMx_CLK_ENABLE(Can);

    /* Configure CAN Tx as AF PP */
    gpioinitstruct.Pin      = CAN_COM_TX_PIN[Can];
    gpioinitstruct.Mode     = GPIO_MODE_AF_OD;
    gpioinitstruct.Speed    = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CAN_COM_TX_PORT[Can], &gpioinitstruct);

    /* Configure CAN Rx as AF Input */
    gpioinitstruct.Pin      = CAN_COM_RX_PIN[Can];
    gpioinitstruct.Mode     = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(CAN_COM_RX_PORT[Can], &gpioinitstruct);

    /* Configure CAN S as Output PP */
    gpioinitstruct.Pin      = CAN_COM_S_PIN[Can];
    gpioinitstruct.Mode     = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(CAN_COM_S_PORT[Can], &gpioinitstruct);
    /* Set CAN S low -> Enable CAN Transceiver Normal/HighSpeed Mode */
    // HAL_GPIO_WritePin(CAN_COM_S_PORT[Can], CAN_COM_S_PIN[Can], GPIO_PIN_RESET);
    // HAL_GPIO_WritePin(CAN_COM_S_PORT[Can], CAN_COM_S_PIN[Can], GPIO_PIN_SET);

    hcan->Instance         = CAN_COM[Can];
    status_CAN_init = HAL_CAN_Init(hcan);
}
