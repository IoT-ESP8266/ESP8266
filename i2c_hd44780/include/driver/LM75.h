/*
 * LM75.h
 *
 *  Created on: 2014.06.08.
 *      Author: Csani
 */

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*===========================================================[ HEADER INIT ]=*/

#ifndef LM75_H_
#define LM75_H_

/*=======================================================[ EXTERNAL MACROS ]=*/
/* macro constants and read macros */

/*=====================================================[ EXTERNAL TYPEDEFS ]=*/
/* State Machine Constants */

/*======================================================[ EXTERNAL GLOBALS ]=*/
/* Just for debug !!!!*/

/*=========================================[ EXTPORTED FUNCTION PROTOTYPES ]=*/

extern void LM75_v_Init_f(uint8_t a2a1a0);
extern uint16_t LM75_u16_GetTemp10x_f(void);
extern real32_t LM75_f32_GetTemp_f(void);

#endif /* LM75_H_ */

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
 */
