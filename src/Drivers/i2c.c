#include "i2c.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* Control flags */
#define I2C_CON_FLAGS (I2C_CON_AA | I2C_CON_SI | I2C_CON_STO | I2C_CON_STA)
#define LPC_I2Cx(id)      ((i2c.ip))
#define SLAVE_ACTIVE(iic) (((iic)->flags & 0xFF00) != 0)

static const PCLKSEL_T I2C_PeriphClk[I2C_NUM_INTERFACE] = {
		PCLKSEL_I2C0,
		PCLKSEL_I2C1,
		PCLKSEL_I2C2
};

/* I2C common interface structure */
struct i2c_interface {
	LPC_I2C_T *ip;						/* IP base address of the I2C device */
	uint32_t  clk;						/* Clock used by I2C */
	I2C_EVENTHANDLER_T mEvent;			/* Current active Master event handler */
	I2C_EVENTHANDLER_T sEvent;			/* Slave transfer events */
	I2C_XFER_T *mXfer;					/* Current active xfer pointer */
	I2C_XFER_T *sXfer;					/* Pointer to store xfer when bus is busy */
	uint32_t flags;						/* Flags used by I2C master and slave */
	uint8_t     *pendingTxBuff;			/*  */
	__RW uint16_t txBuffCount;			/*  */
	__RW uint16_t txBuff_indexIn;		/*  */
	__RW uint16_t txBuff_indexOut;		/*  */
	__RW uint8_t  pending;
	__RW uint8_t  pendingList_indexIn;	/*  */
	__RW uint8_t  pendingList_indexOut;	/*  */
	I2C_XFER_T mXferList[I2C_PENDING_LIST_SZ];	/*  */
};

/* Slave interface structure */
struct i2c_slave_interface {
	I2C_XFER_T *xfer;
	I2C_EVENTHANDLER_T event;
};

__DATA(RamAHB32) static uint8_t g_i2c_txBuff[2048];

/* I2C interfaces */
// since I only use one i2c interface, why waste my memory?
__DATA(RamAHB32) static struct i2c_interface i2c = /*[I2C_NUM_INTERFACE] = {*/
/*	{LPC_I2C0, PCONP_I2C0, I2C_EventHandler, NULL, NULL, NULL, 0, NULL,            0,0,0,0,0,0},*/
	{LPC_I2C1, PCONP_I2C1, I2C_EventHandler, NULL, NULL, NULL, 0, g_i2c_txBuff,    0,0,0,0,0,0};//,
/*	{LPC_I2C2, PCONP_I2C2, I2C_EventHandler, NULL, NULL, NULL, 0, NULL,            0,0,0,0,0,0}*/
//};


static struct i2c_slave_interface i2c_slave[I2C_NUM_INTERFACE][I2C_SLAVE_NUM_INTERFACE] = {0};

static volatile uint32_t i2c_busy_timeouts = 0;
static const uint32_t i2c_busy_limit = 999999; // adapt value to hardware setup and I2C clock rate

// debug
static uint32_t i2c_overflows = 0;
static uint32_t i2c_max_pendings = 0;
static uint32_t i2c_max_buffer_filled = 0;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void critical_section(I2C_ID_T id, bool st)
{
	static bool intStat[I2C_NUM_INTERFACE] = {0};

	if(st){	// enter critical section
		intStat[id] = NVIC_IRQStatus(I2C_IRQn + (IRQn_Type) id);
		if(intStat[id]) {
			NVIC_DisableIRQ(I2C_IRQn + (IRQn_Type) id);
		}
	}
	else {	// exit critical section
		if(intStat[id]) {
			NVIC_EnableIRQ(I2C_IRQn + (IRQn_Type) id);
		}
	}
}

static inline void enableClk(I2C_ID_T id)
{
	PCONP |= i2c.clk;
}

static inline void disableClk(I2C_ID_T id)
{
	PCONP &= ~i2c.clk;
}

/* Get the ADC Clock Rate */
static inline uint32_t getClkRate(I2C_ID_T id)
{
	return SystemCoreClock / getPCLKDiv( I2C_PeriphClk[id] );
}

/* Enable I2C and start master transfer */
static inline void startMasterXfer(LPC_I2C_T *pI2C)
{
	/* Reset STA, STO, SI */
	pI2C->CONCLR = I2C_CON_SI | I2C_CON_STO | I2C_CON_STA | I2C_CON_AA;

	/* Enter to Master Transmitter mode */
	pI2C->CONSET = I2C_CON_I2EN | I2C_CON_STA;
}

/* Enable I2C and enable slave transfers */
static inline void startSlaverXfer(LPC_I2C_T *pI2C)
{
	/* Reset STA, STO, SI */
	pI2C->CONCLR = I2C_CON_SI | I2C_CON_STO | I2C_CON_STA;

	/* Enter to Master Transmitter mode */
	pI2C->CONSET = I2C_CON_I2EN | I2C_CON_AA;
}

/* Check if I2C bus is free */
static inline int isI2CBusFree(LPC_I2C_T *pI2C)
{
	return !(pI2C->CONSET & I2C_CON_STO);
}

/* Get current state of the I2C peripheral */
static inline int getCurState(LPC_I2C_T *pI2C)
{
	return (int) (pI2C->STAT & I2C_STAT_CODE_BITMASK);
}

/* Check if the active state belongs to master mode*/
static inline int isMasterState(LPC_I2C_T *pI2C)
{
	return getCurState(pI2C) < 0x60;
}

/* Set OWN slave address for specific slave ID */
static void setSlaveAddr(LPC_I2C_T *pI2C, I2C_SLAVE_ID sid, uint8_t addr, uint8_t mask)
{
	uint32_t index = (uint32_t) sid - 1;
	pI2C->MASK[index] = mask;
	if (sid == I2C_SLAVE_0) {
		pI2C->ADR0 = addr;
	}
	else {
		volatile uint32_t *abase = &pI2C->ADR1;
		abase[index - 1] = addr;
	}
}

/* Match the slave address */
static int isSlaveAddrMatching(uint8_t addr1, uint8_t addr2, uint8_t mask)
{
	mask |= 1;
	return (addr1 & ~mask) == (addr2 & ~mask);
}

/* Get the index of the active slave */
static I2C_SLAVE_ID lookupSlaveIndex(LPC_I2C_T *pI2C, uint8_t slaveAddr)
{
	if (!(slaveAddr >> 1)) {
		return I2C_SLAVE_GENERAL;					/* General call address */
	}
	if (isSlaveAddrMatching(pI2C->ADR0, slaveAddr, pI2C->MASK[0])) {
		return I2C_SLAVE_0;
	}
	if (isSlaveAddrMatching(pI2C->ADR1, slaveAddr, pI2C->MASK[1])) {
		return I2C_SLAVE_1;
	}
	if (isSlaveAddrMatching(pI2C->ADR2, slaveAddr, pI2C->MASK[2])) {
		return I2C_SLAVE_2;
	}
	if (isSlaveAddrMatching(pI2C->ADR3, slaveAddr, pI2C->MASK[3])) {
		return I2C_SLAVE_3;
	}

	/* If everything is fine the code should never come here */
	return I2C_SLAVE_GENERAL;
}

/* Master transfer state change handler handler */
static int handleMasterXferState(LPC_I2C_T *pI2C, I2C_XFER_T  *xfer)
{
	uint32_t cclr = I2C_CON_FLAGS;
	int curState = getCurState(pI2C);

	if(xfer->ignoreNAK && (curState == 0x20 || curState == 0x30 || curState == 0x48))
		curState -= 0x08;

	switch (curState) {
	case 0x08:		/* Start condition on bus */
		if (xfer->status == I2C_STATUS_START) {
			xfer->status = I2C_STATUS_BUSY;
		}
	case 0x10:		/* Repeated start condition */
		pI2C->DAT = (xfer->slaveAddr << 1) | (xfer->txSz == 0);
		break;

	/* Tx handling */
	case 0x18:		/* SLA+W sent and ACK received */
	case 0x28:		/* DATA sent and ACK received */

		if (!xfer->txSz || !xfer->txBuff) {
			cclr &= ~(xfer->rxSz ? I2C_CON_STA : I2C_CON_STO);
		}
		else {
			pI2C->DAT = *xfer->txBuff++;
			if(xfer->txBuff == I2C_BUF_TX_TOP) xfer->txBuff = g_i2c_txBuff;
			xfer->txSz--;
		}
		break;

	/* Rx handling */
	case 0x58:		/* Data Received and NACK sent */
		cclr &= ~I2C_CON_STO;

	case 0x50:		/* Data Received and ACK sent */
		if(xfer->rxSz) {
			*xfer->rxBuff++ = pI2C->DAT;
			xfer->rxSz--;
		}

	case 0x40:		/* SLA+R sent and ACK received */
		if (xfer->rxSz > 1) {
			cclr &= ~I2C_CON_AA;
		}
		break;

	/* NAK Handling */
	case 0x20:		/* SLA+W sent NAK received */
	case 0x30:		/* DATA sent NAK received */
	case 0x48:		/* SLA+R sent NAK received */
		xfer->status = I2C_STATUS_NAK;
		cclr &= ~I2C_CON_STO;
		break;

	case 0x38:		/* Arbitration lost */
		xfer->status = I2C_STATUS_ARBLOST;
		break;

	/* Bus Error */
	case 0x00:
		xfer->status = I2C_STATUS_BUSERR;
		cclr &= ~I2C_CON_STO;
	}

	/* Set clear control flags */
	pI2C->CONSET = cclr ^ I2C_CON_FLAGS;
	pI2C->CONCLR = cclr;

	/* If stopped return 0 */
	if (!(cclr & I2C_CON_STO) || (xfer->status == I2C_STATUS_ARBLOST)) {
		if (xfer->status == I2C_STATUS_BUSY) {
			xfer->status = I2C_STATUS_DONE;
		}
		return 0;
	}
	return 1;
}

/* Find the slave address of SLA+W or SLA+R */
static I2C_SLAVE_ID getSlaveIndex(LPC_I2C_T *pI2C)
{
	switch (getCurState(pI2C)) {
	case 0x60:
	case 0x68:
	case 0x70:
	case 0x78:
	case 0xA8:
	case 0xB0:
		return lookupSlaveIndex(pI2C, pI2C->DAT);
	}

	/* If everything is fine code should never come here */
	return I2C_SLAVE_GENERAL;
}

/* Slave state machine handler */
static int handleSlaveXferState(LPC_I2C_T *pI2C, I2C_XFER_T *xfer)
{
	uint32_t cclr = I2C_CON_FLAGS;
	int ret = RET_SLAVE_BUSY;

	xfer->status = I2C_STATUS_BUSY;
	switch (getCurState(pI2C)) {
	case 0x80:		/* SLA: Data received + ACK sent */
	case 0x90:		/* GC: Data received + ACK sent */
		*xfer->rxBuff++ = pI2C->DAT;
		xfer->rxSz--;
		ret = RET_SLAVE_RX;
		if (xfer->rxSz > 1) {
			cclr &= ~I2C_CON_AA;
		}
		break;

	case 0x60:		/* Own SLA+W received */
	case 0x68:		/* Own SLA+W received after losing arbitration */
	case 0x70:		/* GC+W received */
	case 0x78:		/* GC+W received after losing arbitration */
		xfer->slaveAddr = pI2C->DAT & ~1;
		if (xfer->rxSz > 1) {
			cclr &= ~I2C_CON_AA;
		}
		break;

	case 0xA8:		/* SLA+R received */
	case 0xB0:		/* SLA+R received after losing arbitration */
		xfer->slaveAddr = pI2C->DAT & ~1;

	case 0xB8:		/* DATA sent and ACK received */
		pI2C->DAT = *xfer->txBuff++;
		xfer->txSz--;
		if (xfer->txSz > 0) {
			cclr &= ~I2C_CON_AA;
		}
		ret = RET_SLAVE_TX;
		break;

	case 0xC0:		/* Data transmitted and NAK received */
	case 0xC8:		/* Last data transmitted and ACK received */
	case 0x88:		/* SLA: Data received + NAK sent */
	case 0x98:		/* GC: Data received + NAK sent */
	case 0xA0:		/* STOP/Repeated START condition received */
		ret = RET_SLAVE_IDLE;
		cclr &= ~I2C_CON_AA;
		xfer->status = I2C_STATUS_DONE;
		if (xfer->slaveAddr & 1) {
			cclr &= ~I2C_CON_STA;
		}
		break;
	}

	/* Set clear control flags */
	pI2C->CONSET = cclr ^ I2C_CON_FLAGS;
	pI2C->CONCLR = cclr;

	return ret;
}

static void appendToBuffer(uint8_t *buffer, uint16_t index, const uint8_t *data, uint16_t len)
{
	if(index + len < I2C_BUF_TX_SZ) {
		memcpy(&buffer[index], data, len);
	}
	else {
		size_t tmp = I2C_BUF_TX_SZ - index;
		memcpy(&buffer[index], data, tmp);
		memcpy(&buffer[0], data +  tmp, len - tmp);
	}
}

static void updateIndexes(I2C_ID_T id, uint16_t txBuff_indexIn, uint16_t txBuffBytes)
{
	struct i2c_interface *iic = &i2c;

	// ENTER CRITICAL SECTION
	critical_section(id, true);

	// update indexs
	iic->txBuff_indexIn = txBuff_indexIn;
	iic->pendingList_indexIn = (iic->pendingList_indexIn + 1) % I2C_PENDING_LIST_SZ;
	iic->txBuffCount += txBuffBytes;
	i2c_max_buffer_filled = MAX(i2c_max_buffer_filled, iic->txBuffCount); // debug

	// EXIT CRITICAL SECTION
	critical_section(id, false);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initializes the LPC_I2C peripheral with specified parameter */
void I2C_Init(I2C_ID_T id)
{
	enableClk(id);

	/* Set I2C operation to default */
	LPC_I2Cx(id)->CONCLR = (I2C_CON_AA | I2C_CON_SI | I2C_CON_STA | I2C_CON_I2EN);
}

/* De-initializes the I2C peripheral registers to their default reset values */
void I2C_DeInit(I2C_ID_T id)
{
	/* Disable I2C control */
	LPC_I2Cx(id)->CONCLR = I2C_CON_I2EN | I2C_CON_SI | I2C_CON_STO | I2C_CON_STA | I2C_CON_AA;

	disableClk(id);
}

/* Set up clock rate for LPC_I2C peripheral */
void I2C_SetClockRate(I2C_ID_T id, uint32_t clockrate)
{
	uint32_t SCLValue;

	SCLValue = (getClkRate(id) / clockrate);
	LPC_I2Cx(id)->SCLH = (uint32_t) (SCLValue >> 1);
	LPC_I2Cx(id)->SCLL = (uint32_t) (SCLValue - LPC_I2Cx(id)->SCLH);
}

/* Get current clock rate for LPC_I2C peripheral */
uint32_t I2C_GetClockRate(I2C_ID_T id)
{
	return getClkRate(id) / (LPC_I2Cx(id)->SCLH + LPC_I2Cx(id)->SCLL);
}

/* Set the master event handler */
int I2C_SetMasterEventHandler(I2C_ID_T id, I2C_EVENTHANDLER_T event)
{
	struct i2c_interface *iic = &i2c;
	if (!iic->mXfer) {
		iic->mEvent = event;
	}
	return iic->mEvent == event;
}

/* Get the master event handler */
I2C_EVENTHANDLER_T I2C_GetMasterEventHandler(I2C_ID_T id)
{
	return i2c.mEvent;
}

void I2C_EventHandler(I2C_ID_T id, I2C_EVENT_T event)
{
	struct i2c_interface *iic = &i2c;
	volatile I2C_STATUS_T *stat = &iic->mXfer->status;
	uint32_t busy_counter = 0;
	I2C_XFER_T *xfer = iic->mXfer;

	switch(event)
	{
	case I2C_EVENT_WAIT:		/**< I2C Wait event */
		/* Wait for the status to change */
		while (*stat == I2C_STATUS_START || *stat == I2C_STATUS_BUSY)
		{
			if (busy_counter > i2c_busy_limit)
			{
				// I2C bus hang-up identified...
				i2c_busy_timeouts++;
				*stat = I2C_STATUS_BUSERR;
				// set stop bit for forced access to the I2C bus
				iic->ip->CONSET = I2C_CON_STO;
			}

			if((!iic->mXfer->polling) && (*stat != I2C_STATUS_START))
				break;

			// Interrupt based transfers must pass through here since
			// IRQ won't be called until MasterTransfer() returns
			if (I2C_IsStateChanged(id))
				I2C_MasterStateHandler(id);

			busy_counter++;
		}
		break;

	case I2C_EVENT_DONE:		/**< Done event that wakes up Wait event */
		if (iic->mXfer->status != I2C_STATUS_BUSERR) {
			/* Wait for stop condition to appear on bus */
			while (!isI2CBusFree(iic->ip)) {}
		}

		// if interrupt was disabled by polling transfer, reenable it
		if(iic->mXfer->polling) {
			NVIC_ClearPendingIRQ(I2C_IRQn + (IRQn_Type) id);
			NVIC_EnableIRQ(I2C_IRQn + (IRQn_Type) id);
		}
		else {
			i2c_max_pendings = MAX(i2c_max_pendings, iic->pending); // debug
			iic->pending--;
			iic->txBuff_indexOut = (iic->txBuff_indexOut + iic->mXfer->txTotal) % I2C_BUF_TX_SZ;
			iic->txBuffCount -= iic->mXfer->txTotal;
			iic->pendingList_indexOut = (iic->pendingList_indexOut + 1) % I2C_PENDING_LIST_SZ;
		}

		if(iic->pending) {
			iic->mXfer = &iic->mXferList[iic->pendingList_indexOut];
			iic->mXfer->status = I2C_STATUS_START;
			startMasterXfer(iic->ip);
			iic->mEvent(id, I2C_EVENT_WAIT);
		}
		else {
			/* Start slave if one is active */
			if (SLAVE_ACTIVE(iic)) {
				startSlaverXfer(iic->ip);
			}
			iic->mEvent(id, I2C_EVENT_UNLOCK);
		}

		if(xfer->cb)
			xfer->cb(xfer);
		break;

	case I2C_EVENT_LOCK:		/**< Re-entrency lock event for I2C transfer */
		iic->flags |= 1;
		break;

	case I2C_EVENT_UNLOCK:		/**< Re-entrency unlock event for I2C transfer */
		iic->mXfer = 0;
		iic->flags &= ~1;
		break;

	default:
		break;
	}
}

// TODO: implement option to copy or not tx buffer
/* Transmit and Receive data in master mode */
int I2C_MasterTransfer(I2C_ID_T id, I2C_XFER_T *xfer)
{
	struct i2c_interface *iic = &i2c;
	xfer->txTotal = xfer->txSz;

	if ( ( iic->pending == I2C_PENDING_LIST_SZ ) ||
			(iic->txBuffCount + xfer->txSz > I2C_BUF_TX_SZ ) ) {
		xfer->status = I2C_STATUS_BUF_FULL;
		i2c_overflows++; // debug
	}
	else
	{
		// save status to avoid changes by any interrupt without disabling them
		uint16_t txBuff_indexIn = iic->txBuff_indexIn;
		uint8_t pendingList_indexIn = iic->pendingList_indexIn;
		iic->pending++;

		if(!(iic->flags & 0x01)) {// if not locked
			xfer->status = I2C_STATUS_START;
			iic->mEvent(id, I2C_EVENT_LOCK);
		}
		else {
			xfer->status = I2C_STATUS_SCHEDULED;
		}

		if(xfer->txSz) {
			// copy buffers
			appendToBuffer(iic->pendingTxBuff, txBuff_indexIn, xfer->txBuff, xfer->txSz);
			xfer->txBuff = &iic->pendingTxBuff[txBuff_indexIn];
			txBuff_indexIn = (iic->txBuff_indexIn + xfer->txSz) % I2C_BUF_TX_SZ;
		}
		memcpy(&iic->mXferList[pendingList_indexIn], xfer, sizeof (I2C_XFER_T));

		if(xfer->status == I2C_STATUS_START) {
			iic->mXfer = &iic->mXferList[pendingList_indexIn];
			/* If slave xfer not in progress */
			if (!iic->sXfer) {
				startMasterXfer(iic->ip);
			}
			iic->mEvent(id, I2C_EVENT_WAIT);
		}

		updateIndexes(id, txBuff_indexIn, xfer->txTotal);
	}

	return (int) xfer->status;
}

// transfer a command followed by the content of xfer (R/W)
int I2C_MasterCmdTransfer(I2C_ID_T id, const uint8_t *cmd, uint16_t cmdLen, I2C_XFER_T *xfer)
{
	struct i2c_interface *iic = &i2c;

	if ( ( iic->pending == I2C_PENDING_LIST_SZ ) ||
			(iic->txBuffCount + xfer->txSz + cmdLen > I2C_BUF_TX_SZ ) ) {
		xfer->status = I2C_STATUS_BUF_FULL;
		i2c_overflows++; // debug
	}
	else
	{
		// save status to avoid changes by any interrupt without disabling them
		uint16_t txBuff_indexIn = iic->txBuff_indexIn;
		uint16_t bindex_aux = txBuff_indexIn;
		uint8_t pendingList_indexIn = iic->pendingList_indexIn;
		iic->pending++;

		if(!(iic->flags & 0x01)) {// if not locked
			xfer->status = I2C_STATUS_START;
			iic->mEvent(id, I2C_EVENT_LOCK);
		}
		else {
			xfer->status = I2C_STATUS_SCHEDULED;
		}

		// copy buffers
		appendToBuffer(iic->pendingTxBuff, bindex_aux, cmd, cmdLen);
		bindex_aux = (bindex_aux + cmdLen) % I2C_BUF_TX_SZ;
		if(xfer->txSz) {
			appendToBuffer(iic->pendingTxBuff, bindex_aux, xfer->txBuff, xfer->txSz);
			bindex_aux = (bindex_aux + xfer->txSz) % I2C_BUF_TX_SZ;
		}
		xfer->txBuff = &iic->pendingTxBuff[txBuff_indexIn];
		xfer->txSz = xfer->txTotal = (xfer->txSz + cmdLen);
		memcpy(&iic->mXferList[pendingList_indexIn], xfer, sizeof (I2C_XFER_T));

		if(xfer->status == I2C_STATUS_START) {
			iic->mXfer = &iic->mXferList[pendingList_indexIn];
			/* If slave xfer not in progress */
			if (!iic->sXfer) {
				startMasterXfer(iic->ip);
			}
			iic->mEvent(id, I2C_EVENT_WAIT);
		}

		updateIndexes(id, bindex_aux, xfer->txTotal);
	}

	return (int) xfer->status;
}

// Send command followed by some data
int I2C_MasterSendCmdData(I2C_ID_T id, uint8_t slaveAddr, const uint8_t *cmd, uint16_t cmdLen,
							const uint8_t *data, uint16_t dataLen, i2c_cb_t callback)
{
	struct i2c_interface *iic = &i2c;
	I2C_STATUS_T status = I2C_STATUS_START;

	if ( ( iic->pending == I2C_PENDING_LIST_SZ ) ||
			(iic->txBuffCount + cmdLen + dataLen > I2C_BUF_TX_SZ ) ) {
		status = I2C_STATUS_BUF_FULL;
		i2c_overflows++; // debug
	}
	else
	{
		// save status to avoid changes by any interrupt without disabling them
		uint16_t txBuff_indexIn = iic->txBuff_indexIn;
		uint16_t bindex_aux = txBuff_indexIn;
		uint8_t pendingList_indexIn = iic->pendingList_indexIn;
		iic->pending++;

		if(!(iic->flags & 0x01)) {// if not locked
			iic->mEvent(id, I2C_EVENT_LOCK);
		}
		else {
			status = I2C_STATUS_SCHEDULED;
		}

		// copy buffers
		appendToBuffer(iic->pendingTxBuff, bindex_aux, cmd, cmdLen);
		bindex_aux = (bindex_aux + cmdLen) % I2C_BUF_TX_SZ;

		appendToBuffer(iic->pendingTxBuff, bindex_aux, data, dataLen);
		bindex_aux = (bindex_aux + dataLen) % I2C_BUF_TX_SZ;

		I2C_XFER_T *xfer = &iic->mXferList[pendingList_indexIn];

		xfer->slaveAddr = slaveAddr;
		xfer->txBuff = &iic->pendingTxBuff[txBuff_indexIn];
		xfer->txSz = xfer->txTotal = (cmdLen + dataLen);

		xfer->rxBuff = NULL;
		xfer->rxSz = 0;

		xfer->cb = callback;
		xfer->status = status;
		xfer->polling = false;
		xfer->ignoreNAK = false;

		if(status == I2C_STATUS_START) {
			iic->mXfer = xfer;
			/* If slave xfer not in progress */
			if (!iic->sXfer) {
				startMasterXfer(iic->ip);
			}
			iic->mEvent(id, I2C_EVENT_WAIT);
		}

		updateIndexes(id, bindex_aux, xfer->txTotal);
	}

	return (int) status;
}

// Send 'len' bytes from 'buff'
int I2C_MasterSend(I2C_ID_T id, uint8_t slaveAddr, const uint8_t *buff, uint16_t len, i2c_cb_t callback)
{
	struct i2c_interface *iic = &i2c;
	I2C_STATUS_T status = I2C_STATUS_START;

	if ( ( iic->pending == I2C_PENDING_LIST_SZ ) ||
			(iic->txBuffCount + len > I2C_BUF_TX_SZ ) ) {
		status = I2C_STATUS_BUF_FULL;
		i2c_overflows++; // debug
	}
	else
	{
		// save status to avoid changes by any interrupt without disabling them
		uint16_t txBuff_indexIn = iic->txBuff_indexIn;
		uint8_t pendingList_indexIn = iic->pendingList_indexIn;
		iic->pending++;

		if(!(iic->flags & 0x01)) {// if not locked
			iic->mEvent(id, I2C_EVENT_LOCK);
		}
		else {
			status = I2C_STATUS_SCHEDULED;
		}

		// copy buffer
		appendToBuffer(iic->pendingTxBuff, txBuff_indexIn, buff, len);

		I2C_XFER_T *xfer = &iic->mXferList[pendingList_indexIn];

		xfer->slaveAddr = slaveAddr;
		xfer->txBuff = &iic->pendingTxBuff[txBuff_indexIn];
		xfer->txSz = xfer->txTotal = len;

		xfer->rxBuff = NULL;
		xfer->rxSz = 0;

		xfer->cb = callback;
		xfer->status = status;
		xfer->polling = false;
		xfer->ignoreNAK = false;

		if(status == I2C_STATUS_START) {
			iic->mXfer = xfer;
			/* If slave xfer not in progress */
			if (!iic->sXfer) {
				startMasterXfer(iic->ip);
			}
			iic->mEvent(id, I2C_EVENT_WAIT);
		}

		txBuff_indexIn = (txBuff_indexIn + len) % I2C_BUF_TX_SZ;
		updateIndexes(id, txBuff_indexIn, xfer->txTotal);
	}

	return (int) status;
}

/* Transmit and Receive data in master mode */
int I2C_MasterTransferPolling(I2C_ID_T id, I2C_XFER_T *xfer)
{
	struct i2c_interface *iic = &i2c;

	xfer->polling = true;
	/* Wait for free interface */
	while(iic->flags & 0x01) {}

	iic->mEvent(id, I2C_EVENT_LOCK);

	/* Wait for stop condition to appear on bus */
	while (!isI2CBusFree(iic->ip)) {}

	NVIC_DisableIRQ(I2C_IRQn + (IRQn_Type) id);
	iic->mXfer = xfer;
	iic->mXfer->status = I2C_STATUS_START;

	/* If slave xfer not in progress */
	if (!iic->sXfer) {
		startMasterXfer(iic->ip);
	}
	iic->mEvent(id, I2C_EVENT_WAIT);

	return (int) xfer->status;
}

/* Master tx only */
int I2C_MasterSendPolling(I2C_ID_T id, uint8_t slaveAddr, const uint8_t *buff, uint16_t len)
{
	I2C_XFER_T xfer = {0};
	xfer.slaveAddr = slaveAddr;
	xfer.txBuff = buff;
	xfer.txSz = len;
	while (I2C_MasterTransferPolling(id, &xfer) == I2C_STATUS_ARBLOST) {}
	return len - xfer.txSz;
}

/* Transmit one byte and receive an array of bytes after a repeated start condition is generated in Master mode.
 * This function is useful for communicating with the I2C slave registers
 */
int I2C_MasterCmdReadPolling(I2C_ID_T id, uint8_t slaveAddr, uint8_t cmd, uint8_t *buff, int len)
{
	I2C_XFER_T xfer = {0};
	xfer.slaveAddr = slaveAddr;
	xfer.txBuff = &cmd;
	xfer.txSz = 1;
	xfer.rxBuff = buff;
	xfer.rxSz = len;
	while (I2C_MasterTransferPolling(id, &xfer) == I2C_STATUS_ARBLOST) {}
	return len - xfer.rxSz;
}

int I2C_MasterCmd2ReadPolling(I2C_ID_T id, uint8_t slaveAddr, uint8_t *cmdBuff, int cmdLen,
										uint8_t *rxBuff, int rxLen)
{
	I2C_XFER_T xfer = {0};
	xfer.slaveAddr = slaveAddr;
	xfer.txBuff = cmdBuff;
	xfer.txSz = cmdLen;
	xfer.rxBuff = rxBuff;
	xfer.rxSz = rxLen;
	while (I2C_MasterTransferPolling(id, &xfer) == I2C_STATUS_ARBLOST) {}
	return rxLen - xfer.rxSz;
}

/* Sequential master read */
int I2C_MasterReadPolling(I2C_ID_T id, uint8_t slaveAddr, uint8_t *buff, int len)
{
	I2C_XFER_T xfer = {0};
	xfer.slaveAddr = slaveAddr;
	xfer.rxBuff = buff;
	xfer.rxSz = len;
	while (I2C_MasterTransferPolling(id, &xfer) == I2C_STATUS_ARBLOST) {}
	return len - xfer.rxSz;
}

/* Check if master state is active */
int I2C_IsMasterActive(I2C_ID_T id)
{
	return isMasterState(i2c.ip);
}

/* State change handler for master transfer */
void I2C_MasterStateHandler(I2C_ID_T id)
{
	if (!i2c.mXfer) {
		// Sometimes (e.g., when I2C is blocked intermittently)
		// something (e.g., Chip_I2C_MasterTransfer())
		// clears i2c.mXfer. In this case don't call
		// handleMasterXferState() as it dereferences mXfer
		// and this leads to a hard fault.
		// Clear interrupt bit to stop isr from being called repeatedly.
		LPC_I2Cx(id)->CONCLR = I2C_CON_SI | I2C_CON_AA | I2C_CON_STA;
	}
	else if (handleMasterXferState(i2c.ip, i2c.mXfer) == 0) // if xfer ended
	{
		i2c.mEvent(id, I2C_EVENT_DONE);
	}
}

/* Setup slave function */
void I2C_SlaveSetup(I2C_ID_T id,
					I2C_SLAVE_ID sid,
					I2C_XFER_T *xfer,
					I2C_EVENTHANDLER_T event,
					uint8_t addrMask)
{
	struct i2c_interface *iic = &i2c;
	struct i2c_slave_interface *si2c = &i2c_slave[id][sid];
	si2c->xfer = xfer;
	si2c->event = event;

	/* Set up the slave address */
	if (sid != I2C_SLAVE_GENERAL) {
		setSlaveAddr(iic->ip, sid, xfer->slaveAddr, addrMask);
	}

	if (!SLAVE_ACTIVE(iic) && !iic->mXfer) {
		startSlaverXfer(iic->ip);
	}
	iic->flags |= 1 << (sid + 8);
}

/* I2C Slave event handler */
void I2C_SlaveStateHandler(I2C_ID_T id)
{
	int ret;
	struct i2c_interface *iic = &i2c;

	/* Get the currently addressed slave */
	if (!iic->sXfer) {
		struct i2c_slave_interface *si2c;

		I2C_SLAVE_ID sid = getSlaveIndex(iic->ip);
		si2c = &i2c_slave[id][sid];
		if(!si2c->xfer || !si2c->event)
			return;
		iic->sXfer = si2c->xfer;
		iic->sEvent = si2c->event;
	}

	iic->sXfer->slaveAddr |= iic->mXfer != 0;
	ret = handleSlaveXferState(iic->ip, iic->sXfer);
	if (ret) {
		if (iic->sXfer->status == I2C_STATUS_DONE) {
			iic->sXfer = NULL;
		}
		iic->sEvent(id, (I2C_EVENT_T) ret);
	}
}

/* Disable I2C device */
void I2C_Disable(I2C_ID_T id)
{
	LPC_I2Cx(id)->CONCLR = I2C_I2CONCLR_I2ENC;
}

/* State change checking */
int I2C_IsStateChanged(I2C_ID_T id)
{
	return (LPC_I2Cx(id)->CONSET & I2C_CON_SI) != 0;
}
