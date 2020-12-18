/*
 * hts221.c
 *
 *  Created on: Dec 17, 2020
 *      Author: psadl
 */

#include "hts221.h"

uint8_t address_hts = HTS221_DEVICE_ADDRESS_0;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address_hts, 0));
}

void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address_hts, 0);
}

void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, address_hts, 1);
}

int32_t hts221_get_temp()
{
	 int16_t T0_out, T1_out, T_out, T0_degC_x8_u16, T1_degC_x8_u16;
	 int16_t T0_degC, T1_degC;
	 uint8_t buffer[4], tmp;
	 int32_t tmp32, temperature;

	 hts221_readArray(buffer, 0x32, 4);
	 tmp = hts221_read_byte(0x35);

	T0_degC_x8_u16 = (((uint16_t)(tmp & 0x03)) << 8) | ((uint16_t)buffer[0]);
	T1_degC_x8_u16 = (((uint16_t)(tmp & 0x0C)) << 6) | ((uint16_t)buffer[1]);
	T0_degC = T0_degC_x8_u16>>3;
	T1_degC = T1_degC_x8_u16>>3;

	hts221_readArray(buffer, 0x3C, 4);
	T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
	T1_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];

	hts221_readArray(buffer, 0x2A, 4);
	T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

	tmp32 = ((int32_t)(T_out - T0_out)) * ((int32_t)(T1_degC - T0_degC));
	temperature = tmp32 /(T1_out - T0_out) + T0_degC;
	return temperature;
}


uint16_t hts221_get_hum()
{
	int16_t H0_T0_out, H1_T0_out, H_T_out;
	int16_t H0_rh, H1_rh;
	uint8_t buffer[2];
	int32_t tmp;
	uint16_t humidity;

    hts221_readArray(buffer, 0x30, 2);
    H0_rh = buffer[0]>>1;
    H1_rh = buffer[1]>>1;

    hts221_readArray(buffer, 0x36, 2);
    H0_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

    hts221_readArray(buffer, 0x3A, 2);
    H1_T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

    hts221_readArray(buffer, HTS221_ADDRESS_HUM, 2);
    H_T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

    tmp = ((int32_t)(H_T_out - H0_T0_out)) * ((int32_t)(H1_rh - H0_rh));
    humidity = (uint16_t)(tmp/(H1_T0_out - H0_T0_out) + H0_rh);

    if (humidity > 100) humidity = 100;

	return humidity;
}


uint8_t hts221_init(void)
{

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else
	{
		status = 0;
		return status;
	}

	uint8_t ctrl1 = 0b10000111;
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	return status;
}
