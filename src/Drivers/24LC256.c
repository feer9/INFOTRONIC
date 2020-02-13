/*
 * 24LC256.c
 *
 *  Created on: 3 ago. 2019
 *      Author: Feer
 */
#include "24LC256.h"

// 7 upper bits device address
#define EEPROM_ADDRESS 0x50
// top memory address; memory size
#define EEPROM_ADDRESS_TOP 0x08000

typedef struct {
	I2C_XFER_T *xfer;
	const uint8_t *txBuff;
	uint16_t txSz;
	uint16_t rxSz;
	uint16_t address;
	uint16_t naks;
	void (*callback) (status);
	bool ready;
	bool busy;
} eeprom_t;

static void     tx_callback(I2C_XFER_T *xfer);
static void     rx_callback(I2C_XFER_T *xfer);
static void status_callback(I2C_XFER_T *xfer);
static void      tx_handler(void);
static void      rx_handler(void);
static int            write(void);
static int             read(void);

static uint8_t curr_address[2];

static I2C_XFER_T g_eeprom_xfer = {
		.slaveAddr = EEPROM_ADDRESS
};


static eeprom_t eeprom = { &g_eeprom_xfer, NULL, 0,0,0,0, NULL, false, false };

static void test_status()
{
	I2C_MasterSend(I2C, EEPROM_ADDRESS, curr_address, 2, status_callback, 0);
}

static void status_callback(I2C_XFER_T *xfer)
{
	I2C_STATUS_T event = xfer->status;
	switch(event)
	{
	case I2C_STATUS_DONE:
		eeprom.ready = true;
		eeprom.naks = 0;
		tx_handler();
		rx_handler();
		break;
	case I2C_STATUS_NAK:
	default:
		eeprom.naks++;
		eeprom.ready = false;
		if(eeprom.naks < 10)
			startTimer(NULL, 1, test_status);
		else if(eeprom.callback)
			eeprom.callback(ERROR);
		break;
	}
}

static void tx_callback(I2C_XFER_T *xfer)
{
	I2C_STATUS_T event = xfer->status;
	int sent = xfer->sent ? ((xfer->sent - 2) - xfer->txDataSz) : 0;

	eeprom.txBuff += sent;
	eeprom.xfer->txDataBuff = eeprom.txBuff;
	// since xfer doesn't record the reference to the original tx buffer
	eeprom.txSz -= sent;
	eeprom.address = (eeprom.address + sent) % EEPROM_ADDRESS_TOP;

	switch(event)
	{
	case I2C_STATUS_DONE:

		tx_handler();
		break;

	case I2C_STATUS_NAK:		/**< NAK received during transfer */
	default:

		eeprom.naks++;
		eeprom.ready = false;
		startTimer(NULL, 1, test_status);
		break;
	}

	if(!eeprom.txSz && eeprom.callback)
		eeprom.callback((event == I2C_STATUS_DONE) ? SUCCESS : ERROR);
}

static void rx_callback(I2C_XFER_T *xfer)
{
	I2C_STATUS_T event = xfer->status;

	eeprom.xfer->rxBuff = xfer->rxBuff;
	eeprom.address = (eeprom.address + (eeprom.rxSz - xfer->rxSz)) % EEPROM_ADDRESS_TOP;
	eeprom.rxSz = xfer->rxSz;

	switch(event)
	{
	case I2C_STATUS_DONE:

		rx_handler();
		break;

	case I2C_STATUS_NAK:		/**< NAK received during transfer */
	default:

		eeprom.naks++;
		eeprom.ready = false;
		startTimer(NULL, 1, test_status);
		break;
	}

	if(!eeprom.rxSz && eeprom.callback)
		eeprom.callback((event == I2C_STATUS_DONE) ? SUCCESS : ERROR);
}

static void rx_handler(void)
{
	if(eeprom.rxSz) {

		if(eeprom.ready) {
			read();
		}
		else {
			startTimer(NULL, 2, test_status);
		}
	}
	else if(!eeprom.txSz) {
		eeprom.busy = false;
	}
}

static void tx_handler(void)
{
	if(eeprom.txSz) {

		if(eeprom.ready) {
			write();
			eeprom.ready = false;
		}
		else {
			startTimer(NULL, 2, test_status);
		}
	}
	else if(!eeprom.txSz) {
			eeprom.busy = false;
	}
}

static int read()
{
	curr_address[0] = (uint8_t) (eeprom.address >> 8);
	curr_address[1] = (uint8_t) (eeprom.address & 0x00FF);

	eeprom.xfer->txCmdBuff = curr_address;
	eeprom.xfer->txCmdSz = 2;
	eeprom.xfer->rxSz = eeprom.rxSz;
	eeprom.xfer->cb = rx_callback;

	return I2C_MasterTransfer(I2C, eeprom.xfer);
}

static int write()
{
	uint16_t sz = (eeprom.txSz > 64) ? 64 : eeprom.txSz;
	uint16_t max_sz = 64 - (eeprom.address % 64);
	if(sz > max_sz) sz = max_sz;

	curr_address[0] = (uint8_t) (eeprom.address >> 8);
	curr_address[1] = (uint8_t) (eeprom.address & 0x00FF);

	eeprom.xfer->txCmdBuff = curr_address;
	eeprom.xfer->txCmdSz = 2;
	eeprom.xfer->txDataSz = sz;
	eeprom.xfer->rxSz = 0;
	eeprom.xfer->cb = tx_callback;

	return I2C_MasterTransfer(I2C, eeprom.xfer);
}

/********** end of private code **********/

// todo: move public code to primitives, using three functions to fill
// eeprom struct, query eeprom busy status and fill callback.
// rx_handler() and tx_handler() will need to be non static. (change the name!)

status EEPROM_read(uint16_t address, uint8_t *buff, uint16_t sz)
{
	if(eeprom.busy || address >= EEPROM_ADDRESS_TOP)
		return ERROR;

	eeprom.busy = true;
	eeprom.rxSz = sz;
	eeprom.xfer->rxBuff = buff;
	eeprom.address = address;
	eeprom.naks = 0;

	rx_handler();
	return SUCCESS;
}

status EEPROM_write(uint16_t address, const uint8_t *buff, uint16_t sz)
{
	if(eeprom.busy || address >= EEPROM_ADDRESS_TOP)
		return ERROR;

	eeprom.busy = true;
	eeprom.txSz = sz;
	eeprom.txBuff =  eeprom.xfer->txDataBuff = buff;
	eeprom.address = address;
	eeprom.naks = 0;

	tx_handler();
	return SUCCESS;
}

status EEPROM_setCallback(void (*cb) (status))
{
	if(eeprom.busy)
		return ERROR;
	eeprom.callback = cb;
	return SUCCESS;
}
