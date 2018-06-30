
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <Aplicacion.h>

void setearTimers	( void );

/* TODO: tengo q hacer un driver piola para el lcd, que le tire
 * un string largo y lo procese con desplazamiento, y si le meto
 * un string chico que complete con espacios en blanco, media pila!
 * deberia guardar en una variable lo que hay en pantalla,
 * asi si quiero cambiar una parte sola puedo mandar tod0 de nuevo
 **/

int main(void)
{
	inicializarKit();
	SetLED(0);
	setearTimers();
	displayLCD("    WELCOME     ", 0, 0);
	displayLCD("                ", 1, 0);
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
		ToggleLED();
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
	startTimer(6, 5000);
	startTimer(7, 4900);
}




void pressedKey(uint8_t key)
{
	switch(key)
	{
	case SW1:
		ledOFF();
		displayLCD("LED:  OFF       ", 0, 0);
		displayLCD("                ", 1, 0);
		break;

	case SW2:
		ledON();
		displayLCD("LED:  ON        ", 0, 0);
		break;

	case SW3:
		ledUP();
		displayLCD("LED:         UP ", 0, 0);
		break;

	case SW4:
		ledDOWN();
		displayLCD("LED:        DOWN", 0, 0);
		break;

#if _5_ENTRADAS
	case SW5:
		if(!blink) {
			blink = ON;
			tick = 0;
			displayLCD("    BLINK ON    ", 1, 0);
		}
		else {
			blink = OFF;
			SetLEDActual();
			displayLCD("    BLINK OFF   ", 1, 0);
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
