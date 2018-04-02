#include <TPO.h>

void InicPower		( void );
void InicializarKit	( void );
void InicLED		( void );
void InicExtInt		( void );
void InicPLL		( void );
void InicSysTick	( void );
void InicTimer0		( void );

//uint32_t* pconp =(uint32_t*) 0x400FC0C4UL;

void InicializarKit ( void )
{
	InicPLL();
	InicPower();
	InicTimer0();
	InicSysTick();
	InicExtInt();
	InicLED();
	SetLED(0);
}

void InicPower()
{
	PCONP &= ~0b00000100001010000000010111011110;
	/*	apago los siguientes perifericos:
	bit	1	timer0
		2	timer1
		3	UART0
		4	UART1
		6	PWM1
		7	I2C0
		8	SPI
		10	SSP1
		19	I2C1
		21	SSP0
		26	I2C2 */
}
