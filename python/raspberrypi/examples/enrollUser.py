'''!
@file enrollUser.py
@brief Example of face or palm user enrollment on Raspberry Pi 2/3/4 (pyserial).
@details This example performs user enrollment every time the board is reset,
@n and prints the enrollment result through the serial port
@n Enrollment should work normally as long as the connection is correct
@copyright   Copyright (c) 2026 DFRobot Co.Ltd (http://www.dfrobot.com)
@license     The MIT License (MIT)
@author      [Olive](feng.yang@dfrobot.com)
@version     V1.0.0
@date        2026-05-10
@url         https://github.com/DFRobot/DFRobot_Biometric
'''

import serial
import time
from DFRobot_Biometric import DFRobot_Biometric, SId

if __name__ == "__main__":
  try:
    ser = serial.Serial(port='/dev/ttyAMA3', baudrate=115200, timeout=0.5)
    ser.write(b"serial start:\r\n")
  except Exception as e:
    ser.write(b"serial fail:: {e}\r\n")
    exit()

  bio = DFRobot_Biometric(port='/dev/serial0', baudrate=115200)
  try:
    ser.write(b"Start connect Module...\r\n")
    while not bio.check_state():
      ser.write(b"Connection Module Fail\r\n")
    ser.write(b" Module is ready\r\n")
    ser.write(b"Starting to enroll face:\r\n")
    result, id = bio.enroll_user(bio.FACE_USER, "zwjhy", bio.ROLE_NORMAL)
    if result == 1:
      ser.write(b"enroll success\r\n")
      ser.write(f"the id is:{id}\r\n".encode())
    elif result == 2:
      ser.write(b"face duplicate\r\n")
    elif result == 3:
      ser.write(b"Enrollment timeout\r\n")
    elif result == bio.ERROR:
      ser.write(b"parameter error\r\n")
    else:
      ser.write(b"Unknown error\r\n")
      time.sleep(2)
  except KeyboardInterrupt:
    print("\n user stop the program")

  except Exception as e:
    print(f"err: {e}")

  finally:
    bio.close()
    print("the serial is closed safely")
