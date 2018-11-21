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
#include "../Drivers/ADC.h"
#include "Aplicacion.h"

/* TODO: añadir, opcion 2 del teclado: enviar por la uart0
 *       Al recibir, imprimir en pantalla "mensaje entrante uart0" o algo asi
 *       y abajo con desplazamiento imprimir el mensaje
 * */

int8_t s_timerId = -1, m_timerId = -1;
extern uint8_t ledStatus;
extern LCD_t LCD;
extern ADC_t adc;

menu_t menu = {
		.level = 0,
		.pos = {0,0},
		.op[0].msg = "CHANGE OUTPUT",
		.op[0].desc = "Turn the state of an output. Hold to enter.",
		.op[0].sub_op[0].msg = "switch LED0",
		.op[0].sub_op[1].msg = "switch LED1",
		.op[0].sub_op[2].msg = "switch LED2",
		.op[0].sub_op[3].msg = "switch LED3",
		.op[1].msg = "ADC Read",
		.op[1].desc = "get value of \"Ent Analog 0\". Hold to enter",
		.op[1].sub_op[0].msg = "AD0.5",
		.op[2].msg = "opt2",
		.op[2].desc = "opt2 desc. Hold to enter."

};

void showClock()
{
	if(LCD.isInClock == FALSE && LCD.isOn == TRUE)
	{
		LCD.scroll.isScrolling = FALSE;
		stopTimer(LCD.scroll.timerId);
		LCD.scroll.timerId = -1;
		LCD_displayClock();
		LCD.isInClock = TRUE;
	}
}

void showADC()
{
	if (adc.change)
	{
		adc.change = 0;
		LCD_printInt(adc.AD5_val, LCD_ROW_2, 4);
	}
	adc.timerId = startTimer(200, showADC);
}

void restoreScreen()
{
	LCD.scroll.isScrolling = FALSE;
	stopTimer(LCD.scroll.timerId);
	LCD.scroll.timerId = -1;
	LCD.isInMenu = FALSE;
	if(LCD.isInClock == TRUE)
		LCD_displayClock();
	else
		LCD_clear();
	LCD.isOn = TRUE;
	menu.pos[0] = 0;
	menu.pos[1] = 0;
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
		write_pin(LEDLPC_G, LEDLPC_ON);
		startTimer(100, ledBlink);
	}
	st = !st;
}

void showMenu()
{
	//stopTimer(s_timerId);
	if(menu.level == 0)
	{
		LCD_printCentered(menu.op[menu.pos[0]].msg , LCD_ROW_1);
		LCD_scrollMessage(menu.op[menu.pos[0]].desc, LCD_ROW_2);
	}
	else if(menu.level == 1)
	{
		LCD.scroll.isScrolling = FALSE;
		stopTimer(LCD.scroll.timerId);
		LCD.scroll.timerId = -1;
		if(menu.pos[0] != 1)
			LCD_clear();
		LCD_printCentered(menu.op[menu.pos[0]].sub_op[menu.pos[1]].msg , LCD_ROW_1);
	}

//	s_timerId = startTimer(20000, restoreScreen);
}

void enterMenu()
{
	if(menu.level == 0)
	{
		if(menu.pos[0] == 1)
		{
			ADC_start();
			showADC();
		}
		menu.level++;
	}
	else if(menu.level == 1)
	{
		switch(menu.pos[0])
		{
		case 0:
			toggleLed(menu.pos[1]);
			break;
		case 1:
			break;
		}
	}
	showMenu();
}

void SW1_handler(uint8_t st)
{
	if(st) // presionado
	{
		m_timerId = startTimer(600, enterMenu);
	}
	else   // soltado
	{
		if(!isTimerEnd(m_timerId))
		{
			stopTimer(m_timerId);
			m_timerId = -1;
			if(LCD.isInMenu)
			{
				if(!(menu.level == 1 && menu.pos[0] == 1)) // entro al adc
				{
					menu.pos[menu.level]++;
					if(menu.level == 0)
						menu.pos[menu.level] %= 3;
					else if(menu.level == 1)
						menu.pos[menu.level] %= 4;
				}
			}
			showMenu();
		}
		LCD.isInMenu = TRUE;
		LCD.isInClock = FALSE;
	}
}

void SW2_handler(uint8_t st)
{
	if(st)
	{
		if(LCD.isInMenu)
		{
			if(menu.level>0)
			{
				if(menu.pos[0] == 1) // adc
				{
					stopTimer(adc.timerId);
					adc.timerId = -1;
					ADC_stop();
				}
				menu.level--;
				showMenu();
			}
			else
			{
				restoreScreen();
			}
		}
	}
}

void SW3_handler(uint8_t st)
{
	if(st)
	{
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
	}
}
/*
void SW3_handler(uint8_t st)
{
	if(st)
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
}
*/
void SW4_handler(uint8_t st) {}

#if _5_ENTRADAS
void SW5_handler(uint8_t st) {}
#endif


