#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# handle incoming touch state callbacks
tinkerforge dispatch multi-touch-bricklet $uid touch-state
