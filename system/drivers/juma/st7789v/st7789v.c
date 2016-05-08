/**
  ******************************************************************************
  * @file    st7789v.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    02-December-2014
  * @brief   This file includes the LCD driver for st7789v LCD.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "st7789v.h"
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup ST7789V
  * @brief This file provides a set of functions needed to drive the 
  *        ST7789V LCD.
  * @{
  */

/** @defgroup ST7789V_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup ST7789V_Private_Defines
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup ST7789V_Private_Macros
  * @{
  */
/**
  * @}
  */  

/** @defgroup ST7789V_Private_Variables
  * @{
  */ 

LCD_DrvTypeDef   st7789v_drv = 
{
  st7789v_Init,
  st7789v_ReadID,
  st7789v_DisplayOn,
  st7789v_DisplayOff,
  0,
  0,
  0,
  0,
  0,
  0,
  st7789v_GetLcdPixelWidth,
  st7789v_GetLcdPixelHeight,
  0,
  0,    
};

/**
  * @}
  */ 
  
/** @defgroup st7789v_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup st7789v_Private_Functions
  * @{
  */   

uint32_t g_LCD = 0;

/**
  * @brief  Power on the LCD.
  * @param  None
  * @retval None
  */
void st7789v_Init(void)
{
  /* Initialize st7789v low level bus layer ----------------------------------*/
  LCD_IO_Init();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,0x01);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,0x01);
  LCD_RESET_HIGH();
  HAL_Delay(1);              //delay 1ms
  LCD_RESET_LOW();
  HAL_Delay(10);             //delay 10ms
  LCD_RESET_HIGH();
  HAL_Delay(120);
  
  /* Configure LCD */
  st7789v_WriteReg(0x11);
  HAL_Delay(120);
  st7789v_WriteReg(0x36); //Memory Data Access Control
  st7789v_WriteData(0x00); 

  st7789v_WriteReg(0x3a);//Interface Pixel Format
  st7789v_WriteData(0x05);//MCU-16bit  05
 
  st7789v_WriteReg(0x2a);
  st7789v_WriteData(0x00);
  st7789v_WriteData(0x00);
  st7789v_WriteData(0x00);
  st7789v_WriteData(0xef);
  
  st7789v_WriteReg(0x2b);
  st7789v_WriteData(0x00);
  st7789v_WriteData(0x00);
  st7789v_WriteData(0x00);
  st7789v_WriteData(0xef);
  
  /*frame rate setting*/
  st7789v_WriteReg(0xb2);//portch
  st7789v_WriteData(0x0c);//normal BP
  st7789v_WriteData(0x0c);//normal FP
  st7789v_WriteData(0x00);//enable seperate
  st7789v_WriteData(0x33);//idle, BP[7:4], FP[3:0]
  st7789v_WriteData(0x33);//partial, BP[7:4], FP[3:0]
  
  st7789v_WriteReg(0xb7); //gate control
  st7789v_WriteData(0x56);//VGH=14.06v,VGL=-11.38
  /*power setting*/
  st7789v_WriteReg(0xbb); //VCOMS Setting
  st7789v_WriteData(0x1e);
  
  st7789v_WriteReg(0xc0);
  st7789v_WriteData(0x2c);
  
  st7789v_WriteReg(0xc2);
  st7789v_WriteData(0x01);
  
  st7789v_WriteReg(0xc3); //VRH Set
  st7789v_WriteData(0x13); //4.5v
  
  st7789v_WriteReg(0xc4);
  st7789v_WriteData(0x20);
  
  st7789v_WriteReg(0xc6); //frame rate control in normal mode
  st7789v_WriteData(0x0f);
  
  st7789v_WriteReg(0xd0); // power control 1
  st7789v_WriteData(0xa4);//
  st7789v_WriteData(0xa1);//AVDD=6.8v, AVCL=-4.8v, VDS=2.3V
  
  //----GAMMA------------------------------------

  st7789v_WriteReg(0xe0);
  st7789v_WriteData(0xD0);
  st7789v_WriteData(0x03);
  st7789v_WriteData(0x08);
  st7789v_WriteData(0x0E);
  st7789v_WriteData(0x11);
  st7789v_WriteData(0x2B);
  st7789v_WriteData(0x3B);
  st7789v_WriteData(0x44);
  st7789v_WriteData(0x4C);
  st7789v_WriteData(0x2B);
  st7789v_WriteData(0x16);
  st7789v_WriteData(0x15);
  st7789v_WriteData(0x1E);
  st7789v_WriteData(0x21);

  st7789v_WriteReg(0xe1);
  st7789v_WriteData(0xD0);
  st7789v_WriteData(0x03);
  st7789v_WriteData(0x08);
  st7789v_WriteData(0x0E);
  st7789v_WriteData(0x11);
  st7789v_WriteData(0x2B);
  st7789v_WriteData(0x3B);
  st7789v_WriteData(0x54);
  st7789v_WriteData(0x4C);
  st7789v_WriteData(0x2B);
  st7789v_WriteData(0x16);
  st7789v_WriteData(0x15);
  st7789v_WriteData(0x1E);
  st7789v_WriteData(0x21);

  st7789v_WriteReg(0x29);	//Display ON 
  
	
//	g_LCD = st7789v_ReadID();
//	
//	while (1);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval LCD Register Value.
  */
uint16_t st7789v_ReadID(void)
{
  LCD_IO_Init();
  return ((uint16_t)st7789v_ReadData(LCD_READ_ID4, LCD_READ_ID4_SIZE));
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void st7789v_DisplayOn(void)
{
  /* Display On */
  st7789v_WriteReg(LCD_DISPLAY_ON);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void st7789v_DisplayOff(void)
{
  /* Display Off */
  st7789v_WriteReg(LCD_DISPLAY_OFF);
}

/**
  * @brief  Writes  to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void st7789v_WriteReg(uint8_t LCD_Reg)
{
  LCD_IO_WriteReg(LCD_Reg);
}

/**
  * @brief  Writes data to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void st7789v_WriteData(uint8_t RegValue)
{
  LCD_IO_WriteData(RegValue);
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  RegValue: Address of the register to read
  * @param  ReadSize: Number of bytes to read
  * @retval LCD Register Value.
  */
uint32_t st7789v_ReadData(uint16_t RegValue, uint8_t ReadSize)
{
  /* Read a max of 4 bytes */
  return (LCD_IO_ReadData(RegValue, ReadSize));
}

/**
  * @brief  Get LCD PIXEL WIDTH.
  * @param  None
  * @retval LCD PIXEL WIDTH.
  */
uint16_t st7789v_GetLcdPixelWidth(void)
{
  /* Return LCD PIXEL WIDTH */
  return ST7789V_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get LCD PIXEL HEIGHT.
  * @param  None
  * @retval LCD PIXEL HEIGHT.
  */
uint16_t st7789v_GetLcdPixelHeight(void)
{
  /* Return LCD PIXEL HEIGHT */
  return ST7789V_LCD_PIXEL_HEIGHT;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
