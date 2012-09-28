#! /usr/bin/ruby
load "compass.rb"

dev = Compass.new
while (true)
  dev.take_measurement
 # puts "control register: " + dev.get_controllreg.to_s
 #puts "x: " + dev.get_x_axis.to_s + "    y: " + dev.get_y_axis.to_s
  puts dev.get_inklination
  sleep (0.035)
end