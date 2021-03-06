/*
 * I2C.h
 *
 *  Created on: 2014.06.07.
 *      Author: Csani
 */

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*===========================================================[ HEADER INIT ]=*/
#include "ets_sys.h"
#include "osapi.h"
#ifndef I2C_H_
#define I2C_H_

/*=======================================================[ EXTERNAL MACROS ]=*/
/* macro constants and read macros */

/** defines the data direction (reading from I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_READ    1

/** defines the data direction (writing to I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_WRITE   0

/*=====================================================[ EXTERNAL TYPEDEFS ]=*/
/* State Machine Constants */

/*======================================================[ EXTERNAL GLOBALS ]=*/
/* Just for debug !!!!*/

/*=========================================[ EXTPORTED FUNCTION PROTOTYPES ]=*/

extern void I2C_v_Init_f(void);
extern bool I2C_bl_Start_f(uint8_t address);
extern void I2C_v_StartWait_f(uint8_t address);
extern bool I2C_bl_RepStart_f(uint8_t address);
extern void I2C_v_Stop_f(void);
extern bool I2C_bl_Write_f(uint8_t data);
extern uint8_t I2C_u8_ReadAck_f(void);
extern uint8_t I2C_u8_ReadNak_f(void);

#endif /* I2C_H_ */


/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
 */
