#!/usr/bin/env python
# -*- coding: utf-8 -*-  

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_multi_touch import MultiTouch

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    mt = MultiTouch(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current touch state
    touch_state = mt.get_touch_state()

    s = ''
    if touch_state & (1 << 12):
        s += 'In proximity, '

    if (touch_state & 0xFFF) == 0:
        s += 'No electrodes touched\n'
    else:
        s += 'Electrodes '
        for i in range(12):
            if touch_state & (1 << i):
                s += str(i) + ' '
        s += 'touched\n'

    print(s)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
