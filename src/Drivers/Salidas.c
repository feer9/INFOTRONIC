#include <Aplicacion.h>

__RW uint8_t ledActual = 0;

void ledUP(void)
{
	if(ledActual == 3)
		SetLED(0);
	else
		SetLED(ledActual + 1);
}

void ledDOWN(void)
{
	if(!ledActual)
		SetLED(3);
	else
		SetLED(ledActual - 1);
}

void SetLED(uint8_t led)
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
	ledActual = led;
}

void SetLEDActual()
{
	SetLED(ledActual);
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
