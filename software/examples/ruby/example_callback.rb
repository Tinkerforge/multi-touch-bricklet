#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_multi_touch'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Multi Touch Bricklet

ipcon = IPConnection.new # Create IP connection
mt = BrickletMultiTouch.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register touch state callback
mt.register_callback(BrickletMultiTouch::CALLBACK_TOUCH_STATE) do |state|
  s = ''

  if (state & (1 << 12)) == (1 << 12)
    s += 'In proximity, '
  end

  if (state & 0xfff) == 0
    s += 'No electrodes touched'
  else
    s += 'Electrodes '
    for i in 0..11
      if (state & (1 << i)) == (1 << i)
        s += i.to_s() + ' '
      end
    end
    s += 'touched'
  end

  puts s
end

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
