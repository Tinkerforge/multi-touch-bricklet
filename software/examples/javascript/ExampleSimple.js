var IPConnection = require('Tinkerforge/IPConnection');
var BrickletMultiTouch = require('Tinkerforge/BrickletMultiTouch');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'jQs';// Change to your UID

var ipcon = new IPConnection();// Create IP connection
var mt = new BrickletMultiTouch(UID, ipcon);// Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);        
    }
);// Connect to brickd

// Don't use device before ipcon is connected
ipcon.on(IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        //Get current touch state
        mt.getTouchState(
            function(touchState) {
                var s = '';
                if (touchState & (1 << 12)) {
                    s += 'In proximity, ';
                }
                if ((touchState & 0xFFF) === 0) {
                    s += 'No electrodes touched\n'
                }
                else {
                    s += 'Electrodes ';
                    for(var i=0; i<12; i++) {
                        if(touchState & (1 << i)) {
                            s += parseInt(i)+' ';
                        }
                    }
                    s += 'touched\n'
                }
                console.log(s);
            },
            function(error) {
                console.log('Error: '+error);
            }
        );
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

