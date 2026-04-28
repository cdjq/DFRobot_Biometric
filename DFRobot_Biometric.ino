#include "DFRobot_Biometric.h"
uint8_t len;
char rec_data;
uint8_t packet[50]={0xEF,0xAA,0x11,0x00,0x00,0x11};//EF AA 12 00 02 00 0A,0x25 
DFRobot_Biometric face(Serial2);//创建类的全局对象
void setup() {
  // put your setup code here, to run once:
  Serial2.begin(115200); //开启串口2,与模块通信
  Serial.begin(115200);//开启串口1，实现信息的打印
  delay(5000);//等待启动200ms,再执行命令，防止串口监视器不显示
  Serial.println("--- 模块测试启动 ---");
  //face.deleteUser(0x0002);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  uint8_t nums=0;
  bool a=false;
  //DFRobot_SEN073X::ID_t user={0,0};
  while(!(a=face.begin()))
  {
    Serial.println("--- 未就绪 ---");
  }
  Serial.println("--- 开始获取人脸信息 ---");
  nums=face.getAllNumsFaceUserIDs();
  Serial.println(nums,HEX);
  Serial.println("--- 开始获取掌静脉信息 ---");
  nums=face.getAllNumsPalmUserIDs();
  Serial.println(nums,HEX);
  //Serial.println("--- 开始录入人脸 ---");
  //nums=face.enrollUser(0);
  //Serial.println(nums,HEX);
  Serial.println("--- 开始录入掌静脉 ---");
  nums=face.enrollUser(0);
  Serial.println(nums,HEX);
  Serial.println("--- 结束获取信息 ---");
  delay(2000);
  
  
}