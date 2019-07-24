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
	I2C_Init(id);
	I2C_SetClockRate(id, clockrate);

	if (id == I2C0)
	{
		setPINSEL(SDA0, PINSEL_FUNC1);
		setPINSEL(SCL0, PINSEL_FUNC1);
	}
	else if (id == I2C1)
	{
		setPINSEL(SDA1, PINSEL_FUNC3);
		setPINSEL(SCL1, PINSEL_FUNC3);

		setPINMODE(SDA1, PINMODE_NONE);
		setPINMODE(SCL1, PINMODE_NONE);

		set_MODE_OD(SDA1, 1);
		set_MODE_OD(SCL1, 1);
	}
	else if(id == I2C2)
	{
		setPINSEL(SDA2, PINSEL_FUNC2);
		setPINSEL(SCL2, PINSEL_FUNC2);

		setPINMODE(SDA2, PINMODE_NONE);
		setPINMODE(SCL2, PINMODE_NONE);

		set_MODE_OD(SDA2, 1);
		set_MODE_OD(SCL2, 1);
	}
	else
	{
		return;
	}

	I2C_SetMasterEventHandler(id, event);
	ISER0 = NVIC_I2C0 << (int)id; // really?...
}
