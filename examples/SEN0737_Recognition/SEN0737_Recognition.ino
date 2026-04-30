#include "DFRobot_Biometric.h"
DFRobot_Biometric face(Serial2);//创建类的全局对象
#define IR_PIN   10//设置红外引脚
void setup() {
  // put your setup code here, to run once:
  Serial2.begin(115200); //开启串口2,与模块通信
  Serial.begin(115200);//开启串口1，实现信息的打印
  pinMode(IR_PIN,INPUT_PULLDOWN);//设置未下拉输入，红外引脚高电平时做出反馈
  delay(2000);//等待模块启动
}

void loop() {
  // put your main code here, to run repeatedly:
  while(face.begin()==false){
    Serial.println("主模块未响应");
  }
  while(digitalRead(IR_PIN)==LOW){
    delay(500);
  }
  DFRobot_Biometric::sId_t user={0,0,DFRobot_Biometric::ROLE_NORMAL,0};
  int8_t result=0;
  result=face.getRecognitionResult(&user);
  if(result==1){
    Serial.println("获取成功,id,用户名,用户类型,用户级别信息如下:");
    Serial.println(user.id);
    Serial.println(user.userName);
    if(user.kind==FACE_USER){
      Serial.println("人脸用户");
    }else if(user.kind==PALM_USER){
      Serial.println("掌静脉用户");
    }
    Serial.println(user.isAdmin);
  }else if(result==2){
    Serial.println("超时");
  }else if(result==NO_ACK){
    Serial.println("模块未响应");
  }else if(result==3){
    Serial.println("用户不存在");
  }
  delay(10000);
}