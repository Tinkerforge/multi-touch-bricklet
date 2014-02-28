var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'jQs'; // Change to your UID

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var mt = new Tinkerforge.BrickletMultiTouch(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

// Register touch state callback
mt.on(Tinkerforge.BrickletMultiTouch.CALLBACK_TOUCH_STATE,
    // Callback function for touch state
    function(touchState) {
        var s = '';
        if(touchState & (1 << 12)) {
            s += 'In proximity, ';
        }
        if((touchState & 0xFFF) === 0) {
            s += 'No electrodes touched\n';
        }
        else {
            s += 'Electrodes ';
            for(var i=0; i<12; i++) {
                if(touchState & (1 << i)) {
                    s += parseInt(i) + ' ';
                }
            }
            s += 'touched\n';
        }
        console.log(s);
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
