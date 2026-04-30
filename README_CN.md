# DFRobot_Biometric
- [English Version](./README.md)

数据手册上抄一下芯片的描述

这里写模块介绍，做到读完这段，就能对模块有初步了解，让客户懂的用这个模块能干什么（数据手册通常比较官方，这里你可以举例子，更场景化）<br>
这个模块的优点，告诉用户为什么要购买这个模块。一些关键术语，我们要在readme中有解释

这里需要显示拍照图片，可以一张图片，可以多张图片（不要用SVG图）

![产品效果图片](./resources/images/SEN0736.png)
![产品效果图片](./resources/images/SEN0737.png)


## 产品链接（链接到中文商城）

    SKU：产品名称

## 目录

  * [概述](#概述)
  * [库安装](#库安装)
  * [方法](#方法)
  * [兼容性](#兼容性)
  * [历史](#历史)
  * [创作者](#创作者)

## 概述

这里填写当前Arduino软件库完成了基础功能，特色功能

## 库安装

使用此库前，请首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。

## 方法

```C++
  /**
   * @fn begin
   * @brief 确认模块是否就绪,是否空闲
   * @details 函数细节描述(简单函数可以不需要)
   * @param None (无，可以不需要)
   * @return bool类型
   * @retval true 就绪
   * @retval false 未就绪
   * @note 初始化或则执行某项命令前可以用begin()函数检查状态
   * @attention 注意事项(没有可不需要)
   */
  bool begin(void);

  /**
   * @fn enrollUser
   * @brief 对人脸或掌静脉进行识别
   * @details 函数细节描述(简单函数可以不需要)
   * @param kind FACE_USER表示进行人脸识别，PALM_USER表示进行掌静脉识别
   * @param userName  用户名称,字符长度为1~32
   * @return 返回任务执行结果
   * @retval NO_ACK -1 主模块无反应
   * @retval ERROR  -2用户名字符串过长
   * @retval 1 表示成功
   * @retval 2 表示重复
   * @retval 3 表示录入超时
   */
  int8_t enrollUser(uint8_t kind,const char* userName,uint16_t* id,eIsAdmin_t idAdmin);

  /**
   * @fn getAllNumsFaceUserIDs
   * @brief 获取人脸用户的数量
   * @details 函数细节描述(简单函数可以不需要)
   * @return 人脸用户的数量
   * @retval  NO_ACK -1表示模块无反应
   */
  int16_t getAllNumsFaceUserIDs(void);

  /**
   * @fn getAllNumsPalmUserIDs
   * @brief 获取掌静脉用户的数量
   * @details 函数细节描述(简单函数可以不需要)
   * @return 掌静脉用户的数量
   * @retval NO_ACK -1 表示主模块无反应
   */
  int16_t getAllNumsPalmUserIDs(void);

  /**
   * @fn getRecognitionResult
   * @brief 对用户进行识别
   * @details 函数细节描述(简单函数可以不需要)
   * @param ID 存放识别到的用户信息
   * @return 执行结果
   * @retval NO_ACK -1 表示主模块无反应
   * @retval 1 执行成功
   * @retval 2 超时
   * @retval 3 无此用户
   */
  int8_t getRecognitionResult(sId_t* ID);

  /**
   * @fn deleteUser
   * @brief 删除指定用户
   * @details 函数细节描述(简单函数可以不需要)
   * @param  id 用户的id号，范围1~500
   * @return 删除结果
   * @retval NO_ACK -1 主模块无反应
   * @retval ERROR  -2  ID超出1~500的范围
   * @retval 1 删除成功
   * @retval 2 无指定用户
   * @retval 3 未知错误，建议再次删除
   */
  int8_t deleteUser(uint16_t id);

  /**
   * @fn deleteAllUser
   * @brief 删除所有用户
   * @details 函数细节描述(简单函数可以不需要)
   * @return 删除结果
   * @retval NO_ACK -1 主模块无反应
   * @retval 1 删除成功
   * @retval 2 未知错误，建议再次删除
   */
  int8_t deleteAllUser(void);

  /**
   * @fn LEDcolor
   * @brief 控制led的状态
   * @details 函数细节描述(简单函数可以不需要)
   * @param color 灯的颜色，COLOR_GREEN 绿灯，COLOR_RED 红灯，COLOR_WHITE 白灯
   * @param kind LED_ON 开灯，LED_OFF 关灯
   * @return 执行结果
   * @retval NO_ACK -1 表示主模块无反应
   * @retval ERROR -2 kind参数无效
   * @retval 1 执行成功
   */
  int8_t LEDcolor(uint8_t color, uint8_t kind);
```

## 兼容性

主板               | 通过  | 未通过   | 未测试   | 备注
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             |
Mega2560        |      √       |              |             |
Leonardo        |      √       |              |             |
ESP32           |      √       |              |             |
micro:bit        |      √       |              |             |


## 历史

- 2026/04/28 - 1.0.0 版本

## 创作者

Written by Olive-hy, 2026. (Welcome to our [website](https://www.dfrobot.com/))
