/**
  */

#ifndef __STM32xx_IT_H
#define __STM32xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void BNRG_SPI_EXTI_IRQHandler(void);
void PUSH_BUTTON_EXTI_IRQHandler(void);

extern void (*EXTI0_IRQFUNC)(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32xx_IT_H */

