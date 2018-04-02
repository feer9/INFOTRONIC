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

void SetDIR(uint8_t port, uint8_t pin, uint8_t dir )
{
	port = port * 8;									//!< Calcula registro FIODIR
	GPIOs[ port ] = GPIOs[ port ] & ( ~ ( 1 << pin ) );	//!< Limpia campo de bits
	GPIOs[ port ] = GPIOs[ port ] | ( dir << pin );		//!< Set de bits en campo
}

void SetPIN(uint8_t port, uint8_t pin, uint8_t estado )
{
	port = port * 8 + 5;									//!< Calcula registro FIOPIN
	GPIOs[ port ] = GPIOs[ port ] & ( ~ ( 1 << pin ) );		//!< Limpia campo de bits
	GPIOs[ port ] = GPIOs[ port ] | ( estado << pin );		//!< Set de bits en campo
}

uint8_t GetPIN(uint8_t port, uint8_t pin, uint8_t actividad )
{
	port = port * 8 + 5;							//!< Calcula registro FIOPIN
	return ( ( ( GPIOs[ port ] >> pin ) & 1 ) == actividad ) ? 1 : 0;	//!< Retorna estado de pin en relacion a la actividad
}

void SetMODE_OD( uint8_t port , uint8_t pin , uint8_t dir )
{
	PINMODE_OD[ port ] = PINMODE_OD[ port ] & ( ~ ( 1 << pin ) );	//!< Limpia campo de bits
	PINMODE_OD[ port ] = PINMODE_OD[ port ] | ( dir << pin );		//!< Set de bits en campo
}

void TooglePIN( uint8_t port , uint8_t pin)
{
	port = port * 8 + 5;			//!< Calcula registro FIOPIN
	
	if ( (GPIOs[ port ] >> pin) & 1 )
		GPIOs[ port ] &= ( ~ ( 1 << pin ) );	//!< Si vale 1, lo limpia
	else
		GPIOs[ port ] |= ( 1 << pin );			//!< Si vale 0, lo setea
}
