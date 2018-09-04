
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
	LCD_printCentered("WELCOME",0);
	startTimer(1, 1000); // reloj en 1s
	while(1)
	{
		Teclado();
	}
}

extern __RW uint8_t tick;
extern __RW uint8_t g_Teclado;
extern __RW uint8_t ledStatus;
uint8_t blink = OFF;
uint8_t displayClockStatus = OFF;
uint8_t LCD_status = ON;

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
			LCD_displayClock();
			displayClockStatus = ON;
		}
		break;
	case 0:
		LCD_clear();
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
	displayClockStatus = OFF;
	switch(key)
	{
	case SW1:
		ledON();
		if(LCD_status)
		{
			LCD_clear();
			LCD_print("Led ON");
			startTimer(1, 1000);
		}
		break;

	case SW2:
		ledOFF();
		if(LCD_status)
		{
			LCD_clear();
			LCD_print("Led OFF");
			startTimer(1, 1000);
		}
		break;

	case SW3:
		if(ledStatus)
		{
			ledUP();
			if(LCD_status)
			{
				LCD_clear();
				LCD_printCentered("Swap", LCD_ROW_1);
				startTimer(1, 2000);
			}
		}
		break;

	case SW4:
		if(LCD_status)
		{
			LCD_OFF();
			LCD_status = OFF;
		}
		else
		{
			LCD_clear();
			LCD_ON();
			LCD_status = ON;
			displayClockStatus = ON;
			LCD_displayClock();
		}
		break;

#if _5_ENTRADAS
	case SW5:
		if(!blink) {
			blink = ON;
			tick = 0;
			LCD_printCentered("BLINK ON", LCD_ROW_2);
		}
		else {
			blink = OFF;
			SetLEDActual();
			LCD_printCentered("BLINK OFF", LCD_ROW_2);
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
