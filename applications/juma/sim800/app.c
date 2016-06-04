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
#include "app.h"
#include "bsp_common.h"
#include "bluenrg_sdk_api.h"
#include "stm32f4xx_hal_msp.h"


#define SIMCOM_OK		0
#define SIMCOM_FAIL	1

const char *AT_CALL_ME = "ATD18621797323;\n"; //"ATD+Tel No+;"
const char *AT_CALL_HANG = "ATH\n"; //hang-up the call

const char *AT_GPRS_On1 = "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n";
const char *AT_GPRS_On2 = "AT+SAPBR=3,1,\"APN\",\"CMNET\"\n";
const char *AT_GPRS_On3 = "AT+SAPBR=1,1\n";
const char *AT_GPRS_On4 = "AT+SAPBR=2,1\n";
const char *AT_GPRS_Off = "AT+SAPBR=0,1\n";

const char *AT_HTTP_1 = "AT+HTTPINIT\n";
const char *AT_HTTP_2 = "AT+HTTPPARA=\"CID\",1\n";
const char *AT_HTTP_3 = "AT+HTTPPARA=\"URL\",\"http://wsensor.applinzi.com/data/write\"\n";
const char *AT_HTTP_4 = "AT+HTTPPARA=\"CONTENT\",\"application/json\"\n";
const char *AT_HTTP_5 = "AT+HTTPDATA=200,10000\n";
const char *AT_HTTP_6 = "{\"SENSOR_ID\":\"0002-0002-0003-0006\",\"SENSOR_TIME\":\"Thu May 26 12:38:15 CST 2016\",\"SENSOR_LOCATION\":\"shanghai\",\"SENSOR_TYPE\":\"Temperature\", \"SENSOR_DATA\":\"19\"}\n";
const char *AT_HTTP_7 = "AT+HTTPACTION=1\n";
const char *AT_HTTP_8 = "AT+HTTPTERM\n";

char uart_response[100] = {0};


int check_simcom_status(const char *response, const int len){
	int i;
	
	for(i=0; i<len; i++){
		if(response[i] == 'O' && response[i+1] == 'K'){
			break;
		}
	}
	
	if(i < len){
		return SIMCOM_OK;
	}
	return SIMCOM_FAIL;
}

/**
*
* Demo how to communicate a SimCom with Cannon board. [Note1]
* connect UART2(Vc, Tx, Rx, Gnd) to SimCom (SimCon800 in this Demo).
* We will demo 2 things:
* - Make a phone call and hangup.
* - Send a HTTP request to a server. [Note2]
*
* Note1: 
* Refer to stm32f4xx_hal_msp.c for Uart configuration.
*
* Note2:
* Sever for webpage reading: http://wsensor.applinzi.com/data/read
* Sever for HTTP post with a json data: http://wsensor.applinzi.com/data/write
*
*/
void on_ready(void)
{
    int uart_status;

    /*
    Blew is the AT command and response for making a phone call.

    //1. call
        ATD18621797323;
        OK

    //2. hanp up
        ATH
        OK
    */

	//1. try AT command of TEL phone call
    Uart_Tx_String(AT_CALL_ME, strlen(AT_CALL_ME));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 20, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
					//some error happens!
		}
    }
		
	//2. hang up the TEL phone call after 10 secs
	HAL_Delay(10000); //Ring for 10 seconds
	Uart_Tx_String(AT_CALL_HANG, strlen(AT_CALL_ME));
	if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 20, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
					//some error happens!
		}
    }
	

    /*
    Blew is the AT command and response for HTTP communication.

    //1. Open GPRS
        AT+SAPBR=3,1,"Contype","GPRS"
        OK
        AT+SAPBR=3,1,"APN","CMNET"
        OK
        AT+SAPBR=1,1
        OK
        AT+SAPBR=2,1
        +SAPBR: 1,1,"10.86.233.134"

    //2. Post a JSON data to the sever
        AT+HTTPINIT
        OK
        AT+HTTPPARA="CID",1
        OK
        AT+HTTPPARA="URL","http://wsensor.applinzi.com/data/write"
        OK
        AT+HTTPPARA="CONTENT","application/json"
        OK
        AT+HTTPDATA=200,10000
        DOWNLOAD
        {"SENSOR_ID":"0002-0002-0003-0005","SENSOR_TIME":"Thu May 26 12:38:15 CST 2016","SENSOR_LOCATION":"shanghai","SENSOR_TYPE":"Temperature", "SENSOR_DATA":"25"}
        OK
        AT+HTTPACTION=1
        OK
        +HTTPACTION: 1,200,1944
        AT+HTTPTERM
        OK

    //3. Close GPRS
        AT+SAPBR=0,1
        OK
    */

    BSP_LED_Off(LED0);
    HAL_Delay(1000);

    Uart_Tx_String(AT_GPRS_On1, strlen(AT_GPRS_On1));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }        
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);

    Uart_Tx_String(AT_GPRS_On2, strlen(AT_GPRS_On2));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);

    Uart_Tx_String(AT_GPRS_On3, strlen(AT_GPRS_On3));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);

    Uart_Tx_String(AT_GPRS_On4, strlen(AT_GPRS_On4));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }     
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);

    Uart_Tx_String(AT_HTTP_1, strlen(AT_HTTP_1));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);

    Uart_Tx_String(AT_HTTP_2, strlen(AT_HTTP_2));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);

    Uart_Tx_String(AT_HTTP_3, strlen(AT_HTTP_3));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);

    Uart_Tx_String(AT_HTTP_4, strlen(AT_HTTP_4));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);

    Uart_Tx_String(AT_HTTP_5, strlen(AT_HTTP_5));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    // BSP_LED_On(LED0);
    // HAL_Delay(1000);
    // BSP_LED_Off(LED0);


    Uart_Tx_String(AT_HTTP_6, strlen(AT_HTTP_6));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    HAL_Delay(10000);
    BSP_LED_Off(LED0);


    Uart_Tx_String(AT_HTTP_7, strlen(AT_HTTP_7));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);


    Uart_Tx_String(AT_HTTP_8, strlen(AT_HTTP_8));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    HAL_Delay(1000);
    BSP_LED_Off(LED0);                   


    Uart_Tx_String(AT_GPRS_Off, strlen(AT_GPRS_Off));
    if(HAL_MSP_SUCCESS == Uart_Rx_String((char *)uart_response, 100, 1000) ){
        if(SIMCOM_OK != check_simcom_status(uart_response, sizeof(uart_response))){
                    //some error happens!
        }
    }    
    BSP_LED_On(LED0);
    // HAL_Delay(1000);
    // BSP_LED_Off(LED0);
}
