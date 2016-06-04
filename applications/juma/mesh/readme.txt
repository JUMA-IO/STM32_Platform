mesh协议demo需要配置:
1.设备的MESH_ID(app.c的宏定义)

2.广播名字用于区分不同设备(board_name，app.c中)

3.demo用的四个设备的MESH_ID分别定义为0x0001,0x0002,0x0004,0x0008.
名字也需改成不同的以方便识别。

4.移动端的测试软件名字为light demo。在目录tools/juma中