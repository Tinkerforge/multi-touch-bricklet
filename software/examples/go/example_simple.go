package main

import (
	"fmt"
    "github.com/tinkerforge/go-api-bindings/ipconnection"
    "github.com/tinkerforge/go-api-bindings/multi_touch_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Multi Touch Bricklet.

func main() {
	ipcon := ipconnection.New()
    defer ipcon.Close()
	mt, _ := multi_touch_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
    defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get current touch state.
	state, _ := mt.GetTouchState()
    var str string
    
    if (state & (1 << 12)) == (1 << 12) {
        str += "In proximity, "
    }
    
    if (state & 0xfff) == 0 {
        str += "No electrodes touched"
    } else {
        str += "Electrodes "
        for i := uint(0); i < 12; i++ {
            if (state & (1 << i)) == (1 << i) {
                str += string(i) + " "
            }
        }
        str += "touched"
    }    
    
	fmt.Print("Press enter to exit.")
	fmt.Scanln()

	ipcon.Disconnect()
}
