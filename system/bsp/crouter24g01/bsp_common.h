#ifndef _ROUTER_BOARD_H
#define _ROUTER_BOARD_H

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
#define BNRG_SPI_BAUDRATEPRESCALER  SPI_BAUDRATEPRESCALER_8		//SPI_BAUDRATEPRESCALER_2
#define BNRG_SPI_CRCCALCULATION     SPI_CRCCALCULATION_DISABLED

#define SPIX_TIMEOUT 								1000 /*<! Value of Timeout when SPI communication fails */

// SPI Reset Pin: PB.0
// PA.8
#define BNRG_SPI_RESET_PIN          GPIO_PIN_0
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
#define BNRG_SPI_SCLK_ALTERNATE     GPIO_AF0_SPI1
#define BNRG_SPI_SCLK_PORT          GPIOA
#define BNRG_SPI_SCLK_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// MISO (Master Input Slave Output): PA.6
#define BNRG_SPI_MISO_PIN           GPIO_PIN_6
#define BNRG_SPI_MISO_MODE          GPIO_MODE_AF_PP
#define BNRG_SPI_MISO_PULL          GPIO_NOPULL
#define BNRG_SPI_MISO_SPEED         GPIO_SPEED_HIGH
#define BNRG_SPI_MISO_ALTERNATE     GPIO_AF0_SPI1
#define BNRG_SPI_MISO_PORT          GPIOA
#define BNRG_SPI_MISO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// MOSI (Master Output Slave Input): PA.7
#define BNRG_SPI_MOSI_PIN           GPIO_PIN_7
#define BNRG_SPI_MOSI_MODE          GPIO_MODE_AF_PP
#define BNRG_SPI_MOSI_PULL          GPIO_NOPULL
#define BNRG_SPI_MOSI_SPEED         GPIO_SPEED_HIGH
#define BNRG_SPI_MOSI_ALTERNATE     GPIO_AF0_SPI1
#define BNRG_SPI_MOSI_PORT          GPIOA
#define BNRG_SPI_MOSI_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

// NSS/CSN/CS: PA.4

#define BNRG_SPI_CS_PIN             GPIO_PIN_4
#define BNRG_SPI_CS_MODE            GPIO_MODE_OUTPUT_PP
#define BNRG_SPI_CS_PULL            GPIO_PULLUP
#define BNRG_SPI_CS_SPEED           GPIO_SPEED_HIGH
#define BNRG_SPI_CS_ALTERNATE       0
#define BNRG_SPI_CS_PORT            GPIOA
#define BNRG_SPI_CS_CLK_ENABLE()    __GPIOA_CLK_ENABLE()

// IRQ: PB.1

#define BNRG_SPI_IRQ_PIN            GPIO_PIN_1
#define BNRG_SPI_IRQ_MODE           GPIO_MODE_IT_RISING
#define BNRG_SPI_IRQ_PULL           GPIO_NOPULL
#define BNRG_SPI_IRQ_SPEED          GPIO_SPEED_HIGH
#define BNRG_SPI_IRQ_ALTERNATE      0
#define BNRG_SPI_IRQ_PORT           GPIOB
#define BNRG_SPI_IRQ_CLK_ENABLE()   __GPIOB_CLK_ENABLE()

// EXTI External Interrupt for SPI
// NOTE: if you change the IRQ pin remember to implement a corresponding handler
// function like EXTI0_IRQHandler() in the user project
#define BNRG_SPI_EXTI_IRQn          EXTI0_1_IRQn
#define BNRG_SPI_EXTI_IRQHandler    EXTI0_1_IRQHandler
#define BNRG_SPI_EXTI_PIN           BNRG_SPI_IRQ_PIN
#define BNRG_SPI_EXTI_PORT          BNRG_SPI_IRQ_PORT
#define RTC_WAKEUP_IRQHandler       RTC_WKUP_IRQHandler


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


/*############################### SPI2 #######################################*/
/**
  * @brief  si24r1 spi2
  */
#define SI24R1_INF_SPIx                       SPI1
#define SI24R1_SPIx_CLK_ENABLE()            __HAL_RCC_SPI1_CLK_ENABLE()
#define SI24R1_SPIx_CLK_DISABLE()           __HAL_RCC_SPI1_CLK_DISABLE()

//spi cs PA11
#define SI24R1_CS_PORT                        GPIOA
#define SI24R1_CS_PIN                         GPIO_PIN_11
#define SI24R1_CS_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()  
#define IS24R1_CS_CLK_DISABLE()             __HAL_RCC_GPIOA_CLK_DISABLE()

//spi mosi PA7
#define SI24R1_SPI_MOSI_AF                    GPIO_AF0_SPI1
#define SI24R1_SPI_MOSI_GPIO_PORT             GPIOA
#define SI24R1_SPI_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define SI24R1_SPI_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()
#define SI24R1_SPI_MOSI_PIN                   GPIO_PIN_7

//spi miso PA6
#define SI24R1_SPI_MISO_AF                    GPIO_AF0_SPI1
#define SI24R1_SPI_MISO_GPIO_PORT             GPIOA
#define SI24R1_SPI_MISO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define SI24R1_SPI_MISO_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOA_CLK_DISABLE()
#define SI24R1_SPI_MISO_PIN                   GPIO_PIN_6

//spi clk PA5
#define SI24R1_SPI_SCK_AF                     GPIO_AF0_SPI1
#define SI24R1_SPI_SCK_GPIO_PORT              GPIOA
#define SI24R1_SPI_SCK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define SI24R1_SPI_SCK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOA_CLK_DISABLE()
#define SI24R1_SPI_SCK_PIN                    GPIO_PIN_5

//ce
#define SI24R1_CE_PORT                        GPIOA
#define SI24R1_CE_PIN                         GPIO_PIN_12
#define SI24R1_CE_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()  
#define IS24R1_CE_CLK_DISABLE()             __HAL_RCC_GPIOA_CLK_DISABLE()

//irq
#define SI24R1_IRQ_GPIO_PORT                  GPIOA
#define SI24R1_IRQ_PIN                        GPIO_PIN_8
#define SI24R1_IRQ_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define SI24R1_IRQ_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()
#define SI24R1_IRQ_IRQN                       EXTI4_15_IRQn


#define LEDn                                    1
#define LED0_PIN                                GPIO_PIN_3
#define LED0_GPIO_PORT                          GPIOB
#define LED0_GPIO_CLK_ENABLE()                  __GPIOB_CLK_ENABLE()
#define LED0_GPIO_CLK_DISABLE()                 __GPIOB_CLK_DISABLE()  
#define LEDx_GPIO_CLK_ENABLE(__INDEX__)    do{if((__INDEX__) == 0) LED0_GPIO_CLK_ENABLE(); \
                                             }while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)    do{if((__INDEX__) == 0) LED0_GPIO_CLK_DISABLE(); \
                                              }while(0)

typedef enum 
{
  LED0 = 0
}Led_TypeDef;

void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);

void BSP_SystemClock_Config(void);
void bsp_init(void);

#endif
