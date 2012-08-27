#! /usr/bin/ruby

require 'wiringpi'
require 'socket'

HOST = "192.168.0.100"
PORT = 8000

is_new_json_object = false
serial_stop = false
json_message = ""
#chage send to pc
puts "Verbindung zum Serialport wird aufgebaut ...."
s = WiringPi::Serial.new('/dev/ttyAMA0', 19200)
s.serialPuts('Hallo Serial')
puts "Verbunden"

loop do
  antwort = s.serialGetchar
  if antwort != -1
    #ASCII Zahl nach Char konvertieren 
    antw_char = antwort.chr
	
	if antw_char == '{'
	  is_new_json_object = true
	  json_message = ""
	end
	if antw_char == '}'
	  json_message += '}'
#test START
	  s.serialPuts(json_message)
#test END
	end
	if is_new_json_object
	  json_message += antw_char
	end
  end
  
  break if serial_stop 

end

s.serialClose
