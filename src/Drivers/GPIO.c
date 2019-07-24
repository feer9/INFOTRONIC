#include "GPIO.h"


void setPINSEL(uint8_t port, uint8_t pin, uint8_t sel)
{
	uint32_t tmp;
	port = port * 2 + pin / 16;				//!< Calcula registro PINSEL
	pin = ( pin % 16 ) * 2;					//!< Calcula campo de bits
	tmp = PINSEL[ port ] & ~(3 << pin);		//!< Limpia campo de bits
	PINSEL[ port ] = tmp | (sel << pin);	//!< Set de bits en campo
}

void setPINMODE(uint8_t port, uint8_t pin, uint8_t modo)
{
	uint32_t tmp;
	port = port * 2 + pin / 16;				//!< Calcula registro PINMODE
	pin = ( pin % 16 ) * 2;					//!< Calcula campo de bits
	tmp = PINMODE[ port ] & ~(3 << pin);	//!< Limpia campo de bits
	PINMODE[ port ] = tmp | (modo << pin);	//!< Set de bits en campo
}

// clear registers when debugging
// (not cleared at software restart)
void PIN_init()
{
#ifdef DEBUG
	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 = 0;
	PINSEL3 = 0;
	PINSEL4 = 0;
	PINSEL7 = 0;
	PINSEL9 = 0;
	PINSEL10 = 0;

	PINMODE0 = 0;
	PINMODE1 = 0;
	PINMODE2 = 0;
	PINMODE3 = 0;
	PINMODE4 = 0;
	PINMODE7 = 0;
	PINMODE9 = 0;

	PINMODE_OD0 = 0;
	PINMODE_OD1 = 0;
	PINMODE_OD2 = 0;
	PINMODE_OD3 = 0;
	PINMODE_OD4 = 0;

	I2CPADCFG = 0;
#endif
}
