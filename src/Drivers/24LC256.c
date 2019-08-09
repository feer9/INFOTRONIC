/*
 * 24LC256.c
 *
 *  Created on: 3 ago. 2019
 *      Author: Feer
 */
#include "24LC256.h"

// 7 upper bits address
#define EEPROM_ADDRESS 0x50

static uint8_t curr_address[2];
static uint8_t buff[66];
static int err = 0;

static I2C_XFER_T xfer = {
		EEPROM_ADDRESS,		/* slaveAddr */
		curr_address,		/* txBuff */
		0,					/* txSz */
		buff,				/* rxBuff */
		0,					/* rxSz */
		NULL,				/* cb */
		I2C_STATUS_DONE,	/* status */
		false,				/* polling */
		false           	/* ignoreNAK */
};

static void tx_callback(I2C_XFER_T *xfer)
{
	I2C_STATUS_T event = xfer->status;
	int ret;
	switch(event)
	{
	case I2C_STATUS_DONE:
		if(xfer->txSz == 0) { // t0do ok
			ret = vcom_write(&buff[2], 64);
			LCD_showNothing();
			LCD_clear();
			LCD_scrollMessage("Successfully sent 64 bytes to EEPROM", 0);
			LCD_pushString("wrote to usb: ", 1, 0);
			LCD_printInt(ret, 1, 14, 2);
			//startTimer(NULL, 5000, showClock);
		}
		else
			err++;
		break;
	case I2C_STATUS_NAK:		/**< NAK received during transfer */
	case I2C_STATUS_ARBLOST:	/**< Aribitration lost during transfer */
	case I2C_STATUS_BUSERR:		/**< Bus error in I2C transfer */
	case I2C_STATUS_BUSY:		/**< I2C is busy doing transfer */
	case I2C_STATUS_START:
	case I2C_STATUS_LOCKED:
		break;
	}
}

static void rx_callback(I2C_XFER_T *xfer)
{
	I2C_STATUS_T event = xfer->status;
	int ret;
	switch(event)
	{
	case I2C_STATUS_DONE:
		if((xfer->rxBuff == &buff[66]) && (xfer->rxSz == 0)) { // t0do ok
			ret = vcom_write(&buff[2], 64);
			LCD_showNothing();
			LCD_clear();
			LCD_printInt(buff[2+0], 0,  0, 2);
			LCD_printInt(buff[2+1], 0,  2, 2);
			LCD_printInt(buff[2+2], 0,  4, 2);
			LCD_printInt(buff[2+3], 0,  6, 2);
			LCD_printInt(buff[2+4], 0, 10, 2);
			LCD_printInt(buff[2+5], 0, 12, 2);
			LCD_printInt(buff[2+6], 0, 14, 2);
			LCD_pushString("wrote to usb: ", 1, 0);
			LCD_printInt(ret, 1, 14, 2);
			//startTimer(NULL, 5000, showClock);
		}
		else
			err++;
		break;
	case I2C_STATUS_NAK:		/**< NAK received during transfer */
	case I2C_STATUS_ARBLOST:	/**< Aribitration lost during transfer */
	case I2C_STATUS_BUSERR:		/**< Bus error in I2C transfer */
	case I2C_STATUS_BUSY:		/**< I2C is busy doing transfer */
	case I2C_STATUS_START:
	case I2C_STATUS_LOCKED:
		break;
	}
}

int EEPROM_read(uint16_t address)
{
	if(I2C_IsMasterActive(I2C))
		return -1;

//	if(!vcom_connected())
//		return -1;

	memset(buff, 0, sizeof buff);

	curr_address[0] = (uint8_t) (address >> 8);
	curr_address[1] = (uint8_t) (address & 0x0F);

	xfer.txBuff = curr_address;
	xfer.txSz = 2;
	xfer.rxBuff = &buff[2];
	xfer.rxSz = 64;
	xfer.cb = rx_callback;

	int ret;
	while ((ret = I2C_MasterTransfer(I2C, &xfer)) == I2C_STATUS_ARBLOST);

//	I2C_MasterCmd2Read(I2C, EEPROM_ADDRESS, curr_address, 2,
//									&buff[2], 64, rx_callback);

	return 0;
}


int EEPROM_write(uint16_t address, int cant)
{
	if(I2C_IsMasterActive(I2C) || cant > 64)
		return -1;

	buff[0] = (uint8_t) (address >> 8);
	buff[1] = (uint8_t) (address & 0x0F);

	xfer.txBuff = buff;
	xfer.txSz = 2 + cant;
	xfer.rxBuff = NULL;
	xfer.rxSz = 0;
	xfer.cb = tx_callback;

	int ret;
	while ((ret = I2C_MasterTransfer(I2C, &xfer)) == I2C_STATUS_ARBLOST);

//	I2C_MasterSend(I2C, EEPROM_ADDRESS, buff, 2+cant);

	return 0;
}

uint8_t *EEPROM_getBuff(void)
{
	return &buff[2];
}
