# cola_os

#### 介绍
300行代码实现多任务管理的OS，在很多MCU开发中，功能很简单，实时性要求不强，如果使用RTOS显得太浪费，任务多了管理不当又很乱，所以才会想起做一个轮询的任务管理。简单好用！
CSDN:
https://blog.csdn.net/weixin_39939185/article/details/129987882

#### 软件架构
软件架构说明

├─app      //主函数main  
├─bsp      //项目工程  
│&nbsp;&nbsp;├─STM32F0xx //stm32f0项目工程  
│&nbsp;&nbsp;│&nbsp;&nbsp;  ├─drivers //驱动文件  
│&nbsp;&nbsp;│&nbsp;&nbsp;  ├─example //MDK工程  
│&nbsp;&nbsp;│&nbsp;&nbsp;  ├─Libraries //库文件  
├─doc       //说明文档  
└─os        //任务调度  

#### 使用说明
参考doc下说明文档

#### 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request

