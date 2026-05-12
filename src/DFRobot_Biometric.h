/*!
 * @file DFRobot_Biometric.h
 * @brief Define the infrastructure of DFRobot_Biometric class
 * @n     This is Biometric sensor that can be controlled through USART port.
 * @copyright	Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [Ouki](ouki.wang@dfrobot.com)
 * @version V1.0
 * @date 2026-04-30
 * @url https://github.com/DFRobot/DFRobot_Biometric
 */
#ifndef __DFROBOT_BIOMETRIC_H
#define __DFROBOT_BIOMETRIC_H
#include <Arduino.h>

#define CMD_BEGIN              { 0xEF, 0xAA, 0x11, 0x00, 0x00, 0x11 }                                  ///<Check status command
#define CMD_GET_FACE_USER_NUMS { 0xEF, 0xAA, 0x24, 0x00, 0x01, 0x01, 0x24 }                            ///<Get face user count command
#define CMD_GET_FALM_USER_NUMS { 0xEF, 0xAA, 0x24, 0x00, 0x01, 0x02, 0x27 }                            ///<Get palm user count command
#define CMD_DELETE_ALL_USER    { 0xEF, 0xAA, 0x21, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24 }    ///<Delete all users command
#define CMD_IDENTIFY_USER      { 0xEF, 0xAA, 0x12, 0x00, 0x02, 0x00, 0x0A, 0x1A }                      ///<Identify user command

#define COLOR_GREEN 0x00    ///<Green light
#define COLOR_RED   0x01    ///<Red light
#define COLOR_WHITE 0x02    ///<White light

#define LED_OFF 0x01    ///<Turn off LED
#define LED_ON  0x00    ///<Turn on LED

#define STATUS_STANDBY 0x00    ///<Standby status
#define STATUS_BUSY    0x01    ///<Busy status
#define STATUS_ERROR   0x02    ///<Error status

#define RESULT_OK         0x00    ///<Command executed successfully
#define RESULT_TIMEOUT    0x0D    ///<Command execution timeout
#define RESULT_REPEAT     0x0A    ///<Face already enrolled
#define RESULT_NOT_FOUND  0x08    ///<User not found
#define RESULT_UNKONW_ERR 0x05    ///<Unknown error occurred

#define FACE_USER 0x01    ///<Face user
#define PALM_USER 0x02    ///<Palm user

class DFRobot_Biometric {
public:
#define NO_ACK -1    ///<No response from module
#define ERROR  -2    ///<Parameter error

  /**
   * @enum eIsAdmin_t
   * @brief Whether the user is an administrator
   * @details Technical details from the datasheet
   * @note Note
   * @attention Attention
   */
  typedef enum {
    eRoleNormal = 0,
    eRoleAdmin  = 1,
  } eIsAdmin_t;

  /**
   * @struct sId_t
   * @brief Store information returned by recognition function
   * @details Technical details from the datasheet
   * @note Note
   * @attention Attention
   */
  typedef struct {
    int16_t    id;
    uint8_t    kind;
    eIsAdmin_t isAdmin;
    char       userName[33];
  } sId_t;

  /**
   * @fn DFRobot_Biometric
   * @brief Constructor
   * @details Constructor details
   * @param serial The serial port used for initialization
   * @return None
   */
  DFRobot_Biometric(Stream& serial);

  /**
   * @fn check_state
   * @brief Check if the module is ready and idle
   * @details Function details
   * @param None
   * @return bool type
   * @retval true Module is ready
   * @retval false Module is not ready
   * @note Use check_state() to check status before initialization or executing commands
   * @attention
   */
  bool check_state(void);

  /**
   * @fn enrollUser
   * @brief Enroll face or palm for recognition
   * @details Function details
   * @param kind FACE_USER for face enrollment, PALM_USER for palm enrollment
   * @param userName User name, length 1~32 characters
   * @return Task execution result
   * @retval NO_ACK -1 No response from module
   * @retval ERROR  -2 User parameter error, such as invalid user type or name length
   * @retval 1 Success
   * @retval 2 Duplicate
   * @retval 3 Enrollment timeout
   */
  int8_t enrollUser(uint8_t kind, const char* userName, uint16_t* id, eIsAdmin_t idAdmin);

  /**
   * @fn getAllNumsFaceUserIDs
   * @brief Get the number of face users
   * @details Function details
   * @return Number of face users
   * @retval NO_ACK -1 No response from module,
   */
  int16_t getAllNumsFaceUserIDs(void);

  /**
   * @fn getAllNumsPalmUserIDs
   * @brief Get the number of palm users
   * @details Function details
   * @return Number of palm users
   * @retval NO_ACK -1 No response from module
   */
  int16_t getAllNumsPalmUserIDs(void);

  /**
   * @fn getRecognitionResult
   * @brief Recognize the user
   * @details Function details
   * @param ID Store the recognized user information
   * @return Execution result
   * @retval NO_ACK -1 No response from module
   * @retval 1 Success
   * @retval 2 Timeout
   * @retval 3 User not found
   */
  int8_t getRecognitionResult(sId_t* ID);

  /**
   * @fn deleteUser
   * @brief Delete specified user
   * @details Function details
   * @param id User ID, range 1~500
   * @return Delete result
   * @retval NO_ACK -1 No response from module
   * @retval ERROR  -2 ID out of range 1~500
   * @retval 1 Delete success
   * @retval 2 User not found
   * @retval 3 Unknown error, suggest retry
   */
  int8_t deleteUser(uint16_t id);

  /**
   * @fn deleteAllUser
   * @brief Delete all users
   * @details Function details
   * @return Delete result
   * @retval NO_ACK -1 No response from module ,ERROR  -2 User parameter error
   * @retval 1 Delete success
   * @retval 2 Unknown error, suggest retry
   */
  int8_t deleteAllUser(void);

  /**
   * @fn ledColor
   * @brief Control LED status
   * @details Function details
   * @param color LED color: COLOR_GREEN green, COLOR_RED red, COLOR_WHITE white
   * @param kind LED_ON turn on, LED_OFF turn off
   * @return Execution result
   * @retval NO_ACK -1 No response from module
   * @retval ERROR -2 Invalid kind or color parameter
   * @retval 1 Success
   */
  int8_t ledColor(uint8_t color, uint8_t kind);

private:
  Stream& _serial;

  /**
   * @fn calculateChecksum
   * @brief Calculate checksum for command
   * @details Function details
   * @param data Command array pointer
   * @param len Command length
   */
  void calculateChecksum(uint8_t* data, uint8_t len);

  /**
   * @fn waitForReply
   * @brief Send command and wait for reply feedback
   * @details Function details
   * @param buffer Command array pointer
   * @param timeout Wait time
   * @return None
   */
  bool waitForReply(uint8_t* buffer, uint16_t timeout);

  /**
   * @fn serialEmpty
   * @brief Clear receive buffer before and after receiving feedback
   * @details Function details
   * @return None
   */
  void serialEmpty(void);

  /**
   * @fn writeCmd
   * @brief Send command
   * @details Function details
   * @param data Command array pointer
   * @param len Command length
   * @param buffer Feedback array pointer
   * @param outTime Timeout
   * @return None
   */
  bool writeCmd(uint8_t* data, uint8_t len, uint8_t* buffer, uint16_t outTime);
};

#endif
