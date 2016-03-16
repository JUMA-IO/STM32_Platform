# STM32_Platform

The STM32 platform integrates a MCU, a BLE transmitter(BlueNRG), and several sensors. All the chips are manufactured by STMicroelectronics.

This repository includes a comprehensive SDK, together with various software resources like drivers, middlewares, and application demos for further developments.


##Cannon
![](http://mydiscuz.b0.upaiyun.com/forum/201603/15/175436widgtvqjl48ujsv4.jpg)  

* Cannon is the very first development board we made for STM32 platform. 
* Cannon boards allow anyone to try out new ideas and to quickly create product prototypes.
* Learn hardware specs at [here](http://www.juma.io/platform/cannon.html).
* Learn step-by-step tutorial at [here](http://www.juma.io/doc/zh/stm32_platform/cannon_first/).
* Learn software API manuals at [here](http://www.juma.io/doc/zh/embedded_sdk.html).


> Cannon is FREE now! Get a Cannon board at JUMA's [Taobao shop](http://tao.bb/5Dh6u).  
> 小钢炮正处于免费推广期，您可以从我们的[淘宝店](http://tao.bb/5Dh6u)申请一块。


##Directories
* `/product` - several examples based on Cannon, including a Thermometer detector, a BLE echo appliction, a SensorTag, etc. 
* `/system` - including drivers, HALs, middlewares, JUMA SDKs, etc.
* `/tools` - including some APP utilities which could be installed and run on Android mobiles. 
* `/document` - things like document, datasheet, manuals, etc.


##Quick Start

1. Choose Demo project  
Learn demos under `/product` folder, select and open one project in Keil.  

2. Learn how to Compile   
Cannon board has two versions, select the right one in Keil compile option.  
Make sure all the header files are included.

3. Learn how to Download firmware  
Connect Cannon to computer via J-Link or ST-LINK.   
Download firmware to Cannon.

4. Work with BLE APP  
A debugger APP (`/tools/JumaBleHelper.apk`) is offered to facilitate BLE development. It can be installed and run on Android machine to transmit BLE raw data to/from Cannon.  
The iOS version can be downloaded in AppStore as well.

> You can get detailed tutorial at [here](http://www.juma.io/doc/zh/stm32_platform/cannon_first/).    
> 详细的小钢炮的上手指南文档请[戳我](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)，请仔细阅读。


##Further Resources
* Home/主页: [http://www.juma.io](http://www.juma.io)
* Contact/联系: [hello@jumacc.com](hello@jumacc.com)
* Forum/论坛: [juma.io/bbs](juma.io/bbs)
* Quick Start/上手指南: [http://www.juma.io/doc/zh/stm32_platform/cannon_first/](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)
* QQ group/小钢炮QQ群: `271927055`


