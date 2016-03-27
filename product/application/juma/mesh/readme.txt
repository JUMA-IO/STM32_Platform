mesh协议demo需要配置:
1.设备的MESH_ID(mesh.h)

2.广播名字用于区分不同设备(board_name)

    手机端通过发送16bit位的数据来控制不同MESH_ID的LED灯，设备端
将接收到的16bit数据按照与自己的MESH_ID按位与的方式来控制LED
例如：0x0001控制MESH_ID为0x0001LED灯亮，其它灯全灭。
0x000F控制MESH_ID为0x0001,0x0002,0x0004,0x0008的灯全亮，其它LED
全灭。