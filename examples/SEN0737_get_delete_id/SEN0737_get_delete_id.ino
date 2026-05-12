/*!
 * @file SEN0737_get_delete_id.ino
 * @brief This routine implements user lookup and deletion
 * @details This routine sends string commands over serial to the host controller
 * @n which parses and executes the corresponding actions on the module.
 * @n Supported commands: AT+GETUSERS (get number of users), AT+DELUSER=<ID> (delete user by ID), and AT+DELALLUSERS (delete all users).
 * @n After each command is successfully executed, the indicator light will display different colors corresponding to the command result.
 * @copyright Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [Olive](feng.yang@dfrobot.com)
 * @version V1.0.0
 * @date 2026-04-30
 * @url https://github.com/DFRobot/DFRobot_Biometric
 */

#include "DFRobot_Biometric.h"
#include "string.h"

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
  char    data[20] = { 0 };
  uint8_t i        = 0;
  int16_t nums = 0, id = 0;
  while (Serial.available()) {
    data[i++] = Serial.read();
  }
  data[i] = '\0';
  if (strcmp(data, "AT+GETUSERS") == 0) {
    nums = face.getAllNumsFaceUserIDs();
    Serial.print("the face ids id:");
    Serial.println(nums);
    nums = face.getAllNumsPalmUserIDs();
    Serial.print("the palm ids id:");
    Serial.println(nums);
    face.ledColor(COLOR_WHITE, LED_OFF);
    face.ledColor(COLOR_RED, LED_OFF);
    face.ledColor(COLOR_GREEN, LED_ON);
  } else if (strncmp(data, "AT+DELUSER=", 11) == 0) {
    id   = atoi(&data[11]);
    nums = face.deleteUser(id);
    Serial.println(nums);
    if (nums == 1) {
      Serial.println("Success");
      face.ledColor(COLOR_RED, LED_OFF);
      face.ledColor(COLOR_GREEN, LED_OFF);
      face.ledColor(COLOR_WHITE, LED_ON);
    } else {
      Serial.println("Fail");
    }
  } else if (strcmp(data, "AT+DELALLUSERS") == 0) {
    nums = face.deleteAllUser();
    if (nums == 1) {
      Serial.println("Success");
      face.ledColor(COLOR_GREEN, LED_OFF);
      face.ledColor(COLOR_WHITE, LED_OFF);
      face.ledColor(COLOR_RED, LED_OFF);
    } else {
      Serial.println("Fail");
    }
  }
  delay(3000);
}
