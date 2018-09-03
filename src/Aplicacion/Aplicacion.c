
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <Aplicacion.h>



/* TODO: mejorar el driver del LCD para que procese strings con desplazamiento
 **/

int main(void)
{
	inicializarKit();
	ledOFF();
	setearTimers();
	LCD_clear();
	LCD_printCentered("WELCOME",0);
	startTimer(1, 1000); // reloj en 1s
	while(1)
	{
		Teclado();
	}
}

extern __RW uint8_t tick;
extern __RW uint8_t g_Teclado;
uint8_t blink = OFF;
uint8_t displayClockStatus = OFF;

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

	case 1:	// led apagado, encender display de reloj
		if(displayClockStatus == OFF)
		{
			displayClockStatus = ON;
			displayClock();
		}
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
		displayClockStatus = OFF;
		LCD_print("LED:  OFF");
		startTimer(1, 2000);
		break;

	case SW2:
		ledON();
		displayClockStatus = OFF;
		LCD_printUP("LED:  ON");
		if(blink)
			LCD_printCentered("BLINK ON", RENGLON_2);
		else
			LCD_printCentered("BLINK OFF", RENGLON_2);
		startTimer(1, 4000);
		break;

	case SW3:
		ledUP();
		displayClockStatus = OFF;
		LCD_printCentered("LED  UP", RENGLON_1);
		LCD_printDOWN("");
		startTimer(1, 4000);
		break;

	case SW4:
		ledDOWN();
		displayClockStatus = OFF;
		LCD_printCentered("LED  DOWN", RENGLON_1);
		LCD_printDOWN("");
		startTimer(1, 4000);
		break;

#if _5_ENTRADAS
	case SW5:
		displayClockStatus = OFF;
		if(!blink) {
			blink = ON;
			tick = 0;
			LCD_printCentered("BLINK ON", RENGLON_2);
		}
		else {
			blink = OFF;
			SetLEDActual();
			LCD_printCentered("BLINK OFF", RENGLON_2);
		}
		LCD_printUP("");
		startTimer(1, 1000);
		break;
#endif
	default:
		break;
	}
}

void releasedKey(uint8_t key)
{

}
