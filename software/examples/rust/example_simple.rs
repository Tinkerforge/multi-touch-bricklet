use std::{error::Error, io};

use tinkerforge::{ipconnection::IpConnection, multi_touch_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Multi Touch Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let multi_touch_bricklet = MultiTouchBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get current touch state
    let state = multi_touch_bricklet.get_touch_state().recv()?;
    let mut string = String::new();

    if (state & (1 << 12)) == (1 << 12) {
        string.push_str("In proximity, ");
    }

    if (state & 0xfff) == 0 {
        string.push_str("No electrodes touched");
    } else {
        string.push_str("Electrodes ");
        for i in 0..12 {
            if (state & (1 << i)) == (1 << i) {
                string.push_str(&(i.to_string() + " "));
            }
        }
        string.push_str("touched");
    }

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
