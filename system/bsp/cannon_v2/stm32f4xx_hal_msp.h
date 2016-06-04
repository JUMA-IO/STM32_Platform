#ifndef __STM32F4XX_HAL_MSP_H
#define __STM32F4XX_HAL_MSP_H

#include "stm32f4xx.h"

void UART_Init(void);
int Uart_Tx_String(char *string, uint16_t len);
int Uart_Rx_String(char *string, uint16_t len, uint32_t timeout);



#define HAL_MSP_SUCCESS     (0)
#define HAL_MSP_FAIL        (1)


/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA  
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA 
#define USARTx_RX_AF                     GPIO_AF7_USART2
  

#endif /* __STM32F4XX_HAL_MSP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
