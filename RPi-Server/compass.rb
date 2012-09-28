require "rubygems"


# need 'i2c' gem installed
require "i2c/i2c"
require "i2c/backends/i2c-dev"

class Compass
  DEV_ADDRESS = 0x30
  PI = 3.141592
  
  def initialize(device = "/dev/i2c-0", address = DEV_ADDRESS) 
    @device = ::I2C.create(device)
	@address = address  
	@controllreg = 0
	@x_axis_val = 0
	@y_axis_val = 0
  end
  
  def take_measurement
	@device.write(DEV_ADDRESS, 0x00)
	@device.write(DEV_ADDRESS, 0x00, 0b00000010)
	@device.write(DEV_ADDRESS, 0x00)
	@device.write(DEV_ADDRESS, 0x00, 0b00000100)
	@device.write(DEV_ADDRESS, 0x00)
    #write user register to trigger measurement
	
	@device.write(DEV_ADDRESS, 0x00, 0b10000001)
	sleep(0.01)
	#get data
	@device.write(DEV_ADDRESS, 0x00)
	@controllreg = @device.read(DEV_ADDRESS, 1, 0x00).unpack("C")[0]
	@x_axis_msb = @device.read(DEV_ADDRESS, 1, 0x01).unpack("C")[0]
	@x_axis_lsb = @device.read(DEV_ADDRESS, 1, 0x02).unpack("C")[0]
	@y_axis_msb = @device.read(DEV_ADDRESS, 1, 0x03).unpack("C")[0]
	@y_axis_lsb = @device.read(DEV_ADDRESS, 1, 0x04).unpack("C")[0]
	
	@x_axis_val = (@x_axis_msb & 0xf) * 256 + @x_axis_lsb
	
	@y_axis_val = (@y_axis_msb & 0xf) * 256 + @y_axis_lsb

  end
  
#  def get_x_axis
#	return @x_axis_val  
#  end
  
#  def get_y_axis
#	return @y_axis_val
#  end
  
#  def get_controllreg
#	return @controllreg
#  end
  
#@in: value of the x-axis and y-axis from the hdmm01
#@out:angle theta between sensor and reference-0°
  def get_inklination

	# conversion theta_radiants-degrees
	@theta_degrees = Math.atan2(@x_axis_val - 2045, @y_axis_val - 2045) * 180 / PI
	if @theta_degrees < 0
       @theta_degrees = @theta_degrees + 360	
	end
	return @theta_degrees.round
  end

end