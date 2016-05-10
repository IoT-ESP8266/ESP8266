/*
 * I2C.c
 *
 *  Created on: 2014.06.07.
 *      Author: Csani
 */

/*==============================================================[ INCLUDES ]=*/

//#include "Common/common.h"

#include "user_config.h"
#include "ets_sys.h"
#include "osapi.h"
#include "user_config.h"
#include "gpio.h"
//#include "gpio_esp.h"

#include "driver/i2c.h"

/*=================================================[ COMPILER SWITCH CHECK ]=*/

/*=======================================================[ INTERNAL MACROS ]=*/

#define i2c_read() GPIO_INPUT_GET(GPIO_ID_PIN(I2C_SDA_PIN));

/*=====================================================[ INTERNAL TYPEDEFS ]=*/

/*===================================================[ INTERNAL PROTOTYPES ]=*/
/* static functions only! */

/*======================================================[ INTERNAL GLOBALS ]=*/

static bool InitReady = FALSE;

/*======================================================[ EXTERNAL GLOBALS ]=*/

/*=========================================[ INTERNAL FUNCTION DEFINITIONS ]=*/

/**
 * Set SDA to state
 */LOCAL void ICACHE_FLASH_ATTR
i2c_sda(uint8 state) {
	state &= 0x01;
	//Set SDA line to state
	if (state)
		gpio_output_set(1 << I2C_SDA_PIN, 0, 1 << I2C_SDA_PIN, 0);
	else
		gpio_output_set(0, 1 << I2C_SDA_PIN, 1 << I2C_SDA_PIN, 0);
}

/**
 * Set SCK to state
 */LOCAL void ICACHE_FLASH_ATTR
i2c_sck(uint8 state) {
	//Set SCK line to state
	if (state)
		gpio_output_set(1 << I2C_SCK_PIN, 0, 1 << I2C_SCK_PIN, 0);
	else
		gpio_output_set(0, 1 << I2C_SCK_PIN, 1 << I2C_SCK_PIN, 0);
}

/**
 * I2C Start signal
 */
void ICACHE_FLASH_ATTR
i2c_start(void) {
	i2c_sda(1);
	i2c_sck(1);
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sda(0);
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sck(0);
	os_delay_us(I2C_SLEEP_TIME);
}

/**
 * I2C Stop signal
 */
void ICACHE_FLASH_ATTR
i2c_stop(void) {
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sck(1);
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sda(1);
	os_delay_us(I2C_SLEEP_TIME);
}

/**
 * Send I2C ACK to the bus
 * uint8 state 1 or 0
 *  1 for ACK
 *  0 for NACK
 */
void ICACHE_FLASH_ATTR
i2c_send_ack(uint8 state) {
	i2c_sck(0);
	os_delay_us(I2C_SLEEP_TIME);
	//Set SDA
	//  HIGH for NACK
	//  LOW  for ACK
	i2c_sda((state ? 0 : 1));

	//Pulse the SCK
	i2c_sck(0);
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sck(1);
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sck(0);
	os_delay_us(I2C_SLEEP_TIME);

	i2c_sda(1);
	os_delay_us(I2C_SLEEP_TIME);
}

/**
 * Receive I2C ACK from the bus
 * returns 1 or 0
 *  1 for ACK
 *  0 for NACK
 */
uint8 ICACHE_FLASH_ATTR
i2c_check_ack(void) {
	uint8 ack;
	i2c_sda(1);
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sck(0);
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sck(1);
	os_delay_us(I2C_SLEEP_TIME);

	//Get SDA pin status
	ack = i2c_read();

	os_delay_us(I2C_SLEEP_TIME);
	i2c_sck(0);
	os_delay_us(I2C_SLEEP_TIME);
	i2c_sda(0);
	os_delay_us(I2C_SLEEP_TIME);

	return (ack ? 0 : 1);
}

/**
 * Receive byte from the I2C bus
 * returns the byte
 */
uint8 ICACHE_FLASH_ATTR
i2c_readByte(void)
{
    uint8 data = 0;
    uint8 data_bit;
    uint8 i;

    i2c_sda(1);

    for (i = 0; i < 8; i++)
    {
        os_delay_us(I2C_SLEEP_TIME);
        i2c_sck(0);
        os_delay_us(I2C_SLEEP_TIME);

        i2c_sck(1);
        os_delay_us(I2C_SLEEP_TIME);

        data_bit = i2c_read();
        os_delay_us(I2C_SLEEP_TIME);

        data_bit <<= (7 - i);
        data |= data_bit;
    }
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);

    return data;
}

/**
 * Write byte to I2C bus
 * uint8 data: to byte to be writen
 */
void ICACHE_FLASH_ATTR
i2c_writeByte(uint8 data)
{
    uint8 data_bit;
    sint8 i;

    os_delay_us(I2C_SLEEP_TIME);

    for (i = 7; i >= 0; i--) {
        data_bit = data >> i;
        i2c_sda(data_bit);
        os_delay_us(I2C_SLEEP_TIME);
        i2c_sck(1);
        os_delay_us(I2C_SLEEP_TIME);
        i2c_sck(0);
        os_delay_us(I2C_SLEEP_TIME);
    }
}

/*=========================================[ EXPORTED FUNCTION DEFINITIONS ]=*/

/*************************************************************************
 Initialization of the I2C bus interface. Need to be called only once
 *************************************************************************/
void ICACHE_FLASH_ATTR I2C_v_Init_f(void) {
	if (InitReady)
		return;

	//Disable interrupts
	ETS_GPIO_INTR_DISABLE();

	//Set pin functions
	PIN_FUNC_SELECT(I2C_SDA_MUX, I2C_SDA_FUNC);
	PIN_FUNC_SELECT(I2C_SCK_MUX, I2C_SCK_FUNC);

	//Set SDA as open drain
	GPIO_REG_WRITE(
			GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SDA_PIN)),
			GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SDA_PIN))) |
			GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)
	);

	GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SDA_PIN));

	//Set SCK as open drain
	GPIO_REG_WRITE(
			GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SCK_PIN)),
			GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_SCK_PIN))) |
			GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)
	);

	GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_SCK_PIN));

	//Turn interrupt back on
	ETS_GPIO_INTR_ENABLE();

	i2c_sda(1);
	i2c_sck(1);

	InitReady = TRUE;
}

/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 return FALSE = device accessible, TRUE= failed to access device
 *************************************************************************/
bool ICACHE_FLASH_ATTR I2C_bl_Start_f(uint8_t address) {
	i2c_start();
	i2c_writeByte(address + I2C_WRITE);
	if (!i2c_check_ack()) {
		i2c_stop();
		return TRUE;
	}

	return FALSE;

}/* i2c_start */

/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 If device is busy, use ack polling to wait until device is ready

 Input:   address and transfer direction of I2C device
 *************************************************************************/
void ICACHE_FLASH_ATTR I2C_v_StartWait_f(uint8_t address) {

}/* i2c_start_wait */

/*************************************************************************
 Issues a repeated start condition and sends address and transfer direction

 Input:   address and transfer direction of I2C device

 Return:  FALSE device accessible
 TRUE failed to access device
 *************************************************************************/
bool ICACHE_FLASH_ATTR I2C_bl_RepStart_f(uint8_t address) {
	return I2C_bl_Start_f(address);
}/* i2c_rep_start */

/*************************************************************************
 Terminates the data transfer and releases the I2C bus
 *************************************************************************/
void ICACHE_FLASH_ATTR I2C_v_Stop_f(void) {
	i2c_stop();
}/* i2c_stop */

/*************************************************************************
 Send one byte to I2C device

 Input:    byte to be transfered
 Return:   FALSE write successful
 TRUE write failed
 *************************************************************************/
bool ICACHE_FLASH_ATTR I2C_bl_Write_f(uint8_t data) {
	i2c_writeByte(data);
	if (!i2c_check_ack()) {
		i2c_stop();
		return TRUE;
	}
	return FALSE;

}/* i2c_write */

/*************************************************************************
 Read one byte from the I2C device, request more data from device

 Return:  byte read from I2C device
 *************************************************************************/
uint8_t ICACHE_FLASH_ATTR I2C_u8_ReadAck_f(void) {
	uint8_t ret = i2c_readByte();
	i2c_send_ack(1);
	return ret;
}/* i2c_readAck */

/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition

 Return:  byte read from I2C device
 *************************************************************************/
uint8_t ICACHE_FLASH_ATTR I2C_u8_ReadNak_f(void) {
	uint8_t ret = i2c_readByte();
	i2c_send_ack(0);
	return ret;
}/* i2c_readNak */

/*
 *===========================================================================*
 * End of File                                                         
 *===========================================================================*
 */
