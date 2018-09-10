#include "../Drivers/regsLPC1769.h"
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/Teclado.h"
#include "../Drivers/GPIO.h"
#include "../Drivers/Timer.h"
#include "../Drivers/LCD.h"
#include "../Drivers/RTC.h"
#include "../Drivers/types.h"
#include "../Drivers/string.h"
#include "../Drivers/varios.h"
#include "../Drivers/UART.h"
#include "Aplicacion.h"


extern uint8_t ledStatus;
extern LCD_t LCD;

void showClock()
{
	if(LCD.isInClock == FALSE && LCD.isOn == TRUE)
	{
		LCD.scroll.isScrolling = FALSE;
		LCD_displayClock();
		LCD.isInClock = TRUE;
	}
}

void restoreScreen()
{
	LCD.scroll.isScrolling = FALSE;
	if(LCD.isInClock == TRUE)
		LCD_displayClock();
	else
		LCD_clear();
	LCD.isOn = TRUE;
}

void ledBlink()
{
	static bool st = TRUE;
	if(st)
	{
		write_pin(LEDLPC_G, LEDLPC_OFF);
		startTimer(4900, ledBlink);
	}
	else
	{
		startTimer(100, ledBlink);
		write_pin(LEDLPC_G, LEDLPC_ON);
	}
	st = !st;
}

void pressedKey(uint8_t key)
{
	switch(key)
	{
	case SW1:
		if(ledStatus)
		{
			ledOFF();
			if(LCD.isOn)
			{
				LCD.isInClock = FALSE;
				LCD_clear();
				LCD_print("Led OFF");
				startnTimer(7, 1000, showClock);
			}
		}
		else
		{
			ledON();
			if(LCD.isOn)
			{
				LCD.isInClock = FALSE;
				LCD_clear();
				LCD_print("Led ON");
				startnTimer(7, 1000, showClock);
			}
		}

		break;

	case SW2:
		break;

	case SW3:
		if(ledStatus)
		{
			ledUP();
			if(LCD.isOn)
			{
				LCD.isInClock = FALSE;
				LCD_clear();
				LCD_printCentered("Swap LED", LCD_ROW_1);
				startnTimer(7, 2000, showClock);
			}
		}
		else
		{
			LCD.isOn = FALSE;
			LCD_clear();
			int err = 0, msjs=0;
			char str[17] = "send()   err:";
			char aux[4];
			for(int i=0; i < BUFFER_TX_SIZE/2; i++) {
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

			startTimer(8000, restoreScreen);
		}
		break;

	case SW4:
		break;

#if _5_ENTRADAS
	case SW5:

		break;
#endif
	}
}

void releasedKey(uint8_t key)
{
	switch(key)
	{
	case SW2:
		if(LCD.isInClock)
		{
			//LCD_OFF();
			LCD.isOn = FALSE;
			LCD.isInClock = FALSE;
			LCD_clear();
		}
		else
		{
		//	LCD_ON();
			LCD.isOn = TRUE;
			LCD.isInClock = TRUE;
			LCD_displayClock();
		}
		break;
	}
}
