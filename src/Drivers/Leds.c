#include <Aplicacion.h>

__RW uint8_t ledActual = 0;
__RW uint8_t ledStatus = ON;

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

void SetLED(int8_t led)
{
	write_pin(LED1, OFF);
	write_pin(LED2, OFF);
	write_pin(LED3, OFF);
	write_pin(LED4, OFF);

	switch(led)
	{
	case 0: write_pin(LED1, ON); break;
	case 1: write_pin(LED2, ON); break;
	case 2: write_pin(LED3, ON); break;
	case 3: write_pin(LED4, ON); break;
	default: break;
	}

}

void SetLEDActual()
{
	if(ledStatus)
		SetLED(ledActual);
}

void ToggleLED()
{
	if(ledStatus)
		ToggleLEDActual();
}

void ToggleLEDActual()
{
	switch(ledActual)
	{
	case 0: toggle_pin(LED1); break;
	case 1: toggle_pin(LED2); break;
	case 2: toggle_pin(LED3); break;
	case 3: toggle_pin(LED4); break;
	default: break;
	}
}
