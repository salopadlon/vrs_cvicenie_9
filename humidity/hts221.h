/*
 * hts221.h
 *
 *  Created on: Dec 17, 2020
 *      Author: psadl
 */

#include "main.h"
#include "i2c.h"


#define 	LPS25HB_DEVICE_ADDRESS_0				0xD4 // todo: check in datasheet
#define 	LPS25HB_DEVICE_ADDRESS_1				0xD6 // todo: check in datasheet

#define 	LPS25HB_WHO_AM_I_VALUE					0xBC
#define 	LPS25HB_WHO_AM_I_ADDRES					0x0F

#define 	LPS25HB_ADDRESS_CTRL1					0x20

#define 	LPS25HB_ADDRESS_HUM						0x28
#define 	LPS25HB_ADDRESS_TEMP					0x2A

uint8_t lps25hb_init(void);
void lps25hb_get_hum();
uint8_t lps25hb_read_byte(uint8_t reg_addr);
void lps25hb_write_byte(uint8_t reg_addr, uint8_t value);
int8_t lps25hb_get_temp();
