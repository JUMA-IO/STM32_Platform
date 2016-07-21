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
#include "sim800.h"


/**
* Demo how to communicate a SimCom with Cannon board. 
* connect UART2(Vc, Tx, Rx, Gnd) to SimCom (SimCon800 in this Demo).
* We will demo 2 things:
* - Make a phone call and hangup.
* - Send a HTTP request to a server. [Note1]
*
* Note1:
* Post JSON data to the server, same result as following curl cmd:
* curl -H "Content-Type: application/json" -X POST -d '{"did":"peter123", "steps":"56789", "volt":"99"}' http://117.121.38.191:3000/save_chicken    
*/
void on_ready(void)
{
    uint32_t start;

    printf("*****************************************\r\n");
    printf("**********SIMCOM TEST APPLICATION *******\r\n");
    printf("*****************************************\r\n");

    BSP_LED_Off(LED0);

    // Initialize SIMCOM800
    printf("\r\nInitialize SIMCOM800\r\n");
    if(SUCCESS == sim800_init()){
        
        // Test to make a phone call
        //printf("\r\nTest to make a phone call\r\n");    
        //sim800_test_phone_call("18621797323");

        // Test HTTP post JSON data
        printf("\r\nTest HTTP post JSON data\r\n");
        for(int i=0; i<10; i++){
            printf("========== No: %d ==========\r\n", i);
            
            int steps = rand() % 10000 + 50000;
            int volt = rand() % 100;
            
            start = HAL_GetTick();
            sim800_http_post_chicken("peter123", steps, volt);
            printf("===== diff = %d(ms) =====\r\n", (HAL_GetTick() - start));
        }

        // De-initialize SIMCOM800
        printf("\r\nDe-initialize SIMCOM800\r\n");    
        sim800_deinit();
        
    }

    printf("************** Application Done! ***********\r\n");    
    while(1);
}
