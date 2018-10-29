#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include "GPIO.h"
#include "Teclado.h"

/*
__RW uint8_t ledActual = 0;
__RW uint8_t ledStatus = OFF;*/

void LEDs_init( void )
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

void toggleLed(uint8_t n)
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
