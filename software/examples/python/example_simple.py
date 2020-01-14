#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Multi Touch Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_multi_touch import BrickletMultiTouch

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    mt = BrickletMultiTouch(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current touch state
    state = mt.get_touch_state()
    s = ""

    if state & (1 << 12):
        s += "In proximity, "

    if (state & 0xfff) == 0:
        s += "No electrodes touched"
    else:
        s += "Electrodes "
        for i in range(12):
            if state & (1 << i):
                s += str(i) + " "
        s += "touched"

    print(s)

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()
