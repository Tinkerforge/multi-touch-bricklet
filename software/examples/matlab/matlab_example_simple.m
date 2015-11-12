function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletMultiTouch;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    mt = handle(BrickletMultiTouch(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current touch state
    state = mt.getTouchState();
    s = '';

    if bitand(state, bitshift(1, 12))
        s = strcat(s, 'In proximity, ');
    end

    if (bitand(state, hex2dec('FFF'))) == 0
        s = strcat(s, 'No electrodes touched');
    else
        s = strcat(s, 'Electrodes ');
        for i = 0:11
            if bitand(state, bitshift(1, i))
                s = strcat(s, num2str(i));
                s = strcat(s, ' ');
            end
        end
        s = strcat(s, 'touched');
    end

    disp(s);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
