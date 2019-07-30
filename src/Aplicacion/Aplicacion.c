#include <string.h>
#include "../Drivers/regsLPC1769.h"
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/Teclado.h"
#include "../Drivers/GPIO.h"
#include "../Drivers/Timer.h"
#include "../Drivers/LCD.h"
#include "../Drivers/RTC.h"
#include "../Drivers/varios.h"
#include "../Drivers/UART.h"
#include "../Drivers/ADC.h"
#include "../Drivers/power_management.h"
#include "../Drivers/digital_outputs.h"
#include "../Drivers/lpc_types.h"
#include "Aplicacion.h"

/* TODO:
	- Cuando no se esta mostrando el reloj,
	desactivar las interrupciones del RTC (segundos)
	- Opcion para reiniciar un Timer
*/

volatile uint32_t decimas_oled = 0;
static int8_t sw1_timerId = -1;
extern uint8_t ledStatus;
extern LCD_t LCD;
extern ADC_t adc;
extern uart_t uart0;



static menu_t menu = {
		.curr_level = 0,			/* 0: main menu ; 1: sub menu */
		.level[0].pos = 0,			/* position in main menu */
		.level[1].pos = 0,			/* option choosed in current menu*/
		.level[0].msg = msg_1,		/* "1- CHANGE OUTPUT" */
		.level[0].desc = desc_1,	/* "Turn the state of an output. Hold to enter." */
		.level[1].msg = msg_1_1,	/* "switch LED0" */
		.level[1].desc = desc_1_1,	/* "OFF" */

		.timerId = -1
};





void SysTick_Handler(void)		// systick cada 2,5ms
{
	static int cnt = 40;
	debounceTeclado();

	if(--cnt == 0) {
		cnt = 40;
		if(decimas_oled)
			decimas_oled--;
	}
}

void RIT_IRQHandler(void) // Repetitive Interrupt Timer cada 1ms
{
	LPC_RITIMER->CTRL |= RIT_CTRL_INT;
	LCD_send();
}


void ledBlink()
{
	static bool st = true;
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

void restoreScreen()
{
//	menu.timerId = -1;
	LCD_stopScroll();
	LCD.isInMenu = false;
	if(LCD.isInClock == true)
		LCD_displayClock();
	else
		LCD_clear();
	LCD.isOn = true;
	menu.level[0].pos = 0;
	menu.level[1].pos = 0;
	strcpy(menu.level[0].msg, msg_1);
	strcpy(menu.level[0].desc, desc_1);
}

void showClock()
{
	LCD.restore_timerId = -1;
	if(LCD.isInClock == false && LCD.isOn == true)
	{
		LCD_stopScroll();
		LCD_displayClock();
		LCD.isInClock = true;
	}
}

void showADC()
{
	if (adc.change)
	{
		adc.change = 0;
		LCD_printInt(adc.AD5_val, LCD_ROW_2, 5, 4);
		LCD_printInt(adc.AD5_val * 100 / 4095, LCD_ROW_2, 11, 3);
	}
	adc.timerId = startTimer(200, showADC);
}

void stopADC()
{
	stopTimer(&adc.timerId);
	ADC_stop();
}

void showMenu()
{
//	stopTimer(&menu.timerId);
	LCD_printCentered(menu.level[menu.curr_level].msg , LCD_ROW_1);
	LCD_scrollMessage(menu.level[menu.curr_level].desc, LCD_ROW_2);
//	menu.timerId = startTimer(20000, restoreScreen);
}



void enterMenu()
{
	sw1_timerId = -1;

	if(!LCD.isInMenu)
		return;

	if(menu.curr_level == 0)
	{
		LCD_clear();
		LCD_stopScroll();
		if(menu.level[0].pos == 0) // digital out
		{
			menu.level[1].pos = 0;
			strcpy(menu.level[1].msg, msg_1_1);
			if(D_OUT_getStatus(menu.level[1].pos))
				strcpy(menu.level[1].desc, desc_1_1);
			else
				strcpy(menu.level[1].desc, desc_1_2);
		}
		else if(menu.level[0].pos == 1) // adc read
		{
			ADC_start();
			strcpy(menu.level[1].msg, msg_2_1);
			strcpy(menu.level[1].desc, "");
			LCD_printDOWN("val:      (   %)");
			adc.change = 1;
			showADC();
		}
		else if(menu.level[0].pos == 2) // uart
		{
			strcpy(menu.level[1].msg, msg_3_1);
			if(uart0.status)
				strcpy(menu.level[1].desc, desc_3_1);
			else
				strcpy(menu.level[1].desc, desc_3_2);
		}

		menu.curr_level++;
	}

	else if(menu.curr_level == 1)
	{
		switch(menu.level[0].pos)
		{
		case 0: // DIGITAL
			D_OUT_toggle(menu.level[1].pos);
			if(D_OUT_getStatus(menu.level[1].pos))
				strcpy(menu.level[1].desc, desc_1_1);
			else
				strcpy(menu.level[1].desc, desc_1_2);
			break;
		case 1: // ADC
			break;
		case 2: // UART
			if(menu.level[1].pos == 0)
			{
				if(uart0.status)
					UART0_sendString("<F0RRo>\r\n");
			}
			else if(menu.level[1].pos == 1)
			{
				if(uart0.status)
					UART0_requestTime();
			}
			else if(menu.level[1].pos == 2)
			{
				if(uart0.status == ON) {
					UART0_setDown();
					strcpy(menu.level[1].msg, msg_3_3_2);
					strcpy(menu.level[1].desc, desc_3_2);
				}
				else {
					UART0_setUp();
					strcpy(menu.level[1].msg, msg_3_3_1);
					strcpy(menu.level[1].desc, desc_3_1);
				}
			}
			break;
		}
	}

	showMenu();
}

void SW1_handler(bool st)
{
	if(st) // presionado
	{
		sw1_timerId = startTimer(600, enterMenu);
	}
	else   // soltado
	{
		if(!isTimerEnd(sw1_timerId)) // si el timer está en curso: sw1="next"
		{
			stopTimer(&sw1_timerId);
			if(LCD.isInMenu)
			{
				uint8_t pos;
				if(menu.curr_level == 0)
				{
					pos = (menu.level[0].pos + 1) % 3;
					menu.level[0].pos = pos;

					if(pos == 0) {
						strcpy(menu.level[0].msg, msg_1);
						strcpy(menu.level[0].desc, desc_1);
					}
					else if(pos == 1) {
						strcpy(menu.level[0].msg, msg_2);
						strcpy(menu.level[0].desc, desc_2);
					}
					else if(pos == 2) {
						strcpy(menu.level[0].msg, msg_3);
						strcpy(menu.level[0].desc, desc_3);
					}
				}
				else if(menu.curr_level == 1)
				{
					switch(menu.level[0].pos)
					{
					case 0: // SALIDAS DIGITALES
						pos = (menu.level[1].pos + 1) % 4;
						menu.level[1].pos = pos;

						if(pos == 0)
							strcpy(menu.level[1].msg, msg_1_1);
						else if(pos == 1)
							strcpy(menu.level[1].msg, msg_1_2);
						else if(pos == 2)
							strcpy(menu.level[1].msg, msg_1_3);
						else if(pos == 3)
							strcpy(menu.level[1].msg, msg_1_4);

						if(D_OUT_getStatus(pos))
							strcpy(menu.level[1].desc, desc_1_1);
						else
							strcpy(menu.level[1].desc, desc_1_2);

						break;
					case 1: // ADC
						break;
					case 2: // UART
						pos = (menu.level[1].pos + 1) % 3;
						menu.level[1].pos = pos;

						if(pos == 0)
							strcpy(menu.level[1].msg, msg_3_1);
						else if(pos == 1)
							strcpy(menu.level[1].msg, msg_3_2);
						else if(pos == 2) {
							if(uart0.status)
								strcpy(menu.level[1].msg, msg_3_3_1);
							else
								strcpy(menu.level[1].msg, msg_3_3_2);
						}
						break;
					}
				}
			}
			showMenu();
			LCD.isInMenu = true;
			LCD.isInClock = false;
		}
	}
}

void SW2_handler(bool st)
{
	if(st)
	{
		LCD_stopScroll();
		if(!LCD.isInClock)
			LCD_clear();
		if(LCD.isInMenu)
		{
			if(menu.curr_level > 0)
			{
				if(menu.level[0].pos == 1) // adc
					stopADC();

				menu.level[menu.curr_level].pos = 0;
				menu.curr_level--;
				showMenu();
			}
			else
			{
				LCD.isInClock = true;
				restoreScreen();
			}
		}
		else if(!LCD.isInClock)
		{
			LCD.isInClock = true;
			restoreScreen();
		}
	}
}

void SW3_handler(bool st)
{
	if(st)
	{
		LCD_stopScroll();
		if(LCD.isInClock || LCD.isInMenu)
		{
			LCD.isOn = false;
			LCD.isInClock = false;
			LCD.isInMenu = false;
			if(menu.curr_level > 0 && menu.level[0].pos == 1)
				stopADC();
			menu.curr_level = 0;
			LCD_clear();
		}
		else
		{
			LCD.isOn = true;
			LCD.isInClock = true;
			LCD.isInMenu = false;
			LCD_displayClock();
		}
	}
}
/*
void SW3_handler(bool st)
{
	if(st)
	{
		LCD.isOn = false;
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
void SW4_handler(bool st) {
//	static PMU_MCUPOWER_T i = PMU_MCU_SLEEP;
	if(!st)
	{
		RTC_setAlarmInSeconds(10);
//		PMU_Sleep(i);
//		Power_Sleep();
//		if(i == PMU_MCU_DEEP_PWRDOWN)
//			i = PMU_MCU_SLEEP;
//		else
//			i++;
	}
}

#if _5_ENTRADAS
void SW5_handler(uint8_t st) {}
#endif



void defaultKeyHandler(bool st)
{
	flashLedLpc(LEDLPC_R);
}

void tramaRecibida(char *msg)
{
	int i = 0;
	switch(msg[i++])
	{
	// answer obtained
	case 'A':
		// time/date message
		if(msg[i] == 'T')
		{
			i++;
			// check 2 bytes year + 1 month + 1 day + 1 hour + 1 min + 1 sec = 7 bytes
			if(strlen(&msg[i]) == 7) {
				RTC_setTime_fromString(&msg[i]);
				RTC_setGPREG_fromTime();
			}

		}
	}

	// test de comunicacion
	if(!strncmp(msg, "PUT0", 4))
		UART0_sendString("<F0RRo>\r\n");
}
