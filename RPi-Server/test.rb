require 'i2c'
addr_write = 0x30
addr_read = 0x30

dev = I2C.create("/dev/i2c-0")

dev.write(addr_write, 0x00)
#set coil
dev.write(addr_write, 0b00000010)

sleep(0.001)

dev.write(addr_write, 0x00)
#reset coil
dev.write(addr_write, 0b00000100)

sleep(0.01)

#messung
dev.write(addr_write, 0x00)
dev.write(addr_write, 0x01)

sleep(0.01)
dev.write(addr_write, 0x00)

#lesen
a = dev.read(addr_read, 1, 0x02)
b = a << 8
puts "Ausgabe:" + a
puts "b: " + b.to_s
puts "Adapter: " + dev.to_s
