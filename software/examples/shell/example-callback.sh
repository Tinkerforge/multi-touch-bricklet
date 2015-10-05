#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this


uid=XYZ # Change to your UID

# Handle incoming touch state callbacks
tinkerforge dispatch multi-touch-bricklet $uid touch-state &

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
