#include "i2c.h"

void I2C0_IRQHandler()
{
	if (I2C_IsMasterActive(I2C0)) {
		I2C_MasterStateHandler(I2C0);
	} else {
		I2C_SlaveStateHandler(I2C0);
	}
}

void I2C1_IRQHandler()
{
	if (I2C_IsMasterActive(I2C1)) {
		I2C_MasterStateHandler(I2C1);
	} else {
		I2C_SlaveStateHandler(I2C1);
	}
}

void I2C2_IRQHandler()
{
	if (I2C_IsMasterActive(I2C2)) {
		I2C_MasterStateHandler(I2C2);
	} else {
		I2C_SlaveStateHandler(I2C2);
	}
}


void I2C_Config(I2C_ID_T id, uint32_t clockrate, I2C_EVENTHANDLER_T event)
{
	if(id < 0 || id >= I2C_NUM_INTERFACE)
		return;

	I2C_Init(id);
	I2C_SetClockRate(id, clockrate);

	if (id == I2C0)
	{
		setPINSEL(SDA0, PINSEL_FUNC1);
		setPINSEL(SCL0, PINSEL_FUNC1);
	}
	else if (id == I2C1)
	{
		configurePin(SDA1, PINMODE_NONE, PINSEL_FUNC3);
		configurePin(SCL1, PINMODE_NONE, PINSEL_FUNC3);

		set_MODE_OD(SDA1, 1);
		set_MODE_OD(SCL1, 1);
	}
	else if(id == I2C2)
	{
		configurePin(SDA2, PINMODE_NONE, PINSEL_FUNC2);
		configurePin(SCL2, PINMODE_NONE, PINSEL_FUNC2);

		set_MODE_OD(SDA2, 1);
		set_MODE_OD(SCL2, 1);
	}

	I2C_SetMasterEventHandler(id, event);
	NVIC_ClearPendingIRQ(I2C_IRQn + (IRQn_Type) id);
	NVIC_EnableIRQ(I2C_IRQn + (IRQn_Type) id);
}

