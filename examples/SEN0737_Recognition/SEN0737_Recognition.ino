/*!
 * @file SEN0737_Recognition.ino
 * @brief Infrared detection and automatic user recognition.
 * @details This routine automatically detects targets within infrared range.
 * @n When a target is detected, it initiates user recognition and prints the user's specific information via the serial port.
 * @copyright Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [Olive](feng.yang@dfrobot.com)
 * @version V1.0.0
 * @date 2026-04-30
 * @url https://github.com/DFRobot/DFRobot_Biometric
 */

#include "DFRobot_Biometric.h"

#if defined(ARDUINO_AVR_MEGA2560)
#define ModuleSerial Serial2
#elif defined(ARDUINO_AVR_LEONARDO)
#define ModuleSerial Serial1
#elif defined(ESP32)
#define ModuleSerial Serial2
#elif defined(__AVR__)
#define ModuleSerial Serial
#else
#error "This board is not supported"
#endif

DFRobot_Biometric face(ModuleSerial);    //Create class global object
#define IR_PIN 25                        //Configure the infrared pin
void setup()
{
  // put your setup code here, to run once:
  ModuleSerial.begin(115200);    //Start ModuleSerial, communicate with module
  Serial.begin(115200);          //Start serial 1, for information printing
  pinMode(IR_PIN, INPUT);        //Configure the infrared pin as an input with a pull-down resistor.
  delay(2000);                   //Wait for module to start
}

void loop()
{
  // put your main code here, to run repeatedly:
  while (face.check_state() == false) {
    Serial.println("Module not ready");
  }
  while (digitalRead(IR_PIN) == LOW) {
    delay(100);
  }
  DFRobot_Biometric::sId_t user   = { 0, 0, DFRobot_Biometric::eRoleNormal, { 0 } };
  int8_t                   result = 0;
  result                          = face.getRecognitionResult(&user);
  if (result == 1) {
    Serial.println("Success! the id,name,user_kind,user_class of user:");
    Serial.println(user.id);
    Serial.println(user.userName);
    if (user.kind == FACE_USER) {
      Serial.println("face user");
    } else if (user.kind == PALM_USER) {
      Serial.println("palm user");
    }
    if (user.isAdmin == face.eRoleNormal) {
      Serial.println("Normal user");
    } else if (user.isAdmin == face.eRoleAdmin) {
      Serial.println("Adminer");
    }
  } else if (result == 2) {
    Serial.println("time out");
  } else if (result == NO_ACK) {
    Serial.println("No response from module");
  } else if (result == 3) {
    Serial.println("not found");
  }
}
