function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletMultiTouch;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'fdsg'; % Change to your UID
    
    ipcon = IPConnection(); % Create IP connection
    mt = BrickletMultiTouch(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register touch state callback to function cb_touch_state
    set(mt, 'TouchStateCallback', @(h, e) cb_touch_state(e));

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end

% Callback function for touch_state
function cb_touch_state(e)
    s = '';
    if bitand(e.state, bitshift(1, 12))
        s = strcat(s, 'In proximity, ');
    end
    if (bitand(e.state, hex2dec('FFF'))) == 0
        s = strcat(s, 'No electrodes touched');
    else
        s = strcat(s, 'Electrodes ');
        for i = 0:11
            if bitand(e.state, bitshift(1, i))
                s = strcat(s, ' ');
                s = strcat(s, num2str(i));
                s = strcat(s, ' ');
            end
        end
        s = strcat(s, ' touched');
    end
    disp(s);
end
