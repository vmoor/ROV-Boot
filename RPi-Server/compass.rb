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
  
  def get_x_axis
	return @x_axis_val  
  end
  
  def get_y_axis
	return @y_axis_val
  end
  
  def get_controllreg
	return @controllreg
  end
  
#@in: value of the x-axis and y-axis from the hdmm01
#@out:angle theta between sensor and reference-0°
  def get_inklination
#  	/************************************************************************/
#	/* To calculate the angle theta between sensor and reference-0° we have */
#	/* to convert cartesian koordinates represented through the variables	*/
#	/* x_axis and y_axis into the polar koordiantes represented	through an	*/
#	/* uncalculated radius r and the searched angle theta.					*/
#	/* The conversion x,y=>angle is done be the term theta = arctan(x/y).	*/
#	/* Because of the periodicity of the tangens funtion (also the inverse	*/
#	/* function arcustangens) we can only calculate the first 180 degrees.	*/
#	/* The next 180 degrees we have determine through the quadrants in		*/
#	/* the cartesian koordinates are.										*/
#	/*																		*/
#	/* To determine all the quadrants we have to add or subtract pi.		*/
#	/* (I. and IV. Quadrant => normally, II. => +pi, III. => -pi)			*/
#	/*																		*/
#	/* SO AFTER SOME GOOGLING I HAVE FOUND OUT THAT THE STANDARD FUNCTION	*/
#	/* atan2(x,y) DOES ALL THE STUFF NAMED IN THE UPPER PART. *fuuuuuu*		*/
#	/*																		*/
#	/* From the function atan2() we get back radiants so we have to convert	*/
#	/* it back into degrees throug the function:							*/
#	/* 360°=2pi rad  => 1rad = 360°/2pi	=> 1rad = 180°/pi					*/
#	/*																		*/
#	/* Not implemented is which angle represents which direction. (0° = N?) */
#	/************************************************************************/

	# conversion theta_radiants-degrees
	@theta_degrees = Math.atan2(@x_axis_val - 2045, @y_axis_val - 2045) * 180 / PI
	if @theta_degrees < 0
       @theta_degrees = @theta_degrees + 360	
	end
	return @theta_degrees
  end

end