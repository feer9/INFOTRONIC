
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

	}
}

extern __RW uint8_t tick;
extern __RW uint8_t ledStatus;
uint8_t blink = OFF;
uint8_t displayClockStatus = OFF;
uint8_t LCD_status = ON;

void Teclado()
{

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
		displayClockStatus = OFF;
		ledON();
		if(LCD_status)
		{
			LCD_clear();
			LCD_print("Led ON");
			startTimer(1, 1000);
		}
		break;

	case SW2:
		displayClockStatus = OFF;
		ledOFF();
		if(LCD_status)
		{
			LCD_clear();
			LCD_print("Led OFF");
			startTimer(1, 1000);
		}
		break;

	case SW3:
		displayClockStatus = OFF;
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
//		startTimer(0,5000);
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
	if(key == SW4)
	{
//		stopTimer(0);
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
	}
}
