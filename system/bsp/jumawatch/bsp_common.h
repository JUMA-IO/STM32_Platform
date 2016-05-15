/**
  */

#ifndef __BSP_COMMON_H
#define __BSP_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f4xx.h"
                                              
typedef enum 
{
  LED3 = 0,
  LED4 = 1
}Led_TypeDef;

typedef enum 
{  
  BUTTON_KEY = 0,
}Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;     

/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL_Exported_Constants
  * @{
  */ 

#define SYSCLK_FREQ 														84000000
#define LEDn                                    2
#define LED3_PIN                                GPIO_PIN_13
#define LED3_GPIO_PORT                          GPIOG
#define LED3_GPIO_CLK_ENABLE()                  __GPIOG_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()                 __GPIOG_CLK_DISABLE()  
#define LED4_PIN                                GPIO_PIN_14
#define LED4_GPIO_PORT                          GPIOG
#define LED4_GPIO_CLK_ENABLE()                  __GPIOG_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()                 __GPIOG_CLK_DISABLE()  
#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LED3_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) LED4_GPIO_CLK_ENABLE(); \
                                            }while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED3_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED4_GPIO_CLK_DISABLE(); \
                                            }while(0)
/**
  * @}
  */ 
  
/** @addtogroup STM32F429I_DISCOVERY_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                                1

/**
 * @brief Wakeup push-button
 */
#define KEY_BUTTON_PIN                         GPIO_PIN_0
#define KEY_BUTTON_GPIO_PORT                   GPIOA
#define KEY_BUTTON_GPIO_CLK_ENABLE()           __GPIOA_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()          __GPIOA_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn                   EXTI0_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)     do{if((__INDEX__) == 0) KEY_BUTTON_GPIO_CLK_ENABLE(); \
                                                 }while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    do{if((__INDEX__) == 0) KEY_BUTTON_GPIO_CLK_DISABLE(); \
                                                 }while(0)
/**
  * @}
  */

/** @addtogroup STM32F429I_DISCOVERY_LOW_LEVEL_BUS
  * @{
  */  
/* Exported constanIO --------------------------------------------------------*/
#define IO_I2C_ADDRESS                      0x82 
#define TS_I2C_ADDRESS                      0x82

#ifdef EE_M24LR64
#define EEPROM_I2C_ADDRESS_A01              0xA0
#define EEPROM_I2C_ADDRESS_A02              0xA6
#endif /* EE_M24LR64 */ 

/*############################### I2Cx #######################################*/
/* User can use this section to tailor I2Cx instance used and associated 
   resources */
#define DISCOVERY_I2Cx                          I2C3
#define DISCOVERY_I2Cx_CLOCK_ENABLE()           __I2C3_CLK_ENABLE()
#define DISCOVERY_I2Cx_FORCE_RESET()            __I2C3_FORCE_RESET()
#define DISCOVERY_I2Cx_RELEASE_RESET()          __I2C3_RELEASE_RESET()
#define DISCOVERY_I2Cx_SDA_GPIO_CLK_ENABLE()    __GPIOC_CLK_ENABLE()
#define DISCOVERY_I2Cx_SCL_GPIO_CLK_ENABLE()    __GPIOA_CLK_ENABLE() 
#define DISCOVERY_I2Cx_SDA_GPIO_CLK_DISABLE()   __GPIOC_CLK_DISABLE()

/* Definition for DISCO I2Cx Pins */
#define DISCOVERY_I2Cx_SCL_PIN                  GPIO_PIN_8
#define DISCOVERY_I2Cx_SCL_GPIO_PORT            GPIOA
#define DISCOVERY_I2Cx_SCL_SDA_AF               GPIO_AF4_I2C3
#define DISCOVERY_I2Cx_SDA_PIN                  GPIO_PIN_9
#define DISCOVERY_I2Cx_SDA_GPIO_PORT            GPIOC

/* Definition for IOE I2Cx's NVIC */
#define DISCOVERY_I2Cx_EV_IRQn                  I2C3_EV_IRQn
#define DISCOVERY_I2Cx_ER_IRQn                  I2C3_ER_IRQn

/* I2C clock speed configuration (in Hz) 
  WARNING: 
   Make sure that this define is not already declared in other files.
   It can be used in parallel by other modules. */
#ifndef BSP_I2C_SPEED
 #define BSP_I2C_SPEED                          100000
#endif /* BSP_I2C_SPEED */

#define I2Cx_TIMEOUT_MAX                    0x3000 /*<! The value of the maximal timeout for I2C waiting loops */

/*############################### SPI3 #######################################*/
#define DISCOVERY_SPIx                          SPI3
#define DISCOVERY_SPIx_CLK_ENABLE()             __SPI3_CLK_ENABLE()
#define DISCOVERY_SPIx_MISO_MOSI_GPIO_PORT          GPIOC                      /* GPIOC */
#define DISCOVERY_SPIx_SCK_GPIO_PORT                GPIOC                      /* GPIOC */
#define DISCOVERY_SPIx_CS_GPIO_PORT                 GPIOA                      /* GPIOA */
#define DISCOVERY_SPIx_AF                       GPIO_AF6_SPI3
#define DISCOVERY_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()        __GPIOC_CLK_ENABLE()
#define DISCOVERY_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()       __GPIOC_CLK_DISABLE()
#define DISCOVERY_SPIx_SCK_GPIO_CLK_ENABLE()              __GPIOC_CLK_ENABLE()
#define DISCOVERY_SPIx_SCK_GPIO_CLK_DISABLE()             __GPIOC_CLK_DISABLE()
#define DISCOVERY_SPIx_CS_GPIO_CLK_ENABLE()               __GPIOA_CLK_ENABLE()
#define DISCOVERY_SPIx_CS_GPIO_CLK_DISABLE()              __GPIOA_CLK_DISABLE()
#define DISCOVERY_SPIx_SCK_PIN                  GPIO_PIN_10                 /* PC.10 */
#define DISCOVERY_SPIx_MISO_PIN                 GPIO_PIN_11                /* PC.11 */
#define DISCOVERY_SPIx_MOSI_PIN                 GPIO_PIN_12                 /* PC.12 */
#define DISCOVERY_SPIx_CS_PIN                   GPIO_PIN_15                /*PA15*/ 
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPIx_TIMEOUT_MAX              ((uint32_t)0x1000)


/*################################ IOE #######################################*/
/** 
  * @brief  IOE Control pin  
  */ 
/* Definition for external IT for STMPE811 */
#define STMPE811_INT_PIN                        GPIO_PIN_15
#define STMPE811_INT_GPIO_PORT                  GPIOA
#define STMPE811_INT_CLK_ENABLE()               __GPIOA_CLK_ENABLE()
#define STMPE811_INT_CLK_DISABLE()              __GPIOA_CLK_DISABLE()
#define STMPE811_INT_EXTI                       EXTI15_10_IRQn
#define STMPE811_INT_EXTIHandler                EXTI15_10_IRQHandler

/*################################ LCD #######################################*/
/* Chip Select macro definition */
#define LCD_CS_LOW()       HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()      HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)

/* Set WRX High to send data */
#define LCD_SDA_LOW()      HAL_GPIO_WritePin(LCD_SDA_GPIO_PORT, LCD_SDA_PIN, GPIO_PIN_RESET)
#define LCD_SDA_HIGH()     HAL_GPIO_WritePin(LCD_SDA_GPIO_PORT, LCD_SDA_PIN, GPIO_PIN_SET)

/* Set WRX High to send data */
#define LCD_RDX_LOW()      HAL_GPIO_WritePin(LCD_RDX_GPIO_PORT, LCD_RDX_PIN, GPIO_PIN_RESET)
#define LCD_RDX_HIGH()     HAL_GPIO_WritePin(LCD_RDX_GPIO_PORT, LCD_RDX_PIN, GPIO_PIN_SET)

/*Set or Reset Rs*/
#define LCD_RS_LOW()      HAL_GPIO_WritePin(LCD_RS_GPIO_PORT, LCD_RS_PIN, GPIO_PIN_RESET)
#define LCD_RS_HIGH()     HAL_GPIO_WritePin(LCD_RS_GPIO_PORT, LCD_RS_PIN, GPIO_PIN_SET)

/*Set or Reset RST*/
#define LCD_RESET_LOW()      HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_RESET)
#define LCD_RESET_HIGH()     HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_SET)

/** 
  * @brief  LCD Control pin  
  */ 
#define LCD_CS_PIN                             GPIO_PIN_15
#define LCD_CS_GPIO_PORT                       GPIOA
#define LCD_CS_GPIO_CLK_ENABLE()               __GPIOA_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()              __GPIOA_CLK_DISABLE()
/*LCD RS*/
#define LCD_RS_PIN                             GPIO_PIN_10
#define LCD_RS_GPIO_PORT                       GPIOC
#define LCD_RS_GPIO_CLK_ENABLE()               __GPIOC_CLK_ENABLE()
#define LCD_RS_GPIO_CLK_DISABLE()              __GPIOC_CLK_DISABLE()
/*LCD RST*/
#define LCD_RESET_PIN                             GPIO_PIN_12
#define LCD_RESET_GPIO_PORT                       GPIOA
#define LCD_RESET_GPIO_CLK_ENABLE()               __GPIOA_CLK_ENABLE()
#define LCD_RESET_GPIO_CLK_DISABLE()              __GPIOA_CLK_DISABLE()


#define LCD_WRX_PIN                             GPIO_PIN_10
#define LCD_WRX_GPIO_PORT                       GPIOC
#define LCD_WRX_GPIO_CLK_ENABLE()               __GPIOC_CLK_ENABLE()
#define LCD_WRX_GPIO_CLK_DISABLE()              __GPIOC_CLK_DISABLE()
/*LCD SDA*/  
#define LCD_SDA_PIN                             GPIO_PIN_12
#define LCD_SDA_GPIO_PORT                       GPIOC
#define LCD_SDA_GPIO_CLK_ENABLE()               __GPIOC_CLK_ENABLE()
#define LCD_SDA_GPIO_CLK_DISABLE()              __GPIOC_CLK_DISABLE()

/*################################ GYROSCOPE #################################*/
/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/* Chip Select macro definition */
#define GYRO_CS_LOW()       HAL_GPIO_WritePin(GYRO_CS_GPIO_PORT, GYRO_CS_PIN, GPIO_PIN_RESET)
#define GYRO_CS_HIGH()      HAL_GPIO_WritePin(GYRO_CS_GPIO_PORT, GYRO_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  GYROSCOPE SPI Interface pins
  */
#define GYRO_CS_PIN                             GPIO_PIN_1                  /* PC.01 */
#define GYRO_CS_GPIO_PORT                       GPIOC                       /* GPIOC */
#define GYRO_CS_GPIO_CLK_ENABLE()               __GPIOC_CLK_ENABLE()
#define GYRO_CS_GPIO_CLK_DISABLE()              __GPIOC_CLK_DISABLE()

#define GYRO_INT_GPIO_CLK_ENABLE()              __GPIOA_CLK_ENABLE()
#define GYRO_INT_GPIO_CLK_DISABLE()             __GPIOA_CLK_DISABLE()
#define GYRO_INT_GPIO_PORT                      GPIOA                       /* GPIOA */
#define GYRO_INT1_PIN                           GPIO_PIN_1                  /* PA.01 */
#define GYRO_INT1_EXTI_IRQn                     EXTI1_IRQn 
#define GYRO_INT2_PIN                           GPIO_PIN_2                  /* PA.02 */
#define GYRO_INT2_EXTI_IRQn                     EXTI2_IRQn 
/**
  * @}
  */ 

#ifdef EE_M24LR64
/** @addtogroup STM32F429I_DISCOVERY_LOW_LEVEL_I2C_EEPROM
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */
#define EEPROM_I2C_DMA                          DMA1   
#define EEPROM_I2C_DMA_CHANNEL                  DMA_CHANNEL_3
#define EEPROM_I2C_DMA_STREAM_TX                DMA1_Stream4
#define EEPROM_I2C_DMA_STREAM_RX                DMA1_Stream2
#define EEPROM_I2C_DMA_CLK_ENABLE()             __DMA1_CLK_ENABLE()
   
#define EEPROM_I2C_DMA_TX_IRQn                  DMA1_Stream4_IRQn
#define EEPROM_I2C_DMA_RX_IRQn                  DMA1_Stream2_IRQn
#define EEPROM_I2C_DMA_TX_IRQHandler            DMA1_Stream4_IRQHandler
#define EEPROM_I2C_DMA_RX_IRQHandler            DMA1_Stream2_IRQHandler
#define EEPROM_I2C_DMA_PREPRIO                  0
/**
  * @}
  */ 

#endif /* EE_M24LR64 */

/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL_Exported_Macros
  * @{
  */  
/**
  * @}
  */ 
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

/** @defgroup STM32F429I_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
uint32_t BSP_GetVersion(void);  
void     BSP_LED_Init(Led_TypeDef Led);
void     BSP_LED_On(Led_TypeDef Led);
void     BSP_LED_Off(Led_TypeDef Led);
void     BSP_LED_Toggle(Led_TypeDef Led);
void     BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
uint32_t BSP_PB_GetState(Button_TypeDef Button);


#ifdef __cplusplus
}
#endif

#endif /* __BSP_COMMON_H */

