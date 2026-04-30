#include "DFRobot_Biometric.h"
#include "string.h"
DFRobot_Biometric face(Serial2);//创建类的全局对象
void setup() {
  // put your setup code here, to run once:
  Serial2.begin(115200); //开启串口2,与模块通信
  Serial.begin(115200);//开启串口1，实现信息的打印
  delay(2000);//等待模块启动
}

void loop() {
  // put your main code here, to run repeatedly:
  while(face.begin()==false){
    Serial.println("主模块未响应");
  }
  char data[20]={0};
  uint8_t i=0;
  int16_t nums=0,id=0; 
  while(Serial.available()){
    data[i++]=Serial.read();
  }
  data[i]='\0';
  if(strcmp(data,"AT+GETUSERS")==0){
    nums=face.getAllNumsFaceUserIDs();
    Serial.print("人脸用户的数量是:");
    Serial.println(nums);
    nums=face.getAllNumsPalmUserIDs();
    Serial.print("掌静脉用户的数量是:");
    Serial.println(nums);
    LEDcolor();
  }else if(strncmp(data,"AT+DELUSER=",11)==0){
    nums=face.deleteUser(id);
    Serial.println(nums);
    if(nums==1){
      Serial.println("删除成功");
    }else{
      Serial.println("删除失败");
    }
  }else if(strcmp(data,"AT+DELALLUSERS")==0){
    nums=face.deleteAllUser();
    if(nums==1){
      Serial.println("删除成功");
    }else{
      Serial.println("删除失败");
    }
  }
  delay(3000);
}