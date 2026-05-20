/*!
 * @file sen0737Recognition.ino
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

#define IR_PIN 19    //Configure the infrared pin,it is up to your mcu
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

  Serial.begin(115200);      //Start serial 1, for information printing
  pinMode(IR_PIN, INPUT);    //Configure the infrared pin as an input with a pull-down resistor.
  delay(200);                //Wait for module to start
}

uint8_t j = 0;
void    loop()
{
  // put your main code here, to run repeatedly:
  while (face.checkState() == false) {    //Determine whether the module is ready
    Serial.println("Module not ready !");
  }
  if (j++ < 1) {
    Serial.println("Module ready");
    Serial.println("------------------------------------------------");
  }
  //Read IR pin: high when object is present
  while (digitalRead(IR_PIN) == LOW) {
    delay(500);
  }
  DFRobot_Biometric::sId_t user   = { 0, DFRobot_Biometric::eFaceUser, DFRobot_Biometric::eRoleNormal, { 0 } };    //Store the recognized user information
  int8_t                   result = 0;
  result                          = face.getRecognitionResult(&user);
  //Determine the execution result
  if (result == 1) {
    Serial.println("Success! This is imformation of the user:");
    Serial.print("id:");
    Serial.println(user.id);
    Serial.print("userName:");
    Serial.println(user.userName);
    Serial.print("user kind:");
    if (user.kind == face.eFaceUser) {
      Serial.println("face user");
    } else if (user.kind == face.ePalmUser) {
      Serial.println("palm user");
    }
    Serial.print("userClass:");
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
  Serial.println("------------------------------------------------");
  Serial.println("");
  delay(2000);
}
