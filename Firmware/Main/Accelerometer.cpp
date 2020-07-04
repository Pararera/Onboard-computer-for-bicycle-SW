
/*

	ACCELEROMETER FUNCTIONS
	MPU9250
	CREDITS: GreatScott

*/


// MODULES
#include 																			"Headers\Accelerometer.h"



// OBJECT
AccelClass Accelerometer;



// FUNCTIONS
void AccelClass::read(uint8_t address, uint8_t regAddr, uint8_t bytes, uint8_t *data)
{
	Wire.beginTransmission(address);
	Wire.write(regAddr);
	Wire.endTransmission();

	Wire.requestFrom(address, bytes);
	uint8_t index = 0;
	while (Wire.available())
	{
		data[index++] = Wire.read();
	}
}

void AccelClass::write(uint8_t address, uint8_t regAddr, uint8_t data)
{
	Wire.beginTransmission(address);
	Wire.write(regAddr);
	Wire.write(data);
	Wire.endTransmission();
}

float AccelClass::leanAngle(void)
{
	uint8_t buffer[14];
	int16_t data = 0;

	read(MPU9250, 0x3B, 14, buffer);
	data = -(buffer[2] << 8 | buffer[3]);

	return (data / 182.04);	
}

void AccelClass::init(void)
{ 
	write(MPU9250, 29, 0x06);
	write(MPU9250, 28, 0x00);
	write(MPU9250, 108, 0x2F); 
}
