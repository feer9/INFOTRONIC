#include <cr_section_macros.h>
#include "Aplicacion.h"

/* TODO: mejorar el driver del LCD para que procese strings con desplazamiento
 **/

void showClock(void);
void ledBlink(void);

extern __RW uint8_t ledStatus;
uint8_t displayClockStatus = OFF;
uint8_t LCD_status = ON;

void dummy(void){}

int main(void)
{
	kit_init();
	ledOFF();
	LCD_printCentered("WELCOME", LCD_ROW_1);
	ledBlink();
	startnTimer(7, 1000, showClock); // reloj en 1s
	while(1) {
		dummy();
	}
}

void showClock()
{
	if(displayClockStatus == OFF)
	{
		LCD_displayClock();
		displayClockStatus = ON;
	}
}

void ledBlink()
{
	static bool st = TRUE;
	if(st)
	{
		write_pin(LEDXpresso, LEDLPC_OFF);
		startTimer(4900, ledBlink);
	}
	else
	{
		startTimer(100, ledBlink);
		write_pin(LEDXpresso, LEDLPC_ON);
	}
	st = !st;
}

void pressedKey(uint8_t key)
{
	switch(key)
	{
	case SW1:
		ledON();
		if(LCD_status)
		{
			displayClockStatus = OFF;
			LCD_clear();
			LCD_print("Led ON");
			startnTimer(7, 1000, showClock);
		}
		break;

	case SW2:
		ledOFF();
		if(LCD_status)
		{
			displayClockStatus = OFF;
			LCD_clear();
			LCD_print("Led OFF");
			startnTimer(7, 1000, showClock);
		}
		break;

	case SW4:
		if(ledStatus)
		{
			ledUP();
			if(LCD_status)
			{
				displayClockStatus = OFF;
				LCD_clear();
				LCD_printCentered("Swap", LCD_ROW_1);
				startnTimer(7, 2000, showClock);
			}
		}
		else
		{
			int err = 0, msjs=0;
			char str[17] = "err: ";
			char aux[4];
			for(int i=0; i < BUFFER_TX_SIZE/2 + 1; i++) {
				err += UART0_sendChar(0x55);
				err += UART0_sendChar(0xAA);
				msjs += 2;
			}
			LCD_clear();
			LCD_printUP(strcat(str, intToStr(err, aux, 1)));
			strcpy(str,"buf:");
			strcat(str, intToStr(BUFFER_TX_SIZE, aux, 1));
			strcat(str," msjs:");
			strcat(str, intToStr(msjs, aux, 1));
			LCD_printDOWN(str);
		}
		break;

	case SW3:
		break;

#if _5_ENTRADAS
	case SW5:

		break;
#endif
	default:
		break;
	}
}

void releasedKey(uint8_t key)
{
	if(key == SW3)
	{
		if(displayClockStatus)
		{
			//LCD_OFF();
			LCD_status = OFF;
			displayClockStatus = OFF;
			LCD_clear();
		}
		else
		{
		//	LCD_ON();
			LCD_status = ON;
			displayClockStatus = ON;
			LCD_displayClock();
		}
	}
}
