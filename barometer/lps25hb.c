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

float lps25hb_get_temp()
{
	uint8_t data[2];
	int16_t temp;

	lps25hb_readArray(data, LPS25HB_ADDRESS_TEMP, 2);

	temp = ((int16_t)((data[1] << 8) | data[0]));
	temp = temp/480 + 42.5;

	return temp;
}


float lps25hb_get_press()
{
	uint8_t data[3];
	uint8_t pressure_xl, pressure_l, pressure_h;
	uint32_t pressure;
	uint8_t temp;

	//get current scale and use it for final calculation
    temp = lps25hb_read_byte(LPS25HB_ADDRESS_CTRL1);

	temp = temp >> 2;
    temp &= 0x03;			//full scale bits exctracted

    lps25hb_readArray(data, LPS25HB_ADDRESS_PRESS, 3);

    pressure_xl = data[0];
    pressure_l = data[1];
    pressure_h = data[2];
    pressure = (uint32_t)pressure_l << 8 | pressure_xl;
    pressure = (uint32_t)pressure_h << 16 | pressure;

	return pressure/4096.0;
}


uint8_t lps25hb_init(void)
{

	uint8_t status = 1;

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
			return status;
		}
	}

	uint8_t ctrl1 = 0b11000000;
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;
}
