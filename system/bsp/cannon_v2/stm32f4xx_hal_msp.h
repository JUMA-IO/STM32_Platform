#ifndef __STM32F4XX_HAL_MSP_H
#define __STM32F4XX_HAL_MSP_H

#include "stm32f4xx.h"

void UART_Init(void);
int Uart_Tx_String(char *string, uint16_t len);
int Uart_Rx_String(char *string, uint16_t len, uint32_t timeout);

extern UART_HandleTypeDef Uart2Handle;


#define HAL_MSP_SUCCESS     (0)
#define HAL_MSP_FAIL        (1)

/* Definition for USARTx1 clock resources */
#define USARTx1                           USART1
#define USARTx1_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx1_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx1_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USARTx1_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx1_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx1 Pins */
#define USARTx1_TX_PIN                    GPIO_PIN_9
#define USARTx1_TX_GPIO_PORT              GPIOA  
#define USARTx1_TX_AF                     GPIO_AF7_USART1
#define USARTx1_RX_PIN                    GPIO_PIN_10
#define USARTx1_RX_GPIO_PORT              GPIOA 
#define USARTx1_RX_AF                     GPIO_AF7_USART1


/* Definition for USARTx2 clock resources */
#define USARTx2                           USART2
#define USARTx2_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTx2_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx2_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USARTx2_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx2_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx2 Pins */
#define USARTx2_TX_PIN                    GPIO_PIN_2
#define USARTx2_TX_GPIO_PORT              GPIOA  
#define USARTx2_TX_AF                     GPIO_AF7_USART2
#define USARTx2_RX_PIN                    GPIO_PIN_3
#define USARTx2_RX_GPIO_PORT              GPIOA 
#define USARTx2_RX_AF                     GPIO_AF7_USART2
  

#endif /* __STM32F4XX_HAL_MSP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
