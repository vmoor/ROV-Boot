#! /usr/bin/ruby

require 'socket'
require 'json'
load 'serial_server.rb'

HOST = "192.168.0.100"
PORT = 20000
VIDEO_PORT = 20001

def start_video
	
end

def stop_video
	
end

def server_anfrage(result_json)
	case result_json["cmd"]
		when "start_serial"
			puts "start serial"
			serial_serv = Serial_server.new(result_json["host"], result_json["port"])
			serial_serv.start
		when "stop_serial"
			puts "stop serial"
			serial_serv.stop
		when "start_video"
			start_video
		when "stop_video"
		else
			puts "falsches Befehl"
	end

end

def motor_anfrage(result_json)
#TODO
end

server = TCPServer.new(PORT)
puts "Warte auf clients..."

while (client = server.accept)
	puts "Client verbunden"
	loop do	
	   anfrage = client.gets	
		if (anfrage != nil)
		   puts "Anfrage : #{anfrage}"
		   # we convert the returned JSON data to native Ruby
		   # data structure - a hash
		   result_json = JSON.parse(anfrage)
		   # if the hash has 'Error' as a key, we raise an error
		   # if the hash has 'Error' as a key, we raise an error
		   #if result.has_key? 'Error'
		   #	  raise "web service error"
		   #end
			case result_json["to"]
				when "server" 
					server_anfrage(result_json)
				when "motor"
					motor_anfrage(result_json)
				else
					puts "falscher empfaenger"
			end
		end
	   
	end
	puts "warte auf nachsten client..."
end
puts "server stopping..."
client.close
server.close

