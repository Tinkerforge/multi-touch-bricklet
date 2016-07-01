<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletMultiTouch.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletMultiTouch;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Multi Touch Bricklet

// Callback function for touch state callback
function cb_touchState($state)
{
    if($state & (1 << 12)) {
        echo "In proximity, ";
    }

    if(($state & 0xfff) == 0) {
        echo "No electrodes touched\n";
    } else {
        echo "Electrodes ";
        for($i = 0; $i < 12; $i++) {
            if($state & (1 << $i)) {
                echo $i . " ";
            }
        }
        echo "touched\n";
    }
}

$ipcon = new IPConnection(); // Create IP connection
$mt = new BrickletMultiTouch(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register touch state callback to function cb_touchState
$mt->registerCallback(BrickletMultiTouch::CALLBACK_TOUCH_STATE, 'cb_touchState');

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
