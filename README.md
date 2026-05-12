# DFRobot_Biometric
- [中文版](./README_CN.md)

The FP001 and FP002 are face and palm vein recognition modules equipped with a bionic face recognition algorithm. They support UVC/UAC transmission and MJPEG video format, achieving both security and a good user experience. Featuring low power consumption, low cost, fast recognition, exquisite appearance, and compact structure, these modules utilize high-performance chips to quickly complete the entire process of face recognition and palm vein recognition.

The Biometric library provides a unified encapsulation of the functions for these two modules. Compared with FP001, FP002 adds infrared detection and a controllable tri-color indicator light. It offers two user registration methods (face and palm vein) with high recognition efficiency, fast and accurate response. The library also includes an administrator user function. The recognition algorithms are executed within the module itself; this library primarily handles command interaction – sending commands, receiving feedback – and ensures timely command response.




![Product Image](./resources/images/SEN0736.png)
![Product Image](./resources/images/SEN0737.png)


## Product Link (Link to DFRobot store)
    SKU: SEN0736 FP001 and SEN0737 FP002 Face and Palm Vein Recognition Modules

## Table of Contents

  * [Summary](#summary)
  * [Installation](#installation)
  * [Methods](#methods)
  * [Compatibility](#compatibility)
  * [History](#history)
  * [Credits](#credits)

## Summary

Introduce the basic and special functions of this Arduino Library.

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @fn begin
   * @brief Check whether the module is ready and idle
   * @details Detailed description (optional for simple functions)
   * @param None
   * @return bool type
   * @retval true ready
   * @retval false not ready
   * @note Can be used before initialization or command execution to check status
   * @attention (optional)
   */
  bool begin(void);

  /**
   * @fn enrollUser
   * @brief Enroll a face or palm vein user
   * @details Detailed description (optional)
   * @param kind FACE_USER for face enrollment, PALM_USER for palm vein enrollment
   * @param userName User name string, length 1~32
   * @param id Pointer to store the assigned user ID (optional)
   * @param idAdmin Administrator flag (eIsAdmin_t)
   * @return Task execution result
   * @retval NO_ACK -1 No response from main module
   * @retval ERROR -2 User name too long or kind is not exist
   * @retval 1 Success
   * @retval 2 Duplicate
   * @retval 3 Enrollment timeout
   */
  int8_t enrollUser(uint8_t kind, const char* userName, uint16_t* id, eIsAdmin_t idAdmin);

  /**
   * @fn getAllNumsFaceUserIDs
   * @brief Get the total number of enrolled face users
   * @details Detailed description (optional)
   * @return Number of face users
   * @retval NO_ACK -1 No response from main module
   */
  int16_t getAllNumsFaceUserIDs(void);

  /**
   * @fn getAllNumsPalmUserIDs
   * @brief Get the total number of enrolled palm vein users
   * @details Detailed description (optional)
   * @return Number of palm vein users
   * @retval NO_ACK -1 No response from main module
   */
  int16_t getAllNumsPalmUserIDs(void);

  /**
   * @fn getRecognitionResult
   * @brief Recognize a user
   * @details Detailed description (optional)
   * @param ID Pointer to store the recognized user information (sId_t type)
   * @return Execution result
   * @retval NO_ACK -1 No response from main module
   * @retval 1 Success
   * @retval 2 Timeout
   * @retval 3 User not found
   */
  int8_t getRecognitionResult(sId_t* ID);

  /**
   * @fn deleteUser
   * @brief Delete a specified user by ID
   * @details Detailed description (optional)
   * @param id User ID, range 1~800
   * @return Deletion result
   * @retval NO_ACK -1 No response from main module
   * @retval ERROR -2 ID out of range (1~800)
   * @retval 1 Deletion successful
   * @retval 2 User not found
   * @retval 3 Unknown error, suggest retry
   */
  int8_t deleteUser(uint16_t id);

  /**
   * @fn deleteAllUser
   * @brief Delete all users
   * @details Detailed description (optional)
   * @return Deletion result
   * @retval NO_ACK -1 No response from main module
   * @retval 1 Deletion successful
   * @retval 2 Unknown error, suggest retry
   */
  int8_t deleteAllUser(void);

  /**
   * @fn ledColor
   * @brief Control LED status
   * @details Detailed description (optional)
   * @param color LED color: COLOR_GREEN green, COLOR_RED red, COLOR_WHITE white
   * @param kind LED_ON turn on, LED_OFF turn off
   * @return Execution result
   * @retval NO_ACK -1 No response from main module
   * @retval ERROR -2 Invalid kind parameter
   * @retval 1 Success
   */
  int8_t ledColor(uint8_t color, uint8_t kind);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             |
Mega2560        |      √       |              |             |
Leonardo        |      √       |              |             |
ESP32           |      √       |              |             |
micro:bit        |      √       |              |             |


## History

- 2026/04/30 - Version 1.0.0 released.

## Credits

Written by Olive-hy(feng.yang@dfrobot.com), 2026-4-30 (Welcome to our [website](https://www.dfrobot.com/))
