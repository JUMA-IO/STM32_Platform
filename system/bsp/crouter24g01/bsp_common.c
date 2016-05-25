/**
  */

#include "stm32l0xx_hal.h"
#include "bsp_common.h"
#include "stm32_bluenrg_ble.h"
#include "hci.h"


GPIO_TypeDef* GPIO_PORT[LEDn] = {LED0_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED0_PIN};
SPI_HandleTypeDef g_spi_handle;
static SPI_HandleTypeDef cannon_si24r1_Spi;
static void SPIx_Error (void);
	
void bsp_init(void)
{
    HAL_Init();
    /* Configure the system clock */
    //BSP_SystemClock_Config();
    /* Configure LED0 */
    BSP_LED_Init(LED0);
    /*UART2 init*/
    UART_Init();
    HAL_Delay(1000);
    /* Initialize the BlueNRG SPI driver */
    BNRG_SPI_Init();
    /* Initialize the BlueNRG HCI */
    HCI_Init();
    /* Reset BlueNRG hardware */
    BlueNRG_RST();
    /* Enable Power Clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    //..rtc_init();
}

/**
  */
void BSP_LED_Init(Led_TypeDef Led)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_LED Clock */
    LEDx_GPIO_CLK_ENABLE(Led);

    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin = GPIO_PIN[Led];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

/**
  */
void BSP_LED_On(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}

void BSP_LED_Off(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}

void BSP_LED_Toggle(Led_TypeDef Led)
{
    HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/
/**
  * @brief  Initializes SPI MSP.
  * @param  None
  * @retval None
  */
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*** Configure the GPIOs ***/
    /* Enable GPIO clock */
 //   NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE();
 //   NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();

    /* Configure SPI SCK */
//    GPIO_InitStruct.Pin = NUCLEO_SPIx_SCK_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull  = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//    GPIO_InitStruct.Alternate = NUCLEO_SPIx_SCK_AF;
//    HAL_GPIO_Init(NUCLEO_SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

//    /* Configure SPI MISO and MOSI */
//    GPIO_InitStruct.Pin = NUCLEO_SPIx_MOSI_PIN;
//    GPIO_InitStruct.Alternate = NUCLEO_SPIx_MISO_MOSI_AF;
//    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
//    HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = NUCLEO_SPIx_MISO_PIN;
//    HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

    /*** Configure the SPI peripheral ***/
    /* Enable SPI clock */
//    NUCLEO_SPIx_CLK_ENABLE();
}

/**
  * @brief  Initializes SPI HAL.
  * @param  None
  * @retval None
  */
static void SPIx_Init(void)
{
    if(HAL_SPI_GetState(&g_spi_handle) == HAL_SPI_STATE_RESET)
    {
        /* SPI Config */
        g_spi_handle.Instance = BNRG_SPI_INSTANCE;
        /* SPI baudrate is set to 12,5 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 100/8 = 12,5 MHz)
         to verify these constraints:
            - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
              Since the provided driver doesn't use read capability from LCD, only constraint
              on write baudrate is considered.
            - SD card SPI interface max baudrate is 25MHz for write/read
            - PCLK2 max frequency is 100 MHz
         */
        g_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
        g_spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        g_spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
        g_spi_handle.Init.CRCPolynomial = 7;
        g_spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
        g_spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
        g_spi_handle.Init.NSS = SPI_NSS_SOFT;
        g_spi_handle.Init.TIMode = SPI_TIMODE_DISABLED;
        g_spi_handle.Init.Mode = SPI_MODE_MASTER;

        SPIx_MspInit(&g_spi_handle);
        HAL_SPI_Init(&g_spi_handle);
    }
}

/**
  * @brief  SPI Read 4 bytes from device.
  * @param  None
  * @retval Read data
*/
static uint32_t SPIx_Read(void)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t readvalue = 0;
    uint32_t writevalue = 0xFFFFFFFF;

    status = HAL_SPI_TransmitReceive(&g_spi_handle, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SPIX_TIMEOUT);


    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* Execute user timeout callback */
        SPIx_Error();
    }

    return readvalue;
}

/**
  * @brief  SPI Write a byte to device.
  * @param  Value: value to be written
  * @retval None
  */
static void SPIx_Write(uint8_t Value)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_SPI_Transmit(&g_spi_handle, (uint8_t*) &Value, 1, SPIX_TIMEOUT);

    /* Check the communication status */
    if(status != HAL_OK)
    {
        /* Execute user timeout callback */
        SPIx_Error();
    }
}

/**
  * @brief  SPI error treatment function.
  * @param  None
  * @retval None
  */
static void SPIx_Error (void)
{
    /* De-initialize the SPI communication BUS */
    HAL_SPI_DeInit(&g_spi_handle);

    /* Re-Initiaize the SPI communication BUS */
    SPIx_Init();
}

/**
 * @brief  System Clock Configuration
 * @param  None
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    //RCC_OscInitStruct.HSICalibrationValue = 6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
 //   RCC_OscInitStruct.PLL.PLLM = 16;
 //   RCC_OscInitStruct.PLL.PLLN = 336;
 //   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
 //   RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//..    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

__IO uint32_t uwCounter = 0;

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    uwCounter++;
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    BSP_LED_On(LED0);
}

/**
 * @brief  EXTI line detection callback.
 * @param  Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    HCI_Isr();
}

void Error_Handler(void)
{
    while (1)
    {
        /* Toggle LED2 with a period of one second */
        BSP_LED_Toggle(LED0);
        HAL_Delay(1000);
    }
}

/********************************* Si24r1 ************************************/



uint32_t si24r1_spi_cs_init(void)
{
  GPIO_InitTypeDef gpio_initStruct;
  
  SI24R1_CS_CLK_ENABLE();
  
  gpio_initStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_initStruct.Pin   = SI24R1_CS_PIN;
  gpio_initStruct.Speed = GPIO_SPEED_HIGH; 
  gpio_initStruct.Pull  = GPIO_PULLUP;
  
  
  HAL_GPIO_Init(SI24R1_CS_PORT, &gpio_initStruct);
  
  HAL_GPIO_WritePin(SI24R1_CS_PORT, SI24R1_CS_PIN, GPIO_PIN_SET);
  return 0;
}

uint32_t si24r1_spi_cs_enable(void)
{
	
  HAL_GPIO_WritePin(SI24R1_CS_PORT, SI24R1_CS_PIN, GPIO_PIN_RESET);
  return 0;
}

uint32_t si24r1_spi_cs_disable(void)
{
  HAL_GPIO_WritePin(SI24R1_CS_PORT, SI24R1_CS_PIN, GPIO_PIN_SET);
  return 0;
}

uint32_t si24r1_spi_rw(uint8_t const * const w_data, uint8_t * r_data, uint8_t data_len)
{  
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t readvalue = 0;
	
  status = HAL_SPI_TransmitReceive(&cannon_si24r1_Spi, (uint8_t *)w_data, r_data, data_len, SPIX_TIMEOUT);
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }

  readvalue = r_data[0];
  return readvalue;
}

void static si24r1_spi_low_level_config(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;  
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  SI24R1_SPI_MOSI_GPIO_CLK_ENABLE();
  SI24R1_SPI_MISO_GPIO_CLK_ENABLE();
  SI24R1_SPI_SCK_GPIO_CLK_ENABLE();
  
  /* Configure SPI SCK */
  GPIO_InitStruct.Pin = SI24R1_SPI_SCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = SI24R1_SPI_SCK_AF;
  HAL_GPIO_Init(SI24R1_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

  /* Configure SPI MOSI */ 
  GPIO_InitStruct.Pin = SI24R1_SPI_MOSI_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate = SI24R1_SPI_MOSI_AF;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SI24R1_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);

  /* Configure SPI MISO */ 
  GPIO_InitStruct.Pin = SI24R1_SPI_MISO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate = SI24R1_SPI_MISO_AF;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SI24R1_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

  /*** Configure the SPI peripheral ***/ 
  /* Enable SPI clock */
  SI24R1_SPIx_CLK_ENABLE();
}

uint32_t si24r1_spi_init(void)
{
  if(HAL_SPI_GetState(&cannon_si24r1_Spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    cannon_si24r1_Spi.Instance = SI24R1_INF_SPIx;
    
    cannon_si24r1_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    cannon_si24r1_Spi.Init.Direction = SPI_DIRECTION_2LINES;
    cannon_si24r1_Spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    cannon_si24r1_Spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    cannon_si24r1_Spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    cannon_si24r1_Spi.Init.CRCPolynomial = 7;
    cannon_si24r1_Spi.Init.DataSize = SPI_DATASIZE_8BIT;
    cannon_si24r1_Spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    cannon_si24r1_Spi.Init.NSS = SPI_NSS_SOFT;
    cannon_si24r1_Spi.Init.TIMode = SPI_TIMODE_DISABLE;
    cannon_si24r1_Spi.Init.Mode = SPI_MODE_MASTER;
    
    si24r1_spi_low_level_config();
    HAL_SPI_Init(&cannon_si24r1_Spi);
  }
  
  return 0;
}

uint32_t si24r1_ce_enable(void)
{ 
	
  HAL_GPIO_WritePin(SI24R1_CE_PORT, SI24R1_CE_PIN, GPIO_PIN_SET);	

  return 0;
}

uint32_t si24r1_ce_disable(void)
{

  HAL_GPIO_WritePin(SI24R1_CE_PORT, SI24R1_CE_PIN, GPIO_PIN_RESET);	
  return 0;
}

uint32_t si24r1_ce_init(void)
{
  GPIO_InitTypeDef gpio_initStruct;
  
  SI24R1_CE_CLK_ENABLE();
  
  gpio_initStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_initStruct.Pin   = SI24R1_CE_PIN;
  gpio_initStruct.Speed = GPIO_SPEED_HIGH; 
  gpio_initStruct.Pull  = GPIO_PULLUP;
  
  HAL_GPIO_Init(SI24R1_CE_PORT, &gpio_initStruct);
  
  HAL_GPIO_WritePin(SI24R1_CE_PORT, SI24R1_CE_PIN, GPIO_PIN_RESET);
  return 0;
}

uint32_t si24r1_irq_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct; 
  
  SI24R1_IRQ_GPIO_CLK_ENABLE();
  
	/* Configure SPI IRQ */
	GPIO_InitStruct.Pin = SI24R1_IRQ_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(SI24R1_IRQ_GPIO_PORT, &GPIO_InitStruct);

  HAL_NVIC_SetPriority((IRQn_Type) SI24R1_IRQ_IRQN, 0x02, 0x00);
	HAL_NVIC_EnableIRQ((IRQn_Type) SI24R1_IRQ_IRQN);
  
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
  
  return 0;
}

uint32_t si24r1_irq_enable(void)
{
  HAL_NVIC_EnableIRQ(SI24R1_IRQ_IRQN);

  return 0;
}

uint32_t si24r1_irq_disable(void)
{
  HAL_NVIC_DisableIRQ(SI24R1_IRQ_IRQN);
  
  return 0;
}
