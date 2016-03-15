# STM32_Platform

The STM32 platform integrates a MCU, a BLE transmitter(BlueNRG), and several sensors. All the chips are manufactured by STMicroelectronics.

This repository includes a comprehensive SDK, together with various software resources like drivers, middlewares, and application demos for further developments.


##Cannon
![](http://mydiscuz.b0.upaiyun.com/forum/201603/15/175436widgtvqjl48ujsv4.jpg)  

* Cannon is the very first development board we made for STM32 platform. 
* Cannon boards allow anyone to try out new ideas and to quickly create product prototypes.
* Learn hardware specs at: [http://www.juma.io/platform/cannon.html](http://www.juma.io/platform/cannon.html)
* Learn step-by-step tutorial at: [http://www.juma.io/doc/zh/stm32_platform/cannon_first/](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)
* Learn software API manuals at: [http://www.juma.io/doc/zh/embedded_sdk.html](http://www.juma.io/doc/zh/embedded_sdk.html)


> Cannon is FREE now! Get a Cannon board at JUMA's Taobao shop: [http://tao.bb/5Dh6u](http://tao.bb/5Dh6u)  
> 小钢炮正处于免费推广期，您可以从我们的[淘宝店](http://tao.bb/5Dh6u)申请一块。


##Directories
* `/product` - several examples based on Cannon, including a Thermometer detector, a BLE echo appliction, a SensorTag, etc. 
* `/system` - including drivers, HALs, middlewares, JUMA SDKs, etc.
* `/tools` - including some APP utilities which could be installed and run on Android mobiles. 
* `/document` - things like document, datasheet, manuals, etc.


##Quick Start
> You can get detailed tutorial at:   
> [http://www.juma.io/doc/zh/stm32_platform/cannon_first/](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)

1. Choose seed project  
Learn examples under `/product` folder, select one to be the seed project.  
Copy & paste it to create your own project.

2. Header files  
Remember to include necessary header files.  
For BLE, it is: `/system/juma/inc/bluenrg_sdk_api.h`  
For sensors, it is: `/system/juma/inc/juma_sensor.h`

3. Make your own application  
Modify the seed project, change the logics, flows, algorithms, etc.  
Make it fit to your own application.

4. Compile firmware  
Cannon board has two versions, select the right one in Keil compile option.

5. Download firmware  
Connect Cannon to computer via J-Link or ST-LINK.   
Keil can download firmware to Cannon.

6. Work with BLE APP  
To facilitate BLE development, we offered a BLE debugger APP in `/tools` folder.   
It can be run on Android mobile to transmit BLE raw data to/from Cannon.


##Further Resources
* Home: [http://www.juma.io](http://www.juma.io)
* Contact: [hello@jumacc.com](hello@jumacc.com)
* Forum: [juma.io/bbs](juma.io/bbs)
* Quick Start/上手指南: [http://www.juma.io/doc/zh/stm32_platform/cannon_first/](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)
* QQ discuzz group/小钢炮QQ群: 271927055


