'''!
@file SEN0736_Recognition.py
@brief Continuous automatic user recognition routine on Raspberry Pi 2/3/4 (pyserial).
@details This routine periodically detects faces, automatically recognizes them
@n and prints user details via the serial port.
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
    while True:
      ser.write(b"Start connect Module...\r\n")
      while not bio.check_state():
        ser.write(b"Connection Module Fail\r\n")
      ser.write(b" Module is ready\r\n")
      my_sid = SId()
      result = bio.get_recognition_result(my_sid)
      if result == 1:
        ser.write(b"Recognition successful\r\n")
        ser.write(f"the id is:{my_sid}\r\n".encode())
      elif result == 2:
        ser.write(b"Recognition timeout\r\n")
      elif result == 3:
        ser.write(b"User not found\r\n")
      elif result == bio.ERROR:
        ser.write(b"parameter error\r\n")
      else:
        ser.write(b"Unknown error\r\n")
      time.sleep(10)
  except KeyboardInterrupt:
    print("\n user stop the program")

  except Exception as e:
    print(f"err: {e}")

  finally:
    bio.close()
    print("the serial is closed safely")
