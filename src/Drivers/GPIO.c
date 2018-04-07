#include <TPO.h>


void SetPINSEL(uint8_t port, uint8_t pin, uint8_t sel)
{
	port = port * 2 + pin / 16;								//!< Calcula registro PINSEL
	pin = ( pin % 16 ) * 2;									//!< Calcula campo de bits
	PINSEL[ port ] = PINSEL[ port ] & ( ~ ( 3 << pin ) );	//!< Limpia campo de bits
	PINSEL[ port ] = PINSEL[ port ] | ( sel << pin );		//!< Set de bits en campo
}

void SetPINMODE(uint8_t port, uint8_t pin, uint8_t modo)
{
	port = port * 2 + pin / 16;								//!< Calcula registro PINMODE
	pin = ( pin % 16 ) * 2;									//!< Calcula campo de bits
	PINMODE[ port ] = PINMODE[ port ] & ( ~ ( 3 << pin ) );	//!< Limpia campo de bits
	PINMODE[ port ] = PINMODE[ port ] | ( modo << pin );	//!< Set de bits en campo
}

void SetMODE_OD (uint8_t port, uint8_t pin, uint8_t mode)
{
	mode ?	(PINMODE_OD [port] |=  (1 << pin)) : (PINMODE_OD [port] &= ~(1 << pin));
}

void SetDIR (uint8_t port, uint8_t pin, uint8_t dir)
{
	dir	?	(*(FIODIR + 8*port) |=  (1 << pin)) : (*(FIODIR + 8*port) &= ~(1 << pin)) ;
}

void SetPIN (uint8_t port, uint8_t pin, uint8_t state)
{
	state ? (*(FIOSET + 8*port) |= (1 << pin)) : (*(FIOCLR + 8*port) |= (1 << pin)) ;
}

uint8_t GetPIN (uint8_t port, uint8_t pin, uint8_t activ)
{
	return (((*(FIOPIN + 8*port) >> pin) & 0x01) == activ) ? 1 : 0 ;
}

void TooglePIN(uint8_t port, uint8_t pin)
{
	read(port,pin) ? write_pin(port,pin, 0) : write_pin(port,pin, 1) ;
}
