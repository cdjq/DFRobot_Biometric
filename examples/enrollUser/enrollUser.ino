/*!
 * @file enrollUser.ino
 * @brief Example of face or palm user enrollment
 * @details This example performs user enrollment every time the board is reset,
 * @n and prints the enrollment result through the serial port
 * @n Enrollment should work normally as long as the connection is correct
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

  Serial.println("--- Loop Start ---");
  while (face.check_state() == false) {
    Serial.println("Module not ready");
    delay(200);
  }
  ModuleSerial.begin(115200);
  int8_t   result = 0;
  uint16_t id;
  Serial.print("start:");
  result = face.enrollUser(FACE_USER, "Lebron James", &id, DFRobot_Biometric::eRoleNormal);
  if (result == 1) {
    Serial.print("Success!The id is:");
    Serial.println(id);
  } else if (result == 2) {
    Serial.println("User already exists");
  } else if (result == 3) {
    Serial.println("Enrollment timeout");
  } else if (result == NO_ACK) {
    Serial.println("No response from module");
  } else if (result == ERROR) {
    Serial.println("User name out of range");
  }
  delay(3000);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
