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

__weak void loop(void){};

int main(void)
{
		bsp_init();
    dispatch_init();
    on_ready();
    while(1)
  	 {
        HCI_Process();
        dispatch();
			  loop();
    }
}

