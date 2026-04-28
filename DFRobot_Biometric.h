/*!
 * @file DFRobot_Biometric.h
 * @brief 定义DFRobot_Biometric 类的基础结构
 * @details 这是一个人脸与掌静脉识别模块，通过串口和USB去发送命令
 * @n 向寄存器0里写入数据，点亮不同颜色的LED灯
 * @n 从寄存器2里读出数据，高四位表示光线强度，低四位表示声音强度
 * @n 从寄存器3 bit0里写入数据，写1表示正常模式，写0表示低功耗模式
 * @n 从寄存器4 读取数据，读到的是芯片版本0xDF
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [Ouki](ouki.wang@dfrobot.com)
 * @version V1.0
 * @date 2026-04-27
 * @url https://github.com/DFRobot/DFRobot_Biometric
 */
#ifndef __DFROBOT_BIOMETRIC_H
#define __DFROBOT_BIOMETRIC_H
#include <Arduino.h>
class DFRobot_Biometric {
public:
  /**
   * @struct sId_t
   * @brief 用于存储识别函数返回的信息
   * @details 提示一些必要的技术细节(从数据手册上抄写)
   * @note 注解
   * @attention 当id<0时，表示识别失败
   */
  typedef struct {
    int16_t id;
    uint8_t admin;
  } sId_t;

  /**
   * @fn DFRobot_Biometric
   * @brief 构造函数
   * @details 函数细节描述(简单函数可以不需要)
   * @param serial 定义对象时，传进来初始化的一个串口
   * @return None
   */
  DFRobot_Biometric(Stream& serial);

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
   * @param kind 0表示进行人脸识别，1表示进行掌静脉识别
   * @return 返回任务执行结果
   * @retval 0 未作出回应
   * @retval 1 表示成功
   * @retval 2 表示重复
   * @retval 3 表示录入超时
   */
  uint8_t enrollUser(uint8_t kind);

  /**
   * @fn getAllNumsFaceUserIDs
   * @brief 获取人脸用户的数量
   * @details 函数细节描述(简单函数可以不需要)
   * @return 人脸用户的数量
   * @retval -1表示获取失败
   */
  int16_t getAllNumsFaceUserIDs(void);

  /**
   * @fn getAllNumsPalmUserIDs
   * @brief 获取掌静脉用户的数量
   * @details 函数细节描述(简单函数可以不需要)
   * @return 掌静脉用户的数量
   * @retval -1表示获取失败
   */
  int16_t getAllNumsPalmUserIDs(void);

  /**
   * @fn getRecognitionResult
   * @brief 对用户进行识别
   * @details 函数细节描述(简单函数可以不需要)
   * @return none
   */
  void getRecognitionResult(sId_t* ID);

  /**
   * @fn deleteUser
   * @brief 删除指定用户
   * @details 函数细节描述(简单函数可以不需要)
   * @return 删除结果
   * @retval 0 删除失败
   * @retval 1 删除成功
   * @retval 2 无指定用户
   */
  uint8_t deleteUser(uint16_t id);

  /**
   * @fn deleteAllUser
   * @brief 删除所有用户
   * @details 函数细节描述(简单函数可以不需要)
   * @return 删除结果
   * @retval 0 删除失败
   * @retval 1 成功删除
   * @retval 2 未知错误，建议再次删除
   */
  uint8_t deleteAllUser(void);

  /**
   * @fn LEDcolor
   * @brief 控制led的状态
   * @details 函数细节描述(简单函数可以不需要)
   * @param color 灯的颜色，0 绿灯，1 红灯，2 白灯
   * @param kind 0 开灯，1 关灯
   * @return 执行结果
   * @retval 0执行失败
   * @retval 1 执行成功
   */
  uint8_t LEDcolor(uint8_t color, uint8_t kind);

private:
  Stream& _serial;

  /**
   * @fn calculateChecksum
   * @brief 计算命令的校验值
   * @details 函数细节描述(简单函数可以不需要)
   * @param data 存放命令的数组首地址
   * @param len 命令长度
   */
  void calculateChecksum(uint8_t* data, uint8_t len);

  /**
   * @fn waitForReply
   * @brief 发送命令后，等待读取reply反馈指令
   * @details 函数细节描述(简单函数可以不需要)
   * @param buffer 存放命令的数组首地址
   * @param bufLen 数组索引
   * @param timeout 等待时间
   * @return None
   */
  bool waitForReply(uint8_t* buffer, uint16_t& bufLen, uint16_t timeout);

  /**
   * @fn serialEmpty
   * @brief 接收反馈前后，清除接收缓冲区
   * @details 函数细节描述(简单函数可以不需要)
   * @return None
   */
  void serialEmpty(void);

  /**
   * @fn writeCmd
   * @brief 发送命令
   * @details 函数细节描述(简单函数可以不需要)
   * @param data 存放命令的数组首地址
   * @param len 命令长度
   * @return None
   */
  void writeCmd(uint8_t* data,uint8_t len);
};

#endif
