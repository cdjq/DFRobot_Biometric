/*!
 * @file DFRobot_Biometric.h
 * @brief 定义DFRobot_Biometric 类的基础结构
 * @details 这是一个人脸与掌静脉识别模块，通过串口和USB去发送命令
 * @copyright	Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [Ouki](ouki.wang@dfrobot.com)
 * @version V1.0
 * @date 2026-04-30
 * @url https://github.com/DFRobot/DFRobot_Biometric
 */
#ifndef __DFROBOT_BIOMETRIC_H
#define __DFROBOT_BIOMETRIC_H
#include <Arduino.h>


#define CMD_BEGIN                    { 0xEF, 0xAA, 0x11, 0x00, 0x00, 0x11 }                                ///<检测状态指令
#define CMD_GET_FACE_USER_NUMS       { 0xEF, 0xAA, 0x24, 0x00, 0x01, 0x01, 0x24 }                          ///<获取用户数量指令
#define CMD_GET_FALM_USER_NUMS       { 0xEF, 0xAA, 0x24, 0x00, 0x01, 0x02, 0x27 }                          ///<获取用户数量指令
#define CMD_DELETE_ALL_USER          { 0xEF, 0xAA, 0x21, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24}   ///<删除所有用户指令
#define CMD_IDENTIFY_USER            { 0xEF, 0xAA, 0x12, 0x00, 0x02, 0x00, 0x0A, 0x1A}                     ///<识别用户指令

#define COLOR_GREEN             0x00     ///<绿灯
#define COLOR_RED               0x01     ///<红灯
#define COLOR_WHITE             0x02     ///<白灯

#define LED_OFF                 0x01     ///<关灯
#define LED_ON                  0x00     ///<开灯

#define STATUS_STANDBY          0x00     ///<空闲状态
#define STATUS_BUSY             0x01     ///<繁忙状态
#define STATUS_ERROR            0x02     ///<出现错误

#define RESULT_OK               0x00     ///<命令执行成功
#define RESULT_TIMEOUT          0x0D     ///<命令执行超时
#define RESULT_REPEAT           0x0A     ///<已有人脸重复录入
#define RESULT_NOT_FOUND        0x08     ///<用户不存在
#define RESULT_UNKONW_ERR       0x05     ///<命令执行出现未知错误

#define FACE_USER               0x01     ///<人脸用户
#define PALM_USER               0x02     ///<掌静脉用户




class DFRobot_Biometric {
public:
  #define NO_ACK -1   ///<模块无反应
  #define ERROR  -2   ///<参数错误

  /**
   * @enum eIsAdmin_t
   * @brief 是否是管理员用户
   * @details 提示一些必要的技术细节(从数据手册上抄写)
   * @note 注解
   * @attention 注意事项
   */
  typedef enum{
    ROLE_NORMAL=0,
    ROLE_ADMIN=1,
  }eIsAdmin_t;

  /**
   * @struct sId_t
   * @brief 用于存储识别函数返回的信息
   * @details 提示一些必要的技术细节(从数据手册上抄写)
   * @note 注解
   * @attention 
   */
  typedef struct {
    int16_t id;
    uint8_t kind;
    eIsAdmin_t isAdmin;
    char userName[33];
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
   * @param timeout 等待时间
   * @return None
   */
  bool waitForReply(uint8_t* buffer,  uint16_t timeout);

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
   * @param buffer 存放模块反馈指令的数组首地址
   * @param outTime 超时时间
   * @return None
   */
  bool writeCmd(uint8_t* data,uint8_t len,uint8_t*buffer,uint16_t outTime);
};

#endif
