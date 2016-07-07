# STM32_Platform

This is the JUMA.IO implementation for STM32 embedded platform. It includes a comprehensive SDK to facilitate development work for BLE communication and typical Wireless Sensor Applications.

##Hardware Support


###Cannon 小钢炮

![](./documents/cannon_v2/cannon_front.png)

Cannon board consists of a powerful MCU, a BLE transmitter(BlueNRG-MS), and several MEMS sensors. All the software resources of this repository can run on it, Both hardware and software are open-sourced, making it an ideal platform for learning or developping BLE/IoT applications.

* Learn hardware specs at [here](http://www.juma.io/platform/cannon.html).
* Learn step-by-step tutorial at [here](http://www.juma.io/doc/zh/stm32_platform/cannon_first/).

> Cannon is FREE for developers right now! Apply for a Cannon at JUMA's [forum](http://www.52cannon.com/bbs/forum.php?mod=viewthread&tid=131&page=1&extra=#pid433).  
> 小钢炮对开发者免费推广，您可以从我们的[论坛]((http://www.52cannon.com/bbs/forum.php?mod=viewthread&tid=131&page=1&extra=#pid433)申请。

###SMM02 BLE module 低功耗蓝牙模块

SMM02 is a ultra low power BLE module integrating STM32L052 MCU and BlueNRG-MS. 

> The module is available at JUMA store [here](https://item.taobao.com/item.htm?spm=a1z10.5-c.w4002-11176521396.38.Wcwon6&id=531299933708).

###JUMA Developer Watch 小钢炮手表

JUMA developer watch integrates STM32F429, BlueNRG-MS, 6 aixs, gyroscope, moto, LCD, touch screen, Mems MIC, speaker etc. It has an external flash for 128M and 64M RAM. It is suitable for wearable developement and motion data analysis.

> Check out the watch [here](https://item.taobao.com/item.htm?spm=a1z10.5-c.w4002-11176521396.29.9s3TVH&id=531660250386).



##Quick Start

Environment: Windows7, Keil, Cannon board.

1. Choose Demo project
Learn demos under `./applications` folder, select and open one project(*.uvprojx) in Keil.

2. Compile   
Use build command to compile the project.

3. Download firmware  
Connect Cannon to computer via J-Link or ST-LINK. Download firmware to Cannon.

4. Work with BLE APP  
A debugger APP (`./tools/JumaBleHelper.apk`) is offered to facilitate BLE development. It can be installed and run on Android machine to transmit BLE raw data to/from Cannon.  
The iOS version can be downloaded in AppStore as well.

> You can get detailed tutorial at [here](http://www.juma.io/doc/zh/stm32_platform/cannon_first/).    
> 详细的小钢炮的上手指南文档请[戳我](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)，请仔细阅读。



##Directories
* `/applications` - examples to show how to communicate with phone via Bluetooth, retrieve data in the sensor etc. 
* `/product` - product base main line.
* `/system` - including drivers, HALs, middlewares, JUMA SDKs, etc.
* `/tools` - including some APP utilities which could be installed and run on Android mobiles. 
* `/document` - things like document, datasheet, manuals, etc.

> Learn software architecture at [here](http://www.juma.io/doc/zh/stm32_platform/cannon_software/).  
> 小钢炮代码学习请参阅[这里](http://www.juma.io/doc/zh/stm32_platform/cannon_software/)。

##Bugs & New Features
You can report a bug by firing a issue at [here](https://github.com/JUMA-IO/STM32_Platform/issues).   

You are also warmly appreciated if you fix bugs or submit new features for Cannon, that indeed is the spirit of Open Source.

> For code contributing, please follow `Fork + Pull` flow, detailed could be viewed at [here](http://www.juma.io/doc/zh/stm32_platform/cannon_software/#_5).  
> 欢迎为小钢炮提交代码，代码提交采用`Fork + Pull`的方式，详情参见[这里](http://www.juma.io/doc/zh/stm32_platform/cannon_software/#_5)。



##Further Resources
* Home/主页: [www.juma.io/cannon.html](http://www.juma.io/cannon.html)
* Contact/联系: [hello@jumacc.com](hello@jumacc.com)
* Forum/论坛: [52cannon.com/bbs](http://52cannon.com/bbs)
* Quick Start/上手指南: [www.juma.io/doc/zh/stm32_platform/cannon_first/](http://www.juma.io/doc/zh/stm32_platform/cannon_first/)
* JUMA online store/聚码在线商店[here](https://shop123943370.taobao.com/)
* QQ group/小钢炮QQ群: `271927055`
