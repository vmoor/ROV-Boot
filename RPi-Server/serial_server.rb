#! /usr/bin/ruby
class Serial_server
	
	
	require 'wiringpi'
	require 'socket'

	def initialize(host = "192.168.0.100", port = 20002)
		@host = host
		@port = port
		@json_message = ""
		@is_running = false
		@is_new_json_object = false
	end
	

	def start()
		@is_running = true
		#serielle schnittstelle initialisieren
		s = WiringPi::Serial.new('/dev/ttyAMA0', 19200)
		#Test ------------------------------------
		s.serialPuts('Hallo Serial')
		
		#conn = TCPSocket.new(@host, @port)
		
		puts "Serial-Server ist gestartet"
		while (@is_running)
			loop do
				antwort = s.serialGetchar
				if antwort != -1
				  #ASCII Zahl nach Char konvertieren 
				  antw_char = antwort.chr
				
				  if antw_char == '{'
					@is_new_json_object = true
					@json_message = ""
				  end

				  if @is_new_json_object
					@json_message += antw_char
				  end
				  
				  break if antw_char == '}'
				end
			end		
			#conn.printf "%s\n", json_message
			puts "mes: " + @json_message
			s.serialPuts(@json_message)
		end	
		#conn.close
		s.serialClose
		puts "Serial-Server wurde gestopt"
		
	end


	def stop()
		@is_running = false
		puts "Serial-Server wird angehalten..."
	end
end