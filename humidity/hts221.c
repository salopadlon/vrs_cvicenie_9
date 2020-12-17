/*
 * hts221.c
 *
 *  Created on: Dec 17, 2020
 *      Author: psadl
 */

#include "hts221.h"

uint8_t addres = HTS221_DEVICE_ADDRESS_0;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, addres, 0));
}

void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, addres, 0);
}

void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, addres, 1);
}

int8_t hts221_get_temp()
{
	uint8_t temp[2];
	lsm6ds0_readArray(temp, HTS221_ADDRESS_TEMP, 2);

	return (((int16_t)((temp[1] << 8) | temp[0])) >> 3)  + 25;
}


void hts221_get_hum(float* hum)
{
	uint8_t data[6];
	int16_t humidity;
	uint8_t temp;

	//get current scale and use it for final calculation
    temp = hts221_read_byte(HTS221_ADDRESS_CTRL1);

	temp = temp >> 2;
    temp &= 0x03;			//full scale bits exctracted

    hts221_readArray(data, HTS221_ADDRESS_HUM, 6);

    humidity = ((uint16_t)data[1]) << 8 | data[0];

	*hum = (xx >> 4) / 1000.0f;
}


uint8_t hts221_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_DEVICE_ADDRESS_0);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		status = 0;
		//return status;
	}

	//acc device init

	uint8_t ctrl1 = 8 << 4; // +-2g res
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	return status;
}
