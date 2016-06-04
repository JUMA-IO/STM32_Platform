/**
  */
#include "debug.h"
#include "stm32l0xx_hal.h"
#include "bsp_common.h"
#include "bsp_isr.h"

#if NO_PRINTF
#define printf(...)
#endif
/* RTC handler declared in "main.c" file */
extern RTC_HandleTypeDef RTCHandle;
#ifdef I2C_DMA_MODE
extern I2C_HandleTypeDef    I2C_EXPBD_Handle;
#endif

volatile uint32_t ms_counter = 0;
volatile uint8_t button_event = 0;
/* SPI handler declared in "main.c" file */
extern SPI_HandleTypeDef SpiHandle;

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  NMI_Handler This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  HardFault_Handler This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  SVC_Handler This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  DebugMon_Handler This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  PendSV_Handler This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  SysTick_Handler This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    HAL_IncTick();
    ms_counter++;
}


/******************************************************************************/
/*                 STM32f4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles External line interrupt request for BlueNRG.
  * @param  None
  * @retval None
  */
void BNRG_SPI_EXTI_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BNRG_SPI_EXTI_PIN);
}


/**
  * @brief  This function handles the Push Button interrupt request.
  * @param  None
  * @retval None
  */
void PUSH_BUTTON_EXTI_IRQHandler(void)
{
    //HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
    button_event = 1;
}

/**
  * @brief  This function handles RTC Auto wake-up interrupt request.
  * @param  None
  * @retval None
  */
void RTC_WKUP_IRQHandler(void)
{
    //HAL_RTCEx_WakeUpTimerIRQHandler(&RTCHandle);
}

/**
  * @brief  This function handles RTC Alarm interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
    //HAL_RTC_AlarmIRQHandler(&RTCHandle);
}
/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*
void PPP_IRQHandler(void)
{
}
*/
void (*EXTI0_IRQFUNC)(void)=0;
#ifdef I2C_DMA_MODE
void (*EXTI9_5_IRQFUNC) (void)=imu_sensor_magneto_irq_callback;
#endif

#ifdef SENSOR_FIFO
/*lsm6ds3*/
void EXTI0_IRQHandler(void)
{
   if(__HAL_GPIO_EXTI_GET_IT(MEMS_INT1_PIN) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(MEMS_INT1_PIN);
    if(EXTI0_IRQFUNC)
      (*EXTI0_IRQFUNC)();

  }
}
#endif

#ifdef I2C_DMA_MODE
/*lsm303agr*/
void EXTI9_5_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(MAGNETO_INT1_PIN) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(MAGNETO_INT1_PIN);
   if(EXTI9_5_IRQFUNC)
      (EXTI9_5_IRQFUNC) ();
  }
}


void DMA_STREAM_IRQHANDLER(void)
{
    HAL_DMA_IRQHandler(I2C_EXPBD_Handle.hdmarx);         
}
#endif

#ifdef INCLUDE_USB_DEVICE
extern PCD_HandleTypeDef hpcd;
extern USBD_HandleTypeDef USBD_Device;

void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}

void OTG_FS_WKUP_IRQHandler(void)
{
  if((&hpcd)->Init.low_power_enable)
  {
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));  
    
    /* Configures system clock after wake-up from STOP: enable HSE, PLL and select 
    PLL as system clock source (HSE and PLL are disabled in STOP mode) */
    BSP_SystemClock_Config();
    
    /* ungate PHY clock */
     __HAL_PCD_UNGATE_PHYCLOCK((&hpcd)); 
  }
  /* Clear EXTI pending Bit*/
  __HAL_USB_OTG_FS_WAKEUP_EXTI_CLEAR_FLAG();
}
#endif

