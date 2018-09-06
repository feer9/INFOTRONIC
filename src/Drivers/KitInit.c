#include <Aplicacion.h>

void initPower		( void );
void initializarKit	( void );
void initGPIOs		( void );
void initExtInt		( void );
void initPLL		( void );
void initSysTick	( uint32_t );
void initTimer0		( uint32_t );
void initRTC		( void );

void inicializarKit ( void )
{
	initPLL();
	initPower();
	initTimer0(1000);
	initSysTick(2500);
	initRTC();
	initExtInt();
	initGPIOs();
	initLCD(0);
	UART0_init();
}

void initPower()
{
	PCONP = (0x01 << PCONP_RTC) | (0x01 << PCONP_GPIO);
//	PCONP =   0b00000000000000001000001000000000;
/*	solo dejo encendidos los perifericos:
	bit	9	RTC
	bit	15	GPIO  */
}

void initGPIOs ( void )
{
	set_dir(RGB_R, SALIDA);
	set_dir(RGB_G, SALIDA);
	set_dir(RGB_B, SALIDA);
	set_dir(LEDLPC_R, SALIDA);
	set_dir(LEDLPC_G, SALIDA);
	set_dir(LEDLPC_B, SALIDA);
	set_dir(LED0, SALIDA);
	set_dir(LED1, SALIDA);
	set_dir(LED2, SALIDA);
	set_dir(LED3, SALIDA);
	set_dir(LED4, SALIDA);

	write_pin(RGB_R, OFF);
	write_pin(RGB_G, OFF);
	write_pin(RGB_B, OFF);
	write_pin(LEDLPC_R, LEDLPC_OFF);
	write_pin(LEDLPC_G, LEDLPC_OFF);
	write_pin(LEDLPC_B, LEDLPC_OFF);
	write_pin(LED0, OFF);
	write_pin(LED1, OFF);
	write_pin(LED2, OFF);
	write_pin(LED3, OFF);
	write_pin(LED4, BUZZER_OFF);
}

