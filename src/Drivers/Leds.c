#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include "GPIO.h"
#include "Teclado.h"

/*
__RW uint8_t ledActual = 0;
__RW uint8_t ledStatus = OFF;*/

void D_IN_init( void )
{
	set_dir_output(RGB_R);
	set_dir_output(RGB_G);
	set_dir_output(RGB_B);
	set_dir_output(LEDLPC_R);
	set_dir_output(LEDLPC_G);
	set_dir_output(LEDLPC_B);
	set_dir_output(LED0);
	set_dir_output(LED1);
	set_dir_output(LED2);
	set_dir_output(LED3);
	set_dir_output(LED4);

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

	// turn off glitch filtering and slew rate control
	// for both P0.27 & P0.28 (I2C features)
	I2CPADCFG = (_BIT(1) | _BIT(3));
}

void D_IN_toggle(uint8_t n)
{
	switch (n)
	{
	case 0: toggle_pin(LED0); break;
	case 1: toggle_pin(LED1); break;
	case 2: toggle_pin(LED2); break;
	case 3: toggle_pin(LED3); break;
	case 4: toggle_pin(LED4); break;
	default: break;
	}
}

bool D_IN_getStatus(uint8_t n)
{
	switch (n)
	{
	case 0: return (bool) read(LED0);
	case 1: return (bool) read(LED1);
	case 2: return (bool) read(LED2);
	case 3: return (bool) read(LED3);
	case 4: return (bool) read(LED4);
	}
	return false;
}
/*
void ledOFF(void)
{
	ledStatus = OFF;
	SetLED(-1);
}

void ledON(void)
{
	ledStatus = ON;
	SetLED(ledActual);
}

void ledUP(void)
{
	if(ledStatus)
	{
		if(ledActual == 3)
			ledActual = -1;
		ledActual++;

		SetLED(ledActual);
	}
}

void ledDOWN(void)
{
	if(ledStatus)
	{
		if(!ledActual)
			ledActual = 4;
		ledActual--;

		SetLED(ledActual);
	}
}
*/
