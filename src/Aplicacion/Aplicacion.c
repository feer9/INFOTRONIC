
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <Aplicacion.h>

void setearTimers	( void );

int main(void)
{
	inicializarKit();
	SetLED(0);
	setearTimers();
	displayLCD("AGUANTE EL PACO ", 0, 0);
	while(1)
	{
		Teclado();
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



//extern __RW uint8_t flagTimerLCD;

/** boludeces del timer */
void timerEnd_Handler(uint8_t n)
{
	switch (n)
	{
	case 6:
		write_pin(LEDXpresso, LEDLPC_OFF);
		setearTimers();
		break;
	case 7:
		write_pin(LEDXpresso, LEDLPC_ON);
		break;

	default:
		break;
	}
}
void setearTimers()
{
	startTimer(6, 2000);
	startTimer(7, 1900);
}




void pressedKey(uint8_t key)
{
	switch(key)
	{
	case SW1:
		SetLED(0);
		displayLCD("uno dos uno dos ", 0, 0);
		displayLCD("uno dos uno dos ", 1, 0);
		break;

	case SW2:
		ledDOWN();
		displayLCD("   TARA DOOOU!  ", 0, 0);
		displayLCD("                ", 1, 0);
		break;

	case SW3:
		ledUP();
		displayLCD("AGUANTE EL PACO ", 0, 0);
		break;

	case SW4:
		SetLED(3);
		displayLCD("1234567890CaEeLi", 1, 0);
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
