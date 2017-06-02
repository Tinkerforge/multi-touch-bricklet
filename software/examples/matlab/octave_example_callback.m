function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Multi Touch Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    mt = javaObject("com.tinkerforge.BrickletMultiTouch", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register touch state callback to function cb_touch_state
    mt.addTouchStateCallback(@cb_touch_state);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for touch state callback
function cb_touch_state(e)
    s = "";

    if bitand(e.state, bitshift(1, 12))
        s = strcat(s, "In proximity, ");
    end

    if (bitand(e.state, hex2dec("FFF"))) == 0
        s = strcat(s, "No electrodes touched");
    else
        s = strcat(s, "Electrodes ");
        for i = 0:11
            if bitand(e.state, bitshift(1, i))
                s = strcat(s, num2str(i));
                s = strcat(s, " ");
            end
        end
        s = strcat(s, "touched");
    end

    disp(s);
end
