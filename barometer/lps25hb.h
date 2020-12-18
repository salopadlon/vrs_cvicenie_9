/*
 * lps25hb.h
 *
 *  Created on: Dec 17, 2020
 *      Author: psadl
 */

#include "main.h"
#include "i2c.h"


#define 	LPS25HB_DEVICE_ADDRESS_0				0xB8
#define 	LPS25HB_DEVICE_ADDRESS_1				0xBA

#define 	LPS25HB_WHO_AM_I_VALUE					0xBD
#define 	LPS25HB_WHO_AM_I_ADDRES					0x0F

#define 	LPS25HB_ADDRESS_CTRL1					0x20

#define 	LPS25HB_ADDRESS_PRESS					0x28
#define 	LPS25HB_ADDRESS_TEMP					0x2B

uint8_t lps25hb_init(void);
float lps25hb_get_press();
uint8_t lps25hb_read_byte(uint8_t reg_addr);
void lps25hb_write_byte(uint8_t reg_addr, uint8_t value);
float lps25hb_get_temp();
