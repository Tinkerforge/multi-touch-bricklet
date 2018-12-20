use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, multi_touch_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Multi Touch Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let mt = MultiTouchBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd
                                          // Don't use device before ipcon is connected

    let touch_state_receiver = mt.get_touch_state_callback_receiver();

    // Spawn thread to handle received events.
    // This thread ends when the `mt` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for state_change in touch_state_receiver {
            let mut string = String::new();

            if (state_change & (1 << 12)) == (1 << 12) {
                string.push_str("In proximity, ");
            }

            if (state_change & 0xfff) == 0 {
                string.push_str("No electrodes touched");
            } else {
                string.push_str("Electrodes ");
                for i in 0..12 {
                    if (state_change & (1 << i)) == (1 << i) {
                        string.push_str(&(i.to_string() + " "));
                    }
                }
                string.push_str("touched");
            }
        }
    });

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
