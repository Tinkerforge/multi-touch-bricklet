#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_multi_touch'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
mt = BrickletMultiTouch.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current touch state
state = mt.get_touch_state
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

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
