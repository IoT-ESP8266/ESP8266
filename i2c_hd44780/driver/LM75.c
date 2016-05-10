/*
 * LM75.c
 *
 *  Created on: 2014.06.08.
 *      Author: Csani
 */

/*==============================================================[ INCLUDES ]=*/

//#include "Common/common.h"

#include "driver/i2c.h"

#include "driver/LM75.h"

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

#define LM75A_ADDRESS 0x90
//#define LM75A_ADDRESS 0x9E

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

/*======================================================[ INTERNAL GLOBALS ]=*/

static uint8_t address;

/*======================================================[ EXTERNAL GLOBALS ]=*/

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

void LM75_v_Init_f(uint8_t a2a1a0)
{
	I2C_v_Init_f();
	address = LM75A_ADDRESS | (a2a1a0 << 1);
}

uint16_t LM75_u16_GetTemp10x_f(void)
{
	uint16_t ret = 0;
	if(I2C_bl_Start_f(address + I2C_READ))
	{
		/* failed to issue start condition, possibly no device found */
	}
	else
	{
		ret = I2C_u8_ReadAck_f();
		ret = ret << 3;
		ret |= I2C_u8_ReadNak_f() >> 5;
		I2C_v_Stop_f();
	}
	ret *= 10;
	ret /= 8;
	return ret;
}

real32_t LM75_f32_GetTemp_f(void)
{
	real32_t result;
	result = (real32_t) LM75_u16_GetTemp10x_f();
	result /= 10.0;
	return result;
}

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
 */
