﻿精简了官方库。





//#include "fsl_video_common.h"
    //#include "fsl_camera.h"
    //#include "fsl_camera_receiver.h"
    文件是必须的，当不必包含在system.h中


//========================================================================================== 
/*      更新说明                                                                    时间
 *      1.优化 Test_OLED_Camera() 
 *      2.增加systick计时器功能 
 *      3.优化延时函数                                                             2018/11/08
 *
 *      1.优化 PID
 *      2.优化 Scheduler                                                           2018/11/09
 *
 *      1.增加QTMR模块的正交解码例程                                               
 *      2.优化神眼摄像头SCCB                                                       2018/11/21
 *
 *      1.优化神眼摄像头帧率设置                                                   2018/11/27
 *
 *      1.优化7725摄像头帧率配置
 *      2.增加上位机显示图像功能                                                   2018/11/28
 *
 *      1.解决神眼188*120分辨率不出图问题
 *      2.解决图像抖动问题                                                         2019/01/15
 *
 *
 *      1.更新GPT定时器延时和计时功能                                              2019/02/18       
 */

2019/3/29       19:49
库移植完成
include.h移植到system.h
摄像头测试可用


2019/3/29       20:42
iic移植完成




2019/3/29       20:55
camera文件移植完成
测试可用

IAR无法跳转 
Tools->Options
Generate browse information打勾重选

Ctrl+T 格式化代码


2019/3/30   16:33
优化了pwm中的部分代码
测试可用


2019/3/30   19:10
pwm.c全部优化完成测试可用
优化encoder模块，通过mEncConfigStruct.enableReverseDirection = true;  
改变计数方向。
模块测试可用


2019/3/30   20:30
部分修改了摄像头。
测试还能用。。

2019/3/30   21:53
移植了新的iic程序
测试可用



2019/4/13   15:10
删掉了部分没用的官方例程代码
精简了驱动程序，摄像头测试可用





system:
  单片机内核层的函数，涉及到寄存器定义、时钟配置、启动文件等。
  
fsl_lib:
  厂级片上独立外设驱动
  
driver
  用户应用级设备初始化与数据获取
  
car
  数据处理与算法层



