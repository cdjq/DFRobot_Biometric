#include "DFRobot_Biometric.h"
DFRobot_Biometric face(Serial2);    //创建类的全局对象
void              setup()
{
  // put your setup code here, to run once:
  Serial2.begin(115200);    //开启串口2,与模块通信
  Serial.begin(115200);     //开启串口1，实现信息的打印
  delay(2000);              //等待模块启动

  while (face.begin() == false) {
    Serial.println("主模块未响应");
  }
  int8_t   result = 0;
  uint16_t id;
  result = face.enrollUser(FACE_USER, "Lebron James", &id, DFRobot_Biometric::eRoleNormal);
  if (result == 1) {
    Serial.print("注册成功,用户的id是:");
    Serial.println(result);
  } else if (result == 2) {
    Serial.println("人脸已注册");
  } else if (result == 3) {
    Serial.println("注册超时");
  } else if (result == NO_ACK) {
    Serial.println("模块未响应,出现问题");
  } else if (result == ERROR) {
    Serial.println("用户名太长");
  }
  delay(3000);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
