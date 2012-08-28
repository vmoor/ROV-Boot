
require 'socket'

HOST = '192.168.0.100'
PORT = 20000

loop do
  conn = TCPServer.new(HOST, PORT)
  antw = conn.read
  
  puts antw
  break if antw == "{q}"
end