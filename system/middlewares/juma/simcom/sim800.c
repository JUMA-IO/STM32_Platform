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
#include "sim800.h"
#include "stm32f4xx_hal_uart.h"

#define DEBUG_LOG
#define UART_RX_INTERRUPT_EN

#define MAX_BUFF_LEN (128)  
#define AT_CMD_TIMEOUT (500)
#define AT_HTTP_TIMEOUT (200)

extern UART_HandleTypeDef Uart2Handle;
UART_HandleTypeDef * pUartHldr = NULL;
char mUartRcvBuff[MAX_BUFF_LEN];


static void clear_uart_rcv_buff(void)
{
    for(int i=0; i<MAX_BUFF_LEN; i++){
        mUartRcvBuff[i] = 0x00; //set to '\0'
    }
}

static int check_rsp_code(const char *code)
{ 
    if(strstr(mUartRcvBuff, code) != NULL)
        return SUCCESS;
    else
        return ERROR;
}

/*
 * Clear UART RX hardware registers.
 */
static int clear_uart_rcv_hw(int timeOut){
    uint16_t tmp;
        
    if(pUartHldr == NULL)
        return 0;

    while((__HAL_UART_GET_FLAG(pUartHldr, UART_FLAG_RXNE) != RESET) && (timeOut > 0)){

        /* just read to clear. */
        if(pUartHldr->Init.Parity == UART_PARITY_NONE)
        {
          tmp = (uint16_t)(pUartHldr->Instance->DR & (uint16_t)0x01FF);
        }
        else
        {
          tmp = (uint16_t)(pUartHldr->Instance->DR & (uint16_t)0x00FF);
        }

        HAL_Delay(1);
        timeOut--;
    }

    return timeOut;
}

static int sim800_setup(){
    /*
    * Routine setup for common usage.
    */

    // Fix baudrate to 115200 bps
    send_AT_cmd("AT+IPR=115200\r\n", NULL, AT_CMD_TIMEOUT);
    
    // Turn off playback
    send_AT_cmd("ATE0\r\n", NULL, AT_CMD_TIMEOUT);

    // Check SIM card
    if(ERROR == send_AT_cmd("AT+CPIN?\r\n", "+CPIN: READY\r\n\r\nOK", AT_CMD_TIMEOUT))
        return ERROR;

    // Check RSSI, should be 18+.
    //if(ERROR == send_AT_cmd("AT+CSQ\r\n", "+CSQ: 18,0", AT_CMD_TIMEOUT))
    //    return ERROR;

    // Check NETWORK registeration
    if(ERROR == send_AT_cmd("AT+CREG?\r\n", "+CREG: 0,1\r\n\r\nOK", AT_CMD_TIMEOUT))
        return ERROR;

    // Check GPRS
    if(ERROR == send_AT_cmd("AT+CGATT?\r\n", "+CGATT: 1\r\n\r\nOK", AT_CMD_TIMEOUT))
        return ERROR;

    /*
    * turn on GPRS.
    */    
    if(ERROR == send_AT_cmd("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n", "OK", AT_CMD_TIMEOUT))
        return ERROR;

#if 1 /* APN Setting for IOT SIM Card of CHINA MOBILE */
    if(ERROR == send_AT_cmd("AT+SAPBR=3,1,\"APN\",\"CMIOT\"\r\n", "OK", AT_CMD_TIMEOUT))
        return ERROR;
#else /* Normal SIM Card */
    if(ERROR == send_AT_cmd("AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n", "OK", AT_CMD_TIMEOUT))
        return ERROR;
#endif

    if(ERROR == send_AT_cmd("AT+SAPBR=1,1\r\n", "OK", 3000)){
        
        //ERROR, maybe it's because that it's already be opened, let's check:
        if(ERROR == send_AT_cmd("AT+SAPBR=2,1\r\n", "+SAPBR: 1,1", 2000))
            return ERROR;        
        else
            return SUCCESS;
        return ERROR;
    }

    //+SAPBR: 1,1, <IP addr>; just to get the IP address.
    if(ERROR == send_AT_cmd("AT+SAPBR=2,1\r\n", NULL, 2000))
        return ERROR;
    
    return SUCCESS;
}


/**
  * @Brief: initialize SIMCOM800 module
  * @Retval: SUCCESS or ERROR
  * @Note: UART2 should already be initailized in MSP.
  */
int sim800_init(){
    int reTry = 3;
    
    /*
    * UART2 should already be initailized in MSP.
    */
    if(Uart2Handle.Instance == NULL)
        return ERROR;

    pUartHldr = &Uart2Handle;

    /*
    * We may need some delay here, because SIM800 need  
    * seconds to be ready for work when powering up.
    */    

    while(ERROR == sim800_setup()){
#ifdef DEBUG_LOG
        printf("\r\n\r\n[sim800]: init setup error, retrying...\r\n");
#endif        
         // Term HTTP
        send_AT_cmd("AT+HTTPTERM\r\n", NULL, AT_CMD_TIMEOUT);
        
        // Turn off GPRS.
        send_AT_cmd("AT+SAPBR=0,1\r\n", NULL, 1000);

        // Reset the GSM module
        // TODO: should be AT+CFUN=1,1.???
        send_AT_cmd("AT+CFUN=1\r\n", NULL, 1000);

        HAL_Delay(5000); //try to delay some time.
        
        reTry--;
        if(reTry <= 0){
            printf("\r\n\r\n[sim800]: init setup failed!\r\n");
            break;
        }
    }

    if(reTry <= 0)
        return ERROR;

#ifdef DEBUG_LOG
    printf("\r\n\r\n[sim800]: init setup completed!\r\n");
#endif        

    return SUCCESS;
}

/**
  * @Brief: shut it down.
  * @Retval: SUCCESS or ERROR
  */

int sim800_deinit(){

    // Turn off GPRS.
    send_AT_cmd("AT+SAPBR=0,1\r\n", NULL, 1000);
    //send_AT_cmd("AT+CFUN=0\r\n", "OK", 1000);

}


/**
  * @Brief: sends out AT command through UART.
  * @Param: cmdBuff - the cmd data buff.
  * @Param: rspCode - expected response from SIM800, to check status.
  * @Param: timeOut - timeout in unit of miilisecond, typically it's 100~1000ms.
  * @Retval: SUCCESS or ERROR
  * @Note: This is executed in BLOCKING method.
  * @Note: if rspCode is NULL, means no check but just simple waiting.
  */
static int send_AT_cmd(const char *cmdBuff, const char* rspCode, int timeOut){

    HAL_StatusTypeDef status;
    int txLen, rxLen;
    uint32_t tickstart, tickDiff;
    
    if((pUartHldr == NULL) ||(cmdBuff == NULL)  )
        return ERROR;
    
    if(HAL_UART_GetState(pUartHldr) & HAL_UART_STATE_ERROR)
        return ERROR;

    if(timeOut <= 0)
        timeOut = 100;
    
    while((HAL_UART_GetState(pUartHldr) & HAL_UART_STATE_BUSY) && (timeOut > 0)){
        HAL_Delay(1);
        timeOut--;        
    }
    if(timeOut <= 0)
        return ERROR;

    txLen = strlen(cmdBuff);
    if(txLen > MAX_BUFF_LEN) /* too long data, doesn't seem right. */
        return ERROR;   

#ifdef DEBUG_LOG
    printf("[sim800 TX]: %s", cmdBuff);
#endif    

    /* clear uart rx buff from last time */
    timeOut = clear_uart_rcv_hw(timeOut);
    if(timeOut <= 0)
        return ERROR;


    tickstart = HAL_GetTick();
    status = HAL_UART_Transmit(pUartHldr, (uint8_t *)cmdBuff, txLen, timeOut); /* Blocking */
    if (status != HAL_OK)
        return ERROR;
    tickDiff = (HAL_GetTick()-tickstart); //overflow isn't an issue with uint. 
    timeOut -= tickDiff;
    if(timeOut <= 0)
        return ERROR;

    if(rspCode != NULL){
        rxLen = strlen(rspCode);
        /* Note: there will be an additionally "\r\n" before the response. */
        rxLen += 2;    

        if(rxLen > MAX_BUFF_LEN)    /* too long data, doesn't seem right. */
            return ERROR; 
    }
    else{
        rxLen = MAX_BUFF_LEN;       //a long data buff, will cause waiting until timeout, 
                                                 // in order to get all rx data.
    }
    
    clear_uart_rcv_buff();

    /* UART RX in Blocking way */
    status = HAL_UART_Receive(pUartHldr, (uint8_t *)mUartRcvBuff, (rxLen), timeOut); /* Blocking */
    if ( (status != HAL_OK) && (status != HAL_TIMEOUT) )
        return ERROR;

#ifdef DEBUG_LOG
        printf("[sim800 RX]: %s\r\n", mUartRcvBuff);
#endif    

    if(rspCode != NULL)
        return check_rsp_code(rspCode);
    else
        return SUCCESS;

}

/**
  * @Brief: test phone call.
  * @Param: phone - phone number string.
  * @Retval: SUCCESS or ERROR
  */

int sim800_test_phone_call(const char * phone){

    char  atBuffer[20] = {0};
    
    if(strlen(phone) != 11)
        return ERROR;

    sprintf(atBuffer, "ATD%s;\r\n", phone);
    
    if(SUCCESS == send_AT_cmd(atBuffer, "OK", 1000)){
        HAL_Delay(10000);

        send_AT_cmd("ATH\r\n", NULL, 1000);

        return SUCCESS;
    }
    else{
        return ERROR;
    }
}

/**
  * @Brief: post a HTTP request with JSON data.
  * @Param: did, steps, volt are the data content.
  * @Retval: SUCCESS or ERROR
  * Note: Same as:
  * curl -H "Content-Type: application/json" -X POST -d '{"did":"peter123", "steps":"56789", "volt":"99"}' http://117.121.38.191:3000/save_chicken    
*/

int sim800_http_post_chicken(const char * did, uint32_t steps, uint32_t volt){

    char atHttpDataCmd[32] = {0};
    char data[128] = {0};

    sprintf(data, "{\"did\":\"%s\",\"steps\":\"%u\",\"volt\":\"%u\"}\r\n", did, steps, volt);
    sprintf(atHttpDataCmd, "AT+HTTPDATA=%d,10000\r\n", strlen(data));

    if(ERROR == send_AT_cmd("AT+HTTPINIT\r\n", "OK", AT_HTTP_TIMEOUT)){
        if(ERROR == send_AT_cmd("AT+HTTPTERM\r\n", "OK", AT_HTTP_TIMEOUT)){
            return ERROR;
        }
        else{
            if(ERROR == send_AT_cmd("AT+HTTPINIT\r\n", "OK", AT_HTTP_TIMEOUT))
                return ERROR;
        }
    }

    if(ERROR == send_AT_cmd("AT+HTTPPARA=\"CID\",1\r\n", "OK", AT_HTTP_TIMEOUT))
        return ERROR;

    if(ERROR == send_AT_cmd("AT+HTTPPARA=\"URL\",\"http://117.121.38.191:3000/save_chicken\"\r\n", "OK", AT_HTTP_TIMEOUT))
        return ERROR;

    if(ERROR == send_AT_cmd("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n", "OK", AT_HTTP_TIMEOUT))
        return ERROR;

    if(ERROR == send_AT_cmd(atHttpDataCmd, "DOWNLOAD", AT_HTTP_TIMEOUT))
        return ERROR;

    if(ERROR == send_AT_cmd(data, "OK", AT_HTTP_TIMEOUT)) //check OK is meaningless.
        return ERROR;   

    if(ERROR == send_AT_cmd("AT+HTTPACTION=1\r\n", "OK", AT_HTTP_TIMEOUT))
        return ERROR;    

    HAL_Delay(2500);// 2~5 seconds depends on the network and data length.

    if(ERROR == send_AT_cmd("AT+HTTPTERM\r\n", "OK", AT_HTTP_TIMEOUT)){
        
        HAL_Delay(1000);
        if(ERROR == send_AT_cmd("AT+HTTPTERM\r\n", "OK", AT_HTTP_TIMEOUT)){

            HAL_Delay(1500);
            if(ERROR == send_AT_cmd("AT+HTTPTERM\r\n", "OK", AT_HTTP_TIMEOUT)){
                
                return ERROR;
            }        
        }
    }

    return SUCCESS;
}

