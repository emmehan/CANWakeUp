/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CANWAKEUP_H
#define __CANWAKEUP_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup BSP
  * @{
  */ 

typedef enum 
{
  LED0 = 0,
  LED1 = 1,
  
  LED_RED  = LED0,
  LED_GREEN = LED1
    
} Led_TypeDef;

typedef enum 
{
  SWITCH_USER = 0,
} Switch_TypeDef;

typedef enum 
{  
  SWITCH_MODE_GPIO = 0,
  SWITCH_MODE_EXTI = 1
} SwitchMode_TypeDef;

/**
 * @brief COM Types Definition
 */
typedef enum 
{
  COM0 = 0,
} COM_TypeDef;

typedef enum
{
  CAN_COM0 = 0,
} CAN_COM_TypeDef;

#define LEDn                             2

#define LED0_PIN                         GPIO_PIN_0
#define LED0_GPIO_PORT                   GPIOA
#define LED0_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()  
#define LED0_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()  

#define LED1_PIN                         GPIO_PIN_1
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()  
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()  

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if((__INDEX__) == 0) LED0_GPIO_CLK_ENABLE(); \
                                               else LED1_GPIO_CLK_ENABLE(); } while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (((__INDEX__) == 0) ? LED0_GPIO_CLK_DISABLE() : LED1_GPIO_CLK_DISABLE())
 
#define SWITCHn                          1

#define USER_SWITCH_PIN                  GPIO_PIN_2
#define USER_SWITCH_GPIO_PORT            GPIOA
#define USER_SWITCH_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_SWITCH_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOA_CLK_DISABLE()
#define USER_SWITCH_EXTI_IRQn            EXTI0_IRQn

#define SWITCHx_GPIO_CLK_ENABLE(__INDEX__)     USER_SWITCH_GPIO_CLK_ENABLE()

#define SWITCHx_GPIO_CLK_DISABLE(__INDEX__)    USER_SWITCH_GPIO_CLK_DISABLE()

/** @addtogroup STM3210E_EVAL_COM
  * @{
  */
#define COMn                                1

/**
 * @brief Definition for COM port0, connected to USART1
 */ 
#define USER_COM0                           USART1
#define USER_COM0_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USER_COM0_CLK_DISABLE()             __HAL_RCC_USART1_CLK_DISABLE()

#define USER_COM0_TX_PIN                    GPIO_PIN_9
#define USER_COM0_TX_GPIO_PORT              GPIOA
#define USER_COM0_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_COM0_TX_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()

#define USER_COM0_RX_PIN                    GPIO_PIN_10
#define USER_COM0_RX_GPIO_PORT              GPIOA
#define USER_COM0_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_COM0_RX_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()

#define COMx_CLK_ENABLE(__INDEX__)          USER_COM0_CLK_ENABLE()
#define COMx_CLK_DISABLE(__INDEX__)         USER_COM0_CLK_DISABLE()

#define COMx_TX_GPIO_CLK_ENABLE(__INDEX__)  USER_COM0_TX_GPIO_CLK_ENABLE()

#define COMx_TX_GPIO_CLK_DISABLE(__INDEX__) USER_COM0_TX_GPIO_CLK_DISABLE()

#define COMx_RX_GPIO_CLK_ENABLE(__INDEX__)  USER_COM0_RX_GPIO_CLK_ENABLE()

#define COMx_RX_GPIO_CLK_DISABLE(__INDEX__) USER_COM0_RX_GPIO_CLK_DISABLE()

/**
 * @brief Definition for COM port1, connected to CAN1
 */ 
#define CAN_COMn                                1
#define USER_CAN_COM0                           CAN1
#define USER_CAN_COM0_CLK_ENABLE()              __HAL_RCC_CAN1_CLK_ENABLE()
#define USER_CAN_COM0_CLK_DISABLE()             __HAL_RCC_CAN1_CLK_DISABLE()

#define USER_CAN_COM0_TX_PIN                    GPIO_PIN_12
#define USER_CAN_COM0_TX_GPIO_PORT              GPIOA
#define USER_CAN_COM0_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_CAN_COM0_TX_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()

#define USER_CAN_COM0_RX_PIN                    GPIO_PIN_11
#define USER_CAN_COM0_RX_GPIO_PORT              GPIOA
#define USER_CAN_COM0_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_CAN_COM0_RX_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()

#define USER_CAN_COM0_S_PIN                     GPIO_PIN_15
#define USER_CAN_COM0_S_GPIO_PORT               GPIOA
#define USER_CAN_COM0_S_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_CAN_COM0_S_GPIO_CLK_DISABLE()      __HAL_RCC_GPIOA_CLK_DISABLE()

#define CAN_COMx_CLK_ENABLE(__INDEX__)          USER_CAN_COM0_CLK_ENABLE()
#define CAN_COMx_CLK_DISABLE(__INDEX__)         USER_CAN_COM0_CLK_DISABLE()

#define CAN_COMx_TX_GPIO_CLK_ENABLE(__INDEX__)  USER_CAN_COM0_TX_GPIO_CLK_ENABLE()

#define CAN_COMx_TX_GPIO_CLK_DISABLE(__INDEX__) USER_CAN_COM0_TX_GPIO_CLK_DISABLE()

#define CAN_COMx_RX_GPIO_CLK_ENABLE(__INDEX__)  USER_CAN_COM0_RX_GPIO_CLK_ENABLE()

#define CAN_COMx_RX_GPIO_CLK_DISABLE(__INDEX__) USER_CAN_COM0_RX_GPIO_CLK_DISABLE()

#define CAN_COMx_S_GPIO_CLK_ENABLE(__INDEX__)   USER_CAN_COM0_S_GPIO_CLK_ENABLE()

#define CAN_COMx_S_GPIO_CLK_DISABLE(__INDEX__)  USER_CAN_COM0_S_GPIO_CLK_DISABLE()

void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);

void BSP_SW_Init(Switch_TypeDef Switch, SwitchMode_TypeDef Mode);
GPIO_PinState BSP_SW_GetState(Switch_TypeDef Switch);

HAL_StatusTypeDef BSP_COM_Init(COM_TypeDef Com, UART_HandleTypeDef *huart);
HAL_StatusTypeDef BSP_COM_Print(UART_HandleTypeDef *huart, char  *pData);

HAL_StatusTypeDef BSP_CAN_COM_Init(CAN_COM_TypeDef Can, CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef BSP_CAN_COM_FilterInit(CAN_COM_TypeDef Can, CAN_HandleTypeDef *hcan);

/**
  * @}
  */ 
  
#ifdef __cplusplus
}
#endif
  
#endif /* __CANWAKEUP_H */
