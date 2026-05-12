'''!
@file SEN0737_get_delete_id.py
@brief This routine implements user lookup and deletion on Raspberry Pi 2/3/4 (pyserial)..
@details This routine sends string commands over serial to the host controller
@n which parses and executes the corresponding actions on the module.
@n Supported commands: AT+GETUSERS (get number of users), AT+DELUSER=<ID> (delete user by ID), and AT+DELALLUSERS (delete all users).
@n After each command is successfully executed, the indicator light will display different colors corresponding to the command result.
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
  ser.reset_input_buffer()
  try:
    while not bio.check_state():
      ser.write(b"Connection Module Fail\r\n")
    ser.write(b" Module is ready\r\n")
    while True:
      if ser.in_waiting > 0:
        data = ser.read(ser.in_waiting).strip()
        if data == b"AT+GETUSERS":
          nums = bio.get_all_nums_face_user_ids()
          ser.write(f"the face ids id:{nums}\r\n".encode())
          nums = bio.get_all_nums_palm_user_ids()
          ser.write(f"the palm ids id:{nums}\r\n".encode())
          bio.led_color(bio.COLOR_RED, bio.LED_OFF)
          bio.led_color(bio.COLOR_WHITE, bio.LED_OFF)
          bio.led_color(bio.COLOR_GREEN, bio.LED_ON)
        elif data == b"AT+DELALLUSERS":
          result = bio.delete_all_user()
          if result == 1:
            ser.write(b"Success\r\n")
          else:
            ser.write(b"Fail\r\n")
          bio.led_color(bio.COLOR_WHITE, bio.LED_OFF)
          bio.led_color(bio.COLOR_GREEN, bio.LED_OFF)
          bio.led_color(bio.COLOR_RED, bio.LED_ON)
        elif data.startswith(b"AT+DELUSER="):
          user_id = data.split(b"=")[1]
          result = bio.delete_user(int(user_id))
          if result == 1:
            ser.write(b"Success\r\n")
          else:
            ser.write(b"Fail\r\n")
          bio.led_color(bio.COLOR_GREEN, bio.LED_OFF)
          bio.led_color(bio.COLOR_RED, bio.LED_OFF)
          bio.led_color(bio.COLOR_WHITE, bio.LED_ON)
        else:
          ser.write(b"Unknown command\r\n")
    time.sleep(0.5)

  except KeyboardInterrupt:
    print("\n user stop the program")

  except Exception as e:
    print(f"err: {e}")

  finally:
    bio.close()
    print("the serial is closed safely")
