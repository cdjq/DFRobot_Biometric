/*!
 * @file sen0737GetDeleteId.ino
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

/* ---------------------------------------------------------------------------------------------------------
  *    board   |             MCU                |       Mega2560    |    UNO    |   Leonardo   |   ESP32   |
  *     VCC    |            3.3V/5V             |        VCC        |    VCC    |      VCC     |    VCC    |
  *     GND    |              GND               |        GND        |    GND    |      GND     |    GND    |
  *     RX     |              TX                |     Serial2 TX1   |     11    |  Serial1 TX1 |     17    |
  *     TX     |              RX                |     Serial2 RX1   |     10    |  Serial1 RX1 |     16    |
  * -------------------------------------------------------------------------------------------------------*/
/* Baud rate cannot be changed , it is 115200 */

#if defined(ARDUINO_AVR_UNO)
#include <SoftwareSerial.h>
SoftwareSerial    moduleSerial(10, 11);
DFRobot_Biometric face(moduleSerial);

#elif defined(ESP32)
#define RX_PIN 16
#define TX_PIN 17
DFRobot_Biometric face(Serial2);

#elif defined(ARDUINO_AVR_MEGA2560)
// RX 17,TX 16
DFRobot_Biometric face(Serial2);

#elif defined(ARDUINO_AVR_LEONARDO)
// RX 0,TX 1
DFRobot_Biometric face(Serial1);

#else
#error "This board is not supported"
#endif

void setup()
{
#if defined(ESP32)
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
#else
#if defined(ARDUINO_AVR_LEONARDO)
  Serial1.begin(115200);
#elif defined(ARDUINO_AVR_MEGA2560)
  Serial2.begin(115200);
#elif defined(__AVR__)
  moduleSerial.begin(115200);
#endif
#endif

  Serial.begin(115200);    //Start serial 1, for information printing
  delay(200);              //Wait for module to start
}
uint8_t j = 0;
void    loop()
{
  // put your main code here, to run repeatedly:
  while (face.checkState() == false) {    //Determine whether the module is ready
    Serial.println("Module not ready !");
  }
  if (j++ < 1) {
    Serial.println("Module ready !");
    Serial.println("-----------------------------");
    //Command Introduction
    Serial.println("Available commands:");
    Serial.println("AT+GETUSERS       # Get all user information");
    Serial.println("AT+DELUSER=ID     # Delete the specified user (e.g. AT+DELUSER=0x1001)");
    Serial.println("AT+DELALLUSERS    # Delete all users");
    Serial.println("-----------------------------");
  }
  char    data[20] = { 0 };
  int16_t nums = 0, id = 0;
  while (!Serial.available()) {
    ;
  }
  //Read the user's command
  String input = Serial.readStringUntil('\n');
  input.trim();
  input.toCharArray(data, 30);
  //Match the command of user
  if (strcmp(data, "AT+GETUSERS") == 0) {
    nums = face.getAllNumsFaceUserIDs();
    Serial.print("the nums of face id:");
    Serial.println(nums);
    delay(100);
    uint16_t id[50];
    face.getAllFaceUserIDs(id, 50);
    if (nums > 0) {
      Serial.println("the specific information of face id:");
      for (int16_t i = 0; i < nums; i++) {
        Serial.print("user");
        Serial.print(i + 1);
        Serial.print(":");
        Serial.println(id[i]);
      }
      delay(100);
    }
    nums = face.getAllNumsPalmUserIDs();
    Serial.print("the nums of palm id:");
    Serial.println(nums);
    face.ledColor(COLOR_WHITE, LED_OFF);
    face.ledColor(COLOR_RED, LED_OFF);
    face.ledColor(COLOR_GREEN, LED_ON);
  } else if (strncmp(data, "AT+DELUSER=", 11) == 0) {
    id   = atoi(&data[11]);
    nums = face.deleteUser(id);
    if (nums == 1) {
      Serial.print("Successful delete user:");
      Serial.println(id);
      face.ledColor(COLOR_RED, LED_OFF);
      face.ledColor(COLOR_GREEN, LED_OFF);
      face.ledColor(COLOR_WHITE, LED_ON);
    } else {
      Serial.println("Fail");
    }
  } else if (strcmp(data, "AT+DELALLUSERS") == 0) {
    nums = face.deleteAllUser();
    if (nums == 1) {
      Serial.println("Successful delete all");
      face.ledColor(COLOR_GREEN, LED_OFF);
      face.ledColor(COLOR_WHITE, LED_OFF);
      face.ledColor(COLOR_RED, LED_OFF);
    } else {
      Serial.println("Fail");
    }
  }
  Serial.println("");
  Serial.println("please continue:");
  delay(3000);
}
