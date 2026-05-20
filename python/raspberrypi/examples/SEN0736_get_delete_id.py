'''!
@file SEN0736_get_delete_id.py
@brief This routine implements user lookup and deletion on Raspberry Pi 2/3/4 (pyserial).
@details This routine sends string commands over serial to the host controller
@n which parses and executes the corresponding actions on the module.
@n Supported commands: AT+GETUSERS (get number of users), AT+DELUSER=<ID> (delete user by ID), and AT+DELALLUSERS (delete all users).
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
    ser = serial.Serial(port='/dev/ttyAMA3', baudrate=115200, timeout=0.5)  # Initialize the serial port for printing information
    ser.write(b"serial start:\r\n")
  except Exception as e:
    ser.write(b"serial fail:: {e}\r\n")
    exit()

  bio = DFRobot_Biometric(port='/dev/serial0', baudrate=115200)  # Initialize the class object and pass in a serial port
  ser.reset_input_buffer()
  try:
    # Determine whether the module is ready
    while not bio.check_state():
      ser.write(b"Connection Module Fail\r\n")
    # Command Introduction
    ser.write(b" Module is ready\r\n")
    ser.write(b"-----------------------------\r\n")
    ser.write(b"Available commands:\r\n")
    ser.write(b"AT+GETUSERS       # Get all user information\r\n")
    ser.write(b"AT+DELUSER=ID     # Delete the specified user (e.g. AT+DELUSER=0x1001)\r\n")
    ser.write(b"AT+DELALLUSERS    # Delete all users\r\n")
    ser.write(b"-----------------------------\r\n")
    while True:
      while not ser.in_waiting:
        time.sleep(0.1)
      # Read the user's command
      if ser.in_waiting > 0:
        data = ser.read(ser.in_waiting).strip()
        # Match the command of user
        if data == b"AT+GETUSERS":
          nums = bio.get_all_nums_face_user_ids()
          ser.write(f"the nums of face id: {nums}\r\n".encode())
          id_buffer = [0] * 50
          bio.get_all_face_user_ids(id_buffer, 50)
          ser.write(b"the specific information of face id:\r\n")
          for i in range(nums):
            ser.write(f"face id {i + 1}: {hex(id_buffer[i])}\r\n".encode())
          nums = bio.get_all_nums_palm_user_ids()
          ser.write(f"the nums of palm id: {nums}\r\n".encode())
        elif data == b"AT+DELALLUSERS":
          result = bio.delete_all_user()
          if result == 1:
            ser.write(b"Successful delete all users\r\n")
          else:
            ser.write(b"Fail\r\n")
        elif data.startswith(b"AT+DELUSER="):
          user_id = data.split(b"=")[1]
          result = bio.delete_user(int(user_id))
          if result == 1:
            ser.write(f"Successful delete user:{user_id.decode()}\r\n".encode())
          else:
            ser.write(b"Fail\r\n")
        else:
          ser.write(b"Unknown command\r\n")
      time.sleep(0.5)
      ser.write(b"\r\n")
      ser.write(b"please continue command:\r\n")

  except KeyboardInterrupt:
    print("\n user stop the program")

  except Exception as e:
    print(f"err: {e}")

  finally:
    bio.close()
    print("the serial is closed safely")
