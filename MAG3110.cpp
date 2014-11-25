/*
MAG3110.cpp
libary for using the I2C MAG3110 magnetometer
*/
#include <Arduino.h>
#include "MAG3110.h"
#include <Wire.h>
//Constructor
MAG3110::MAG3110()
{
}
// Configure magnetometer
void MAG3110::init()
{
    //CTRL_REG1
    //DR2|DR1|DR0|OS1|OS0|FastRead|Trigger|ActiveMode|
    // 0 | 1 | 1 | 1 | 1 |    0	  |   0	  |   1		 |=dec121
    write(MAG3110_CTRL_REG1, 0x79); // Active Mode, 1.25 Hz datarate, 8x oversampling
    delay(15);
    //CTRL_REG2:
    //AutoMagRst|---|Raw|Mag_Rst|---|---|---|---|
    // 1 		| 0 | 0 | 0 	| 0 | 0 | 0 | 0 |
    write(MAG3110_CTRL_REG2, 0x80); // Auto resets before each measurement
}

int8_t MAG3110::read(uint8_t _register)
{
	int8_t data;
	
	Wire.beginTransmission(MAG_ADDRESS); // transmit to device 0x0E
    Wire.write(_register); // MSB reg
    Wire.endTransmission(); // stop transmitting
    delayMicroseconds(2); //needs at least 1.3us free time between start and stop
    Wire.requestFrom(MAG_ADDRESS, 1); // request 1 byte
    if (Wire.available() > 0) // slave may send less than requested
    {
        data = Wire.read(); // receive the byte
    }
    Wire.endTransmission();
    return data;	
}

void MAG3110::write(uint8_t _register, uint8_t _data)
{
	Wire.begin();
	Wire.beginTransmission(MAG_ADDRESS);
	Wire.write(_register);   
	Wire.write(_data);
	Wire.endTransmission();
}

int16_t MAG3110::read(uint8_t addressh, uint8_t addressl)
{
	int data, t_data;

	Wire.beginTransmission(MAG_ADDRESS);
	Wire.write(addressh);
	Wire.endTransmission();
	delayMicroseconds(2);
	Wire.requestFrom(MAG_ADDRESS, 1);
	if (Wire.available() > 0)
	{
		t_data = Wire.read();
		data = t_data << 8;
	}
	Wire.beginTransmission(MAG_ADDRESS);
	Wire.write(addressl);
	Wire.endTransmission();
	delayMicroseconds(2);
	Wire.requestFrom(MAG_ADDRESS, 1);
	if(Wire.available() > 0)
	{
		data |= Wire.read();
	}
	return data;
}

void MAG3110::Calibrate(unsigned int samples, unsigned int samplesDelayMS)
{
	int16_t x_offset_temp;
	int16_t y_offset_temp;
	int16_t z_offset_temp;
	int16_t x,y,z;

	x_offset = 0;
	y_offset = 0;
	z_offset = 0;

	readXYZ(&x, &y, &z);
	for (int i = 0; i < samples; ++i)
	{
		delay(samplesDelayMS);
		readXYZ(&x, &y, &z);
		x_offset_temp += x;
		y_offset_temp += y;
		z_offset_temp += z;
	}

	x_offset = abs(x_offset_temp) / samples;
	y_offset = abs(y_offset_temp) / samples;
	z_offset = abs(z_offset_temp) / samples;
	if (x_offset_temp > 0) x_offset = -x_offset;
	if (y_offset_temp > 0) y_offset = -y_offset;
	if (z_offset_temp > 0) z_offset = -z_offset;
}

// read X, Y, Z value
void MAG3110::readXYZ(int16_t *x, int16_t *y, int16_t *z)
{
	*x = read(MAG3110_OUT_X_MSB, MAG3110_OUT_X_LSB) + x_offset;
	*y = read(MAG3110_OUT_Y_MSB, MAG3110_OUT_Y_LSB) + x_offset;
	*z = read(MAG3110_OUT_Z_MSB, MAG3110_OUT_Z_LSB) + x_offset;
}
