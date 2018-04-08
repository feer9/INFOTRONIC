
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <TPO.h>

void setearTimers	( void );

int main(void)
{
	InicializarKit();
	setearTimers();

	while(1)
	{
		Teclado();
//		LCD();
	}
}

extern __RW uint8_t tick;
extern __RW uint8_t g_Teclado;
uint8_t blink = OFF;

void Teclado()
{
	if(tick && blink)
	{
		tick = 0;
		ToggleLEDActual();
	}
	if(g_Teclado)
	{
		pressedKey(g_Teclado);
		g_Teclado = 0;
	}
}



extern __RW uint8_t flagTimerLCD;

/** boludeces del timer */
void timerEnd_Handler(uint8_t n)
{
	switch (n)
	{
	case 6:
		write_pin(LEDXpresso, LEDLPC_OFF); setearTimers(); break;
	case 7:
		write_pin(LEDXpresso, LEDLPC_ON); break;

	case 0:
		flagTimerLCD |= 0x01; break;
	case 1:
		flagTimerLCD |= 0x02; break;

	default: break;
	}
}
void setearTimers()
{
	startTimer(6, 200000000);
	startTimer(7, 190000000);
}




void pressedKey(uint8_t key)
{
	switch(key)
	{
	case SW1:
		SetLED(0);
		break;

	case SW2:
		ledDOWN();
		break;

	case SW3:
		ledUP();
		break;

	case SW4:
		SetLED(3);
		break;

#if _5_ENTRADAS
	case SW5:
		if(!blink) {
			blink = ON;
			tick = 0;
		}
		else {
			blink = OFF;
			SetLEDActual();
		}
		break;
#endif
	default:
		break;
	}
}

void releasedKey(uint8_t key)
{

}
