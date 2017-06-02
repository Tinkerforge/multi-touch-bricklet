function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Multi Touch Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    mt = javaObject("com.tinkerforge.BrickletMultiTouch", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current touch state
    state = mt.getTouchState();
    s = "";

    if bitand(state, bitshift(1, 12))
        s = strcat(s, "In proximity, ");
    end

    if (bitand(state, hex2dec("FFF"))) == 0
        s = strcat(s, "No electrodes touched");
    else
        s = strcat(s, "Electrodes ");
        for i = 0:11
            if bitand(state, bitshift(1, i))
                s = strcat(s, num2str(i));
                s = strcat(s, " ");
            end
        end
        s = strcat(s, "touched");
    end

    disp(s);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
