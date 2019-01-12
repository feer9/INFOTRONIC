#include "GPIO.h"


void setPINSEL(uint8_t port, uint8_t pin, uint8_t sel)
{
	port = port * 2 + pin / 16;				//!< Calcula registro PINSEL
	pin = ( pin % 16 ) * 2;					//!< Calcula campo de bits
	PINSEL[ port ] &= ( ~ ( 3 << pin ) );	//!< Limpia campo de bits
	PINSEL[ port ] |= ( sel << pin );		//!< Set de bits en campo
}

void setPINMODE(uint8_t port, uint8_t pin, uint8_t modo)
{
	port = port * 2 + pin / 16;				//!< Calcula registro PINMODE
	pin = ( pin % 16 ) * 2;					//!< Calcula campo de bits
	PINMODE[ port ] &= ( ~ ( 3 << pin ) );	//!< Limpia campo de bits
	PINMODE[ port ] |= ( modo << pin );		//!< Set de bits en campo
}

inline void setMODE_OD (uint8_t port, uint8_t pin, uint8_t mode)
{
	mode ?	(PINMODE_OD[port] |=  (1UL << pin)) : (PINMODE_OD[port] &= ~(1UL << pin));
}

inline void setDIR (uint8_t port, uint8_t pin, uint8_t dir)
{
	dir	?	(FIODIR[8*port] |=  (1UL << pin)) : (FIODIR[8*port] &= ~(1UL << pin)) ;
}

inline void setPIN (uint8_t port, uint8_t pin, uint8_t state)
{
	state ? (FIOSET[8*port] |= (1UL << pin)) : (FIOCLR[8*port] |= (1UL << pin)) ;
}

inline uint8_t getPIN (uint8_t port, uint8_t pin, uint8_t activ)
{
	return (((FIOPIN[8*port] >> pin) & 1UL) == activ) ? 1 : 0 ;
}

inline void tooglePIN(uint8_t port, uint8_t pin)
{
	FIOPIN[8*port] ^= (1UL << pin) ;
}
