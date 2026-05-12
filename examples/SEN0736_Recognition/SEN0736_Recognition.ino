/*!
 * @file SEN0736_Recognition.ino
 * @brief Continuous automatic user recognition routine on Raspberry Pi 2/3/4 (pyserial).
 * @details This routine periodically detects faces, automatically recognizes them
 * @n and prints user details via the serial port.
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
void              setup()
{
  // put your setup code here, to run once:
  ModuleSerial.begin(115200);    //Start ModuleSerial, communicate with module
  Serial.begin(115200);          //Start serial 1, for information printing
  delay(2000);                   //Wait for module to start
}

void loop()
{
  // put your main code here, to run repeatedly:
  while (face.check_state() == false) {
    Serial.println("Module not ready");
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
  delay(10000);
}
