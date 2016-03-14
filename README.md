# STM32_Platform

##Introduction
The STM32 platform integrates a MCU, a BLE transmitter(BlueNRG), and several sensors manufactured by STMicroelectronics.

This repository includes a comprehensive SDK together with various software resources like drivers, middlewares, examples for STM32 platform development.


##Cannon
* Cannon is one of the development boards belongs to STM32 platform. 
* Cannon boards allow anyone to try out new ideas and to quickly create prototypes.
* Learn hardware specs at: [http://www.juma.io/platform/cannon.html](http://www.juma.io/platform/cannon.html)
* Learn step-by-step tutorial at: [http://www.juma.io/doc/zh/stm32_platform/cannon_first/](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)
* Learn software API manuals at: [http://www.juma.io/doc/zh/embedded_sdk.html](http://www.juma.io/doc/zh/embedded_sdk.html)
* Get a Cannon board at JUMA's Taobao shop: [http://tao.bb/5Dh6u](http://tao.bb/5Dh6u)

##Directories
* /product - several examples based on Cannon, including a Thermometer detector, a BLE echo appliction, a SensorTag, etc. 
* /system - including drivers, HALs, middlewares, JUMA SDKs, etc.
* /tool - including some APP utilities which could be installed and run on Android mobiles. 
* /document - things like document, datasheet, manuals, etc.


##Quick Start
> You can get detail tutorial at:   
> [http://www.juma.io/doc/zh/stm32_platform/cannon_first/](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)

1. Choose seed project  
Learn examples under */product* folder, select one to be the seed project.  
Copy & paste it to create your own project.

2. Header files  
Remember to include necessary header files.  
For BLE, it is: */system/juma/inc/bluenrg_sdk_api.h*  
For sensors, it is: */system/juma/inc/juma_sensor.h*

3. Make your own application  
Modify the seed project, change the logics, flows, algorithms, etc.  
Make it fit to your own application.

4. Compile firmware  
Cannon board has two versions, select the right one in Keil compile option.

5. Download firmware  
Connect Cannon to computer via J-Link or ST-LINK.   
Keil can download firmware to Cannon.

6. Work with BLE APP  
To facilitate BLE development, we offered a BLE debugger APP in */tool* folder.   
It can be run on Android mobile to transmit BLE raw data to/from Cannon.


##Further Resources
* Home: [http://www.juma.io](http://www.juma.io)
* Contact: [hello@jumacc.com](hello@jumacc.com)
* Forum: [juma.io/bbs](juma.io/bbs)
* Quick Start/上手指南: [http://www.juma.io/doc/zh/stm32_platform/cannon_first/](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)
* QQ discuzz group/小钢炮QQ群: 271927055


