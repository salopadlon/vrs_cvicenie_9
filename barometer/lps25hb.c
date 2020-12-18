/*
 * lps25hb.c
 *
 *  Created on: Dec 17, 2020
 *      Author: psadl
 */

#include "lps25hb.h"

uint8_t address_lps = LPS25HB_DEVICE_ADDRESS_0;

uint8_t lps25hb_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address_lps, 0));
}


void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address_lps, 0);
}

void lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, address_lps, 1);
}

int8_t lps25hb_get_temp()
{
	uint8_t temp[2];
	lps25hb_readArray(temp, LPS25HB_ADDRESS_TEMP, 2);

	return (((int16_t)((temp[1] << 8) | temp[0])) >> 3)  + 25;
}


void lps25hb_get_press(float* press)
{
	uint8_t data[6];
	int16_t pressure;
	uint8_t temp;

	//get current scale and use it for final calculation
    temp = lps25hb_read_byte(LPS25HB_ADDRESS_CTRL1);

	temp = temp >> 2;
    temp &= 0x03;			//full scale bits exctracted

    lps25hb_readArray(data, LPS25HB_ADDRESS_PRESS, 6);

    pressure = ((uint16_t)data[1]) << 8 | data[0];

	*press = (pressure >> 4) / 1000.0f;
}


uint8_t lps25hb_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		address_lps = LPS25HB_DEVICE_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);
		if(val == LPS25HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}

	//acc device init

	uint8_t ctrl1 = 0x11000000;
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;
}
