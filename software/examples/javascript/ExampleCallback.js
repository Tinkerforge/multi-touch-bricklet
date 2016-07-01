var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Multi Touch Bricklet

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var mt = new Tinkerforge.BrickletMultiTouch(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

// Register touch state callback
mt.on(Tinkerforge.BrickletMultiTouch.CALLBACK_TOUCH_STATE,
    // Callback function for touch state callback
    function (state) {
        var s = '';
        if(state & (1 << 12)) {
            s += 'In proximity, ';
        }
        if((state & 0xfff) === 0) {
            s += 'No electrodes touched';
        }
        else {
            s += 'Electrodes ';
            for(var i = 0; i < 12; ++i) {
                if(state & (1 << i)) {
                    s += i + ' ';
                }
            }
            s += 'touched';
        }
        console.log(s);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
