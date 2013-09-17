#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# get current touch state
tinkerforge call multi-touch-bricklet $uid get-touch-state
