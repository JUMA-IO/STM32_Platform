/*
 *
 *  JUMA.IO - JUMA SDK for STM families
 *
 *  Copyright (C) 2013-2016  JUMA Technology
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Apache V2 License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#ifndef __BSP_COMMON_H
#define __BSP_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f4xx.h"

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
 
typedef enum 
{
  LED0 = 0
}Led_TypeDef;

typedef enum 
{  
  BUTTON_KEY = 0
}Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define SYSCLK_FREQ 														84000000

#define LEDn                                    1
#define LED0_PIN                                GPIO_PIN_3
#define LED0_GPIO_PORT                          GPIOB
#define LED0_GPIO_CLK_ENABLE()                  __GPIOB_CLK_ENABLE()
#define LED0_GPIO_CLK_DISABLE()                 __GPIOB_CLK_DISABLE()  
#define LEDx_GPIO_CLK_ENABLE(__INDEX__)    do{if((__INDEX__) == 0) LED0_GPIO_CLK_ENABLE();}while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)    do{if((__INDEX__) == 0) LED0_GPIO_CLK_DISABLE();}while(0)

#define BUTTONn                                 1  
#define KEY_BUTTON_PIN                          GPIO_PIN_1
#define KEY_BUTTON_GPIO_PORT                    GPIOA
#define KEY_BUTTON_GPIO_CLK_ENABLE()            __GPIOA_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()           __GPIOA_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn                    EXTI1_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    do{if((__INDEX__) == 0) KEY_BUTTON_GPIO_CLK_ENABLE();}while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    do{if((__INDEX__) == 0) KEY_BUTTON_GPIO_CLK_DISABLE();}while(0)

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define NUCLEO_SPIx                                     SPI1
#define NUCLEO_SPIx_CLK_ENABLE()                        __SPI1_CLK_ENABLE()

#define NUCLEO_SPIx_SCK_AF                              GPIO_AF5_SPI1
#define NUCLEO_SPIx_SCK_GPIO_PORT                       GPIOA
#define NUCLEO_SPIx_SCK_PIN                             GPIO_PIN_5
#define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()               __GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()              __GPIOA_CLK_DISABLE()

#define NUCLEO_SPIx_MISO_MOSI_AF                        GPIO_AF5_SPI1
#define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT                 GPIOA
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()         __GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()        __GPIOA_CLK_DISABLE()
#define NUCLEO_SPIx_MISO_PIN                            GPIO_PIN_6
#define NUCLEO_SPIx_MOSI_PIN                            GPIO_PIN_7
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define NUCLEO_SPIx_TIMEOUT_MAX                   1000
/**
* @brief SPI communication details between Nucleo F4 and BlueNRG
*        Expansion Board.
*/
// SPI Instance
#define BNRG_SPI_INSTANCE           SPI1
#define BNRG_SPI_CLK_ENABLE()       __SPI1_CLK_ENABLE()

// SPI Configuration
#define BNRG_SPI_MODE               SPI_MODE_MASTER
#define BNRG_SPI_DIRECTION          SPI_DIRECTION_2LINES
#define BNRG_SPI_DATASIZE           SPI_DATASIZE_8BIT
#define BNRG_SPI_CLKPOLARITY        SPI_POLARITY_LOW
#define BNRG_SPI_CLKPHASE           SPI_PHASE_1EDGE
#define BNRG_SPI_NSS                SPI_NSS_SOFT
#define BNRG_SPI_FIRSTBIT           SPI_FIRSTBIT_MSB
#define BNRG_SPI_TIMODE             SPI_TIMODE_DISABLED
#define BNRG_SPI_CRCPOLYNOMIAL      7
#define BNRG_SPI_BAUDRATEPRESCALER  SPI_BAUDRATEPRESCALER_2
#define BNRG_SPI_CRCCALCULATION     SPI_CRCCALCULATION_DISABLED

// SPI Reset Pin: PB.1
// PA.8
#define BNRG_SPI_RESET_PIN          GPIO_PIN_1
#define BNRG_SPI_RESET_MODE         GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_RESET_PULL         GPIO_PULLUP
#define BNRG_SPI_RESET_SPEED        GPIO_SPEED_LOW
#define BNRG_SPI_RESET_ALTERNATE    0
#define BNRG_SPI_RESET_PORT         GPIOB
#define BNRG_SPI_RESET_CLK_ENABLE() __GPIOB_CLK_ENABLE()

// SCLK: PA.5
// PB.3
#define BNRG_SPI_SCLK_PIN           GPIO_PIN_5
#define BNRG_SPI_SCLK_MODE          GPIO_MODE_AF_PP
#define BNRG_SPI_SCLK_PULL          GPIO_PULLDOWN
#define BNRG_SPI_SCLK_SPEED         GPIO_SPEED_HIGH
#define BNRG_SPI_SCLK_ALTERNATE     GPIO_AF5_SPI1
#define BNRG_SPI_SCLK_PORT          GPIOA
#define BNRG_SPI_SCLK_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// MISO (Master Input Slave Output): PA.6
#define BNRG_SPI_MISO_PIN           GPIO_PIN_6
#define BNRG_SPI_MISO_MODE          GPIO_MODE_AF_PP
#define BNRG_SPI_MISO_PULL          GPIO_NOPULL
#define BNRG_SPI_MISO_SPEED         GPIO_SPEED_HIGH
#define BNRG_SPI_MISO_ALTERNATE     GPIO_AF5_SPI1
#define BNRG_SPI_MISO_PORT          GPIOA
#define BNRG_SPI_MISO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// MOSI (Master Output Slave Input): PA.7
#define BNRG_SPI_MOSI_PIN           GPIO_PIN_7
#define BNRG_SPI_MOSI_MODE          GPIO_MODE_AF_PP
#define BNRG_SPI_MOSI_PULL          GPIO_NOPULL
#define BNRG_SPI_MOSI_SPEED         GPIO_SPEED_HIGH
#define BNRG_SPI_MOSI_ALTERNATE     GPIO_AF5_SPI1
#define BNRG_SPI_MOSI_PORT          GPIOA
#define BNRG_SPI_MOSI_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// NSS/CSN/CS: PA.4
// PA.1
#define BNRG_SPI_CS_PIN             GPIO_PIN_4
#define BNRG_SPI_CS_MODE            GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_CS_PULL            GPIO_PULLUP
#define BNRG_SPI_CS_SPEED           GPIO_SPEED_HIGH
#define BNRG_SPI_CS_ALTERNATE       0
#define BNRG_SPI_CS_PORT            GPIOA
#define BNRG_SPI_CS_CLK_ENABLE()    __GPIOA_CLK_ENABLE()

// IRQ: PC.4
// PA.0
#define BNRG_SPI_IRQ_PIN            GPIO_PIN_4
#define BNRG_SPI_IRQ_MODE           GPIO_MODE_IT_RISING
#define BNRG_SPI_IRQ_PULL           GPIO_NOPULL
#define BNRG_SPI_IRQ_SPEED          GPIO_SPEED_HIGH
#define BNRG_SPI_IRQ_ALTERNATE      0
#define BNRG_SPI_IRQ_PORT           GPIOC
#define BNRG_SPI_IRQ_CLK_ENABLE()   __GPIOC_CLK_ENABLE()

// EXTI External Interrupt for SPI
// NOTE: if you change the IRQ pin remember to implement a corresponding handler
// function like EXTI0_IRQHandler() in the user project
#define BNRG_SPI_EXTI_IRQn          EXTI4_IRQn
#define BNRG_SPI_EXTI_IRQHandler    EXTI4_IRQHandler
#define BNRG_SPI_EXTI_PIN           BNRG_SPI_IRQ_PIN
#define BNRG_SPI_EXTI_PORT          BNRG_SPI_IRQ_PORT
#define RTC_WAKEUP_IRQHandler       RTC_WKUP_IRQHandler

//EXTI External Interrupt for user button
#define PUSH_BUTTON_EXTI_IRQHandler EXTI15_10_IRQHandler
/**
  * @}
  */

/** @defgroup STM32F4XX_NUCLEO_BLUENRG_Exported_Functions
  * @{
  */
  
void Enable_SPI_IRQ(void);
void Disable_SPI_IRQ(void);
void Clear_SPI_IRQ(void);
void Clear_SPI_EXTI_Flag(void);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  SD Control Lines management
*/
#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  SD Control Lines management
*/
#define SD_CS_PIN                                 GPIO_PIN_5
#define SD_CS_GPIO_PORT                           GPIOB
#define SD_CS_GPIO_CLK_ENABLE()                 __GPIOB_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                __GPIOB_CLK_DISABLE()

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  LCD Control Lines management
*/
#define LCD_CS_LOW()      HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()     HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW()      HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()     HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)
     
/*
  LCD Control Interface pins
*/
#define LCD_CS_PIN                                 GPIO_PIN_6
#define LCD_CS_GPIO_PORT                           GPIOB
#define LCD_CS_GPIO_CLK_ENABLE()                 __GPIOB_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()                __GPIOB_CLK_DISABLE()
    
/*
  LCD Data/Command Interface pins
*/
#define LCD_DC_PIN                                 GPIO_PIN_9
#define LCD_DC_GPIO_PORT                           GPIOA
#define LCD_DC_GPIO_CLK_ENABLE()                 __GPIOA_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()                __GPIOA_CLK_DISABLE()

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  ADC Interface pins
*/
#define NUCLEO_ADCx                                 ADC1
#define NUCLEO_ADCx_CLK_ENABLE()                  __ADC1_CLK_ENABLE()
#define NUCLEO_ADCx_CHANNEL                       ADC_CHANNEL_8
#define NUCLEO_ADCx_GPIO_PORT                       GPIOB
#define NUCLEO_ADCx_GPIO_PIN                        GPIO_PIN_0
#define NUCLEO_ADCx_GPIO_CLK_ENABLE()             __GPIOB_CLK_ENABLE()
#define NUCLEO_ADCx_GPIO_CLK_DISABLE()            __GPIOB_CLK_DISABLE()

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void bsp_init(void);

void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void BSP_SystemClock_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_COMMON_H */
