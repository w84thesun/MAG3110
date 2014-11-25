/*
MAG3110.h
libary for using the I2C MAG3110 magnetometer
*/
#ifndef __MAG3110_H__
#define __MAG3110_H__

#include <Arduino.h>

// define magnetometer I2C address (fixed)
#define MAG_ADDRESS 0x0E // Magnetometer MAG3110
// MAG3110 register defines
#define MAG3110_DR_STATUS 0x00
#define MAG3110_OUT_X_MSB 0x01
#define MAG3110_OUT_X_LSB 0x02
#define MAG3110_OUT_Y_MSB 0x03
#define MAG3110_OUT_Y_LSB 0x04
#define MAG3110_OUT_Z_MSB 0x05
#define MAG3110_OUT_Z_LSB 0x06
#define MAG3110_WHO_AM_I  0x07
#define MAG3110_SYSMOD    0x08
#define MAG3110_OFF_X_MSB 0x09
#define MAG3110_OFF_X_LSB 0x0A
#define MAG3110_OFF_Y_MSB 0x0B
#define MAG3110_OFF_Y_LSB 0x0C
#define MAG3110_OFF_Z_MSB 0x0D
#define MAG3110_OFF_Z_LSB 0x0E
#define MAG3110_DIE_TEMP  0x0F
#define MAG3110_CTRL_REG1 0x10
#define MAG3110_CTRL_REG2 0x11

class MAG3110
{
private:
	int8_t read(uint8_t _register);
	int16_t x_offset;
	int16_t y_offset;
	int16_t z_offset;

public:
    MAG3110();
    void init();
    void readXYZ(int16_t *x, int16_t *y, int16_t *z);
    //вспомогательные функции для записи/чтения данных
    int16_t read(uint8_t addressh, uint8_t addressl);
    void write(uint8_t _register, uint8_t _data);

    void Calibrate(unsigned int samlpes, unsigned int saplesDelayMS); 
};

#endif // __MAG3110_H__