#include <Aplicacion.h>

void initPower		( void );
void initializarKit	( void );
void initGPIOs		( void );
void initExtInt		( void );
void initPLL		( void );
void initSysTick	( void );
void initTimer0		( uint32_t ); // prescaler * 10ns
void initRTC		( void );

void inicializarKit ( void )
{
	initPLL();
	initPower();
	initTimer0(100000);
	initSysTick();
	initRTC();
	initExtInt();
	initGPIOs();
	initLCD();
}

void initPower()
{
	PConP =   0b00000000000000001000001000000000;
/*	solo dejo encendidos los perifericos:
	bit	9	RTC
	bit	15	GPIO  */
}
