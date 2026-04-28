/*!
 * @file DFRobot_Sensor.cpp
 * @brief 定义DFRobot_Sensor 类的基础结构，基础方法的实现
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [Ouki](ouki.wang@dfrobot.com)
 * @version V1.0
 * @date 2019-07-13
 * @url https://github.com/DFRobot/DFRobot_Sensor
 */
#include "DFRobot_Biometric.h"

#include <Arduino.h>

DFRobot_Biometric::DFRobot_Biometric(Stream& serial) : _serial(serial)
{
  _serial = serial;
}

bool DFRobot_Biometric::begin(void)
{
  uint8_t  data[6]     = { 0xEF, 0xAA, 0x11, 0x00, 0x00, 0x11 };
  bool     state       = false;
  uint16_t bufLen      = 0;
  uint8_t  buffer[250] = { 0 };
  calculateChecksum(data, 6);
  writeCmd(data,6);
  state = waitForReply(buffer, bufLen, 1000);
  if (state == true) {
    if (buffer[7] == 0) {
      return true;
    }
  }
  return false;
}

void DFRobot_Biometric::calculateChecksum(uint8_t* data, uint8_t len)
{
  uint8_t checkSum = 0;
  for (uint8_t i = 2; i < len - 1; i++) {
    checkSum ^= data[i];
  }
  data[len - 1] = checkSum;
}

int16_t DFRobot_Biometric::getAllNumsFaceUserIDs(void)
{

  uint8_t  data[7]     = { 0xEF, 0xAA, 0x24, 0x00, 0x01, 0x01, 0x00 };
  bool     state       = false;
  uint16_t bufLen      = 0;
  int16_t  nums        = 0;
  uint8_t  buffer[250] = { 0 };
  calculateChecksum(data, 7);
  writeCmd(data,7);
  state = waitForReply(buffer, bufLen, 1000);
  if (state == true) {
    nums = buffer[7] * 256 + buffer[8];
    return nums;
  }
  return -1;
}

int16_t DFRobot_Biometric::getAllNumsPalmUserIDs(void)
{
  uint8_t  data[7]     = { 0xEF, 0xAA, 0x24, 0x00, 0x01, 0x02, 0x00 };
  bool     state       = false;
  uint16_t bufLen      = 0;
  int16_t  nums        = 0;
  uint8_t  buffer[250] = { 0 };
  calculateChecksum(data, 7);
  writeCmd(data,7);
  state = waitForReply(buffer, bufLen, 1000);
  if (state == true) {
    nums = buffer[7] * 256 + buffer[8];
    return nums;
  }
  return -1;
}

bool DFRobot_Biometric::waitForReply(uint8_t* buffer, uint16_t& bufLen, uint16_t timeout)
{
  uint16_t recmdLen  = 0;
  uint8_t  recData   = 0;
  uint32_t startTime = millis();
  while (millis() - startTime < timeout) {
    if (_serial.available()) {
      recData = _serial.read();
      if (bufLen < 3) {
        if (recData == 0xEF && bufLen == 0) {
          buffer[bufLen++] = recData;
        } else if (recData == 0xAA && bufLen == 1) {
          buffer[bufLen++] = recData;
        } else if (recData == 0x00 && bufLen == 2) {
          buffer[bufLen++] = recData;
        } else if (recData == 0x01 && bufLen == 2) {
          bufLen = 0;
        } else {
          bufLen = 0;
        }
      } else {
        if (bufLen == 3) {
          buffer[bufLen++] = recData;
        } else if (bufLen == 4) {
          buffer[bufLen++] = recData;
          recmdLen         = buffer[3] * 256 + buffer[4];
        } else if (bufLen > 4 && bufLen <= (4 + recmdLen)) {
          buffer[bufLen++] = recData;
        } else if (bufLen == (5 + recmdLen)) {
          calculateChecksum(buffer, bufLen + 1);
          if (recData == buffer[bufLen]) {
            serialEmpty();
            return true;
          }
        }
      }
    }
  }
  return false;
}

uint8_t DFRobot_Biometric::enrollUser(uint8_t kind)
{
  uint8_t  data[46] = { 0xEF, 0xAA, 0x26, 0x00, 0x28, 0x00 };
  uint16_t i = 0, bufLen = 0;
  bool     state       = false;
  uint8_t  buffer[250] = { 0 };
  for (i = 6; i < 6 + 32; i++) {
    data[i] = 0x00;
  }
  if (kind == 1) {
    data[38] = 0x00;
  } else if (kind == 2) {
    data[38] = 0xFD;
  }
  data[39] = 0x01;    //单帧录入
  data[40] = 0x00;    //0为人脸不可重复录入，但用户名可重复录入;1为可重复录入
  data[41] = 0x0A;    //等待时间
  data[42] = 0x00;
  data[43] = 0x00;
  data[44] = 0x00;
  calculateChecksum(data, 46);
  writeCmd(data,46);
  state = waitForReply(buffer, bufLen, 11000);
  if (state == true) {
    if (buffer[6] == 0x00) {
      return 1;
    } else if (buffer[6] == 0x0A) {
      return 2;
    } else if (buffer[6] == 0x0D) {
      return 3;
    }
  }
  return 0;
}

uint8_t DFRobot_Biometric::deleteUser(uint16_t id)    //能够成功删除指定用户，但删除不存在的用户不会报错
{

  uint8_t  data[9]     = { 0xEF, 0xAA };
  uint8_t  buffer[250] = { 0 };
  uint16_t bufLen      = 0;
  bool     state       = false;
  if (id <= 500) {
    data[2] = 0x20;    //人脸指令
    data[7] = 0x00;    //人脸
  } else if (id > 500) {
    data[2] = 0x80;    //掌静脉指令
    data[7] = 0x01;    //掌静脉
  }
  data[3] = 0x00;
  data[4] = 0x03;
  data[5] = id / 256;
  data[6] = id % 256;
  calculateChecksum(data, 9);
  writeCmd(data,9);
  state = waitForReply(buffer, bufLen, 11000);
  if (state == true) {
    if (buffer[6] == 0x00) {
      return 1;
    } else if (buffer[6] == 0x08) {
      return 2;
    }
  }
  return 0;
}

uint8_t DFRobot_Biometric::deleteAllUser(void)    //??选择范围删除时，人脸和掌静脉都被删除了？删除后得不到正确的指令，但都能成功删除
{
  uint8_t  data[11]    = { 0xEF, 0xAA, 0x21, 0x00, 0x05 };
  uint8_t  buffer[250] = { 0 };
  uint16_t bufLen      = 0;
  bool     state       = false;
  data[5]              = 0x00;
  data[6]              = 0x00;
  data[7]              = 0x00;
  data[8]              = 0x00;
  data[9]              = 0x00;
  calculateChecksum(data, 11);
  writeCmd(data,11);
  state = waitForReply(buffer, bufLen, 5000);
  if (state == true) {
    if (buffer[6] == 0x00) {
      return 1;
    } else if (buffer[6] == 0x05) {
      return 2;
    }
  }
  return 0;
}

void DFRobot_Biometric::getRecognitionResult(sId_t* ID)
{
  uint8_t  data[8]     = { 0xEF, 0xAA, 0x12, 0x00, 0x02, 0x00, 0x10, 0x00 };
  uint8_t  buffer[250] = { 0 };
  uint16_t bufLen      = 0;
  bool     state       = false;
  calculateChecksum(data, 8);
  writeCmd(data,8);
  state = waitForReply(buffer, bufLen, 5000);
  if (state == true) {
    if (buffer[6] == 0 && ((buffer[42] == 0xC8) || buffer[42] == 0xCC)) {
      ID->id    = buffer[7] * 256 + buffer[8];
      ID->admin = buffer[41];
    } else if (buffer[6] == 0 && buffer[42] == 0xFA) {
      ID->id    = buffer[7] * 256 + buffer[8];
      ID->admin = buffer[41];
    } else if (buffer[6] == 0x0D) {
      ID->id = -1;
    }
  }
}

uint8_t DFRobot_Biometric::LEDcolor(uint8_t color, uint8_t kind)
{  
  uint8_t  data[8]     = { 0xEF, 0xAA, 0x90, 0x00, 0x02, 0x00, 0x00, 0x00 };
  uint8_t  buffer[250] = { 0 };
  uint16_t bufLen      = 0;
  bool     state       = false;
  data[5]              = color;
  data[6]              = kind;
  calculateChecksum(data, 8);
  writeCmd(data,8);
  state = waitForReply(buffer, bufLen, 1500);
  if (state == true) {
    if (buffer[6] == 0x00) {
      return 1;
    }
  }
  return 0;
}

void DFRobot_Biometric::serialEmpty(void)
{
  while (_serial.available()) {
    _serial.read();
  }
}

void DFRobot_Biometric::writeCmd(uint8_t* data,uint8_t len)
{
  serialEmpty();
  _serial.write(data, len);
}
