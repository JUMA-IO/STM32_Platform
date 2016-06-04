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
#include "bsp_common.h"
#include "st7789v.h"

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

uint32_t g_LCD = 0;

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

