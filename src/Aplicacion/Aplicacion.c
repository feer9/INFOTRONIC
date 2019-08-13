#include "Aplicacion.h"
#include <string.h>
#include "../Drivers/lpc_types.h"
#include "../Drivers/regsLPC1769.h"
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/oled_ssd1306.h"
#include "../Drivers/LCD.h"
#include "../Drivers/Teclado.h"
#include "../Drivers/GPIO.h"
#include "../Drivers/Timer.h"
#include "../Drivers/RTC.h"
#include "../Drivers/varios.h"
#include "../Drivers/UART.h"
#include "../Drivers/ADC.h"
#include "../Drivers/power_management.h"
#include "../Drivers/digital_outputs.h"
#include "../Drivers/24LC256.h"
#include "../vcom/cdc_vcom.h"
/* TODO:
	- Cuando no se esta mostrando el reloj,
	desactivar las interrupciones del RTC (segundos)
*/

volatile uint32_t decimas_oled = 0;
static timer_id_t sw1_timerId = -1;

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

static lcd_t *lcd = NULL;
static u8g2_t *u8g2 = NULL;

void APP_setGlobalSymbols(lcd_t *lcdp, u8g2_t *u8g2p)
{
	lcd = lcdp;
	u8g2 = u8g2p;
}


void SysTick_Handler(void)	// systick cada 10ms
{
	static int cnt = 10;

	if(--cnt == 0) {
		cnt = 10;
		if(decimas_oled)
			decimas_oled--;
	}
}

void RIT_IRQHandler(void) // Repetitive Interrupt Timer cada 200us
{
	LPC_RITIMER->CTRL |= RIT_CTRL_INT;
	debounceTeclado();
	LCD_send();
}


static inline void ledLpcG_Off(void) {
	write_pin(LEDLPC_G, LEDLPC_OFF);
}

static void ledLpcG_Blink(void) {
	write_pin(LEDLPC_G, LEDLPC_ON);
	startTimer(NULL, 100, ledLpcG_Off);
}

void ledBlink(bool st)
{
	static timer_id_t tid = -1;
	if(st)
	{
		startTimer(&tid, 5000, ledLpcG_Blink);
		timerLoop(&tid, true);
	}
	else
	{
		stopTimer(&tid);
		ledLpcG_Off();
	}
}

void restoreScreen()
{
	LCD_stopScroll();
	lcd->isInMenu = false;
	if(lcd->isInClock == true)
		LCD_displayClock();
	else
		LCD_clear();
	lcd->isOn = true;
	menu.level[0].pos = 0;
	menu.level[1].pos = 0;
	strcpy(menu.level[0].msg, msg_1);
	strcpy(menu.level[0].desc, desc_1);
}

void showClock()
{
	if(lcd->isInClock == false && lcd->isOn == true)
	{
		LCD_stopScroll();
		LCD_displayClock();
		lcd->isInClock = true;
	}
}

volatile static int8_t adc_timerId;

void showADC()
{
	if (ADC_hasChanged())
	{
		ADC_clearChange();
		uint32_t val = ADC_getVal();
		LCD_printInt(val, LCD_ROW_2, 5, 4);
		LCD_printInt(val * 100 / 4095, LCD_ROW_2, 11, 3);
	}
	startTimer(&adc_timerId, 200, showADC);
}

void stopADC()
{
	stopTimer(&adc_timerId);
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
	if(!lcd->isInMenu)
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
			showADC();
		}
		else if(menu.level[0].pos == 2) // uart
		{
			strcpy(menu.level[1].msg, msg_3_1);
			if(UART0_isUp())
				strcpy(menu.level[1].desc, desc_3_1);
			else
				strcpy(menu.level[1].desc, desc_3_2);
		}
		else if(menu.level[0].pos == 3) // oled
		{
			if(ssd1306_getStatus() == ON)
				strcpy(menu.level[1].msg, msg_4_1);
			else
				strcpy(menu.level[1].msg, msg_4_2);
			strcpy(menu.level[1].desc, desc_4_1);
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
				if(UART0_isUp())
					UART0_sendString("<F0RRo>\r\n");
			}
			else if(menu.level[1].pos == 1)
			{
				if(UART0_isUp())
					UART0_requestTime();
			}
			else if(menu.level[1].pos == 2)
			{
				if(UART0_isUp() == ON) {
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
		case 3: // OLED
			if(ssd1306_getStatus() == OFF) {
				ssd1306_on();
				strcpy(menu.level[1].msg, msg_4_1);
			}
			else {
				ssd1306_off();
				strcpy(menu.level[1].msg, msg_4_2);
			}
		}
	}

	showMenu();
}

void SW1_handler(bool st)
{
	if(!lcd->isOn)
		toggle_on_lcd();

	if(st) // pushed
	{
		startTimer(&sw1_timerId, 600, enterMenu);
	}
	else   // released
	{
		uint8_t pos0 = menu.level[0].pos;
		uint8_t pos1 = menu.level[1].pos;
		if(!isTimerEnd(sw1_timerId))
		{
			stopTimer(&sw1_timerId);
			if(lcd->isInMenu)
			{
				if(menu.curr_level == 0)
				{
					pos0++;
					pos0 %= N_OPTIONS;

					if(pos0 == 0) {
						strcpy(menu.level[0].msg, msg_1);
						strcpy(menu.level[0].desc, desc_1);
					}
					else if(pos0 == 1) {
						strcpy(menu.level[0].msg, msg_2);
						strcpy(menu.level[0].desc, desc_2);
					}
					else if(pos0 == 2) {
						strcpy(menu.level[0].msg, msg_3);
						strcpy(menu.level[0].desc, desc_3);
					}
					else if(pos0 == 3) {
						strcpy(menu.level[0].msg, msg_4);
						strcpy(menu.level[0].desc, desc_4);
					}
				}
				else if(menu.curr_level == 1)
				{
					switch(pos0)
					{
					case 0: // SALIDAS DIGITALES
						pos1++;
						pos1 %= 4;

						if(pos1 == 0)
							strcpy(menu.level[1].msg, msg_1_1);
						else if(pos1 == 1)
							strcpy(menu.level[1].msg, msg_1_2);
						else if(pos1 == 2)
							strcpy(menu.level[1].msg, msg_1_3);
						else if(pos1 == 3)
							strcpy(menu.level[1].msg, msg_1_4);

						if(D_OUT_getStatus(pos1))
							strcpy(menu.level[1].desc, desc_1_1);
						else
							strcpy(menu.level[1].desc, desc_1_2);

						break;
					case 1: // ADC
						break;
					case 2: // UART
						pos1++;
						pos1 %= 3;

						if(pos1 == 0)
							strcpy(menu.level[1].msg, msg_3_1);
						else if(pos1 == 1)
							strcpy(menu.level[1].msg, msg_3_2);
						else if(pos1 == 2) {
							if(UART0_isUp())
								strcpy(menu.level[1].msg, msg_3_3_1);
							else
								strcpy(menu.level[1].msg, msg_3_3_2);
						}
						break;
					case 3: // OLED
						break;
					}
				}
			}
			menu.level[0].pos = pos0;
			menu.level[1].pos = pos1;
			showMenu();
			lcd->isInMenu = true;
			lcd->isInClock = false;
		}
	}
}

void SW2_handler(bool st)
{
	if(!lcd->isOn)
		toggle_on_lcd();

	if(st)
	{
		LCD_stopScroll();
		if(!lcd->isInClock)
			LCD_clear();
		if(lcd->isInMenu)
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
				lcd->isInClock = true;
				restoreScreen();
			}
		}
		else if(!lcd->isInClock)
		{
			lcd->isInClock = true;
			restoreScreen();
		}
	}
}

void toggle_on_lcd()
{
	LCD_stopScroll();
	lcd->isOn = true;
	lcd->isInClock = true;
	lcd->isInMenu = false;
	LCD_displayClock();
}

void toggle_off_lcd()
{
	LCD_stopScroll();
	lcd->isOn = false;
	lcd->isInClock = false;
	lcd->isInMenu = false;
	if(menu.curr_level > 0 && menu.level[0].pos == 1)
		stopADC();
	menu.curr_level = 0;
	LCD_clear();
}

static uint8_t g_eeprom_buf[512];

static void e2prom_rx_cb(status st) {
	bool rx_ok = true;
	LCD_showNothing();
	LCD_clear();
	if(st == SUCCESS) {
		for(int i=0; i< sizeof g_eeprom_buf; i++) {
			if(g_eeprom_buf[i] != (uint8_t) (i & 0x1F)) {
				rx_ok = false;
				break;
			}
		}
	}
	else {
		rx_ok = false;
	}
	if(rx_ok)
		LCD_scrollMessage("Successfully received 512 bytes from EEPROM", 0);
	else
		LCD_scrollMessage("Error while receiving from EEPROM", 0);
}

static void e2prom_tx_cb(status st) {
	LCD_showNothing();
	LCD_clear();
	if(st == SUCCESS) {
		LCD_scrollMessage("Successfully sent 512 bytes to EEPROM", 0);
	}
	else {
		LCD_scrollMessage("Error while sending to EEPROM", 0);
	}
}

void SW3_handler(bool st)
{
	if(!st)
	{
	//	toggle_off_lcd();
		for(int i=0; i< sizeof g_eeprom_buf; i++) {
			g_eeprom_buf[i] = (uint8_t) (i & 0x1F);
		}
		EEPROM_setCallback(e2prom_tx_cb);
		EEPROM_write(0x0000, g_eeprom_buf, sizeof g_eeprom_buf);
	}
}

void SW4_handler(bool st) {

//	static PMU_MCUPOWER_T i = PMU_MCU_SLEEP;
	if(!st)
	{
//		RTC_setAlarmInSeconds(10);
//		PMU_Sleep(i);
//		Power_Sleep();
//		if(i == PMU_MCU_DEEP_PWRDOWN)
//			i = PMU_MCU_SLEEP;
//		else
//			i++;
		memset(g_eeprom_buf, 0, sizeof g_eeprom_buf);
		EEPROM_setCallback(e2prom_rx_cb);
		EEPROM_read(0x0000, g_eeprom_buf, sizeof g_eeprom_buf);
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
