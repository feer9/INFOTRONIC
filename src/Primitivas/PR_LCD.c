#include "chip.h"
#include "../Drivers/varios.h"
#include "../Drivers/LCD.h"
#include "../Drivers/Timer.h"
#include "../Drivers/UART.h"
#include "../Aplicacion/Aplicacion.h"
#include <string.h>

static void makeLine (const char* src, char* dest, \
						uint8_t start, uint8_t len);

static lcd_t *lcd = NULL;

void set_lcd_pointer(lcd_t *lcdp) { lcd = lcdp; }

inline bool LCD_isOn      (void) { return lcd->isOn;      }
inline bool LCD_isInClock (void) { return lcd->isInClock; }
inline bool LCD_isInMenu  (void) { return lcd->isInMenu;  }

inline void LCD_setOff() { lcd->isOn = false; }
inline void LCD_setOn () { lcd->isOn = true ; }
inline void LCD_setClockOff() { lcd->isInClock = false; }
inline void LCD_setClockOn () { lcd->isInClock = true ; }
inline void LCD_showNothing() {
	LCD_stopScroll();
	lcd->isOn = true;
	lcd->isInClock = false;
	lcd->isInMenu = false;
}


void LCD_scrollMessage(const char* msg, uint8_t line)
{
	uint8_t len = (uint8_t) strlen(msg);

	LCD_stopScroll();
	if(len > 16)
	{
		strncpy((char*) lcd->scroll.string, msg, len);
		lcd->scroll.len = len;
		lcd->scroll.line = line;
		lcd->scroll.index = 0;
		lcd->scroll.isScrolling = true;

		LCD_scroll();
	}
	else
	{
		LCD_pushString(msg, line, 0);
	}
}

inline void LCD_stopScroll()
{
	if(lcd->scroll.isScrolling)
	{
		lcd->scroll.isScrolling = false;
		stopTimer(&lcd->scroll.timerId);
	//	pushLCD( 0x06 , LCD_CONTROL);
	}
}

void LCD_print(const char* msg)
{
	char line1[17];
	char line2[17];
	size_t len = strlen(msg);

	if(len <= 16)
	{
		LCD_pushString(msg, LCD_ROW_1, 0);
	}
	else
	{
		extract_substr(msg, line1, 0, 15);

		if(len > 32)
			len = 32;

		extract_substr(msg, line2, 16, len);

		LCD_pushString(line1, LCD_ROW_1, 0);
		LCD_pushString(line2, LCD_ROW_2, 0);
	}
}

void LCD_printUP(const char* msg)
{
	char line[17];
	makeLine(msg, line, 0, (uint8_t) strlen(msg));
	LCD_pushLine(line, LCD_ROW_1);
}

void LCD_printDOWN(const char* msg)
{
	char line[17];
	makeLine(msg, line, 0, (uint8_t) strlen(msg));
	LCD_pushLine(line, LCD_ROW_2);
}

void LCD_printCentered(const char* msg, uint8_t row)
{
	uint8_t len = (uint8_t) strlen(msg);
	char line[17];
	makeLine(msg, line, (uint8_t) ((16 - len) / 2), len);
	LCD_pushLine(line, row);
}

uint8_t LCD_pushString(const char* msg, uint8_t row, uint8_t pos)
{
	uint8_t i, err=0;

	if (row == LCD_ROW_1)
		err += pushLCD( pos + 0x80U , LCD_CONTROL );

	else
		err += pushLCD( pos + 0xC0U , LCD_CONTROL );


	for( i = 0 ; msg[i] != '\0' && pos+i < 16 ; i++ )
		err += pushLCD( msg[i] , LCD_DATA );

	return err;
}

uint8_t LCD_pushLine(const char* msg, uint8_t row)
{
	uint8_t i , err=0;

	if (row == LCD_ROW_1)
		err += pushLCD( 0x80U , LCD_CONTROL );

	else
		err += pushLCD( 0xC0U , LCD_CONTROL );


	for( i = 0 ; i < 16 ; i++ )
		err += pushLCD( msg [i] , LCD_DATA );

	return err;
}


void LCD_displayClock()
{
	LCD_printClockTemplate();
	LCD_updateClock();
}

void LCD_updateClock()
{
	char aux[5];

	LCD_pushString( intToStr((int) LPC_RTC->HOUR , aux, 2) , LCD_POS_HOUR );
	LCD_pushString( intToStr((int) LPC_RTC->MIN  , aux, 2) , LCD_POS_MIN  );
	LCD_pushString( intToStr((int) LPC_RTC->SEC  , aux, 2) , LCD_POS_SEC  );
	LCD_pushString( intToStr((int) LPC_RTC->YEAR , aux, 4) , LCD_POS_YEAR );
	LCD_pushString( intToStr((int) LPC_RTC->MONTH, aux, 2) , LCD_POS_MONTH);
	LCD_pushString( intToStr((int) LPC_RTC->DOM  , aux, 2) , LCD_POS_DAY  );
}


void LCD_printReceived(const char* msg)
{
	lcd->isOn = true;
	lcd->isInClock = false;
	lcd->isInMenu = false;
	if(lcd->scroll.isScrolling) {
		lcd->scroll.isScrolling = false;
		stopTimer(&lcd->scroll.timerId);
	}
	LCD_clear();
	LCD_printCentered("UART0 Received:", LCD_ROW_1);
	LCD_scrollMessage(msg, LCD_ROW_2);

	if(lcd->restore_timerId >= 0)
		stopTimer(&lcd->restore_timerId);
	startTimer(&lcd->restore_timerId, 10000,showClock);
}

// copia src en dest, empezando en start, y llenando con espacios
static void makeLine(const char* src, char* dest, uint8_t start, uint8_t len)
{
	uint8_t i;

	for( i = 0 ; i < 16 ; i++ )
	{
		if(i < start)
			dest[i] = ' ';
		else if(i < len + start)
			dest[i] = src[i - start];
		else
			dest[i] = ' ';
	}
	dest[i] = '\0';
}

void LCD_printInt(int num, uint8_t row, uint8_t pos, uint8_t digits)
{
	char num_str[17];
	intToStr(num, num_str, digits);
	LCD_pushString(num_str, row, pos);
}

void LCD_WelcomeMessage(void)
{
	lcd->isOn = true;
	LCD_printCentered("WELCOME", LCD_ROW_1);

	if(LPC_RTC->GPREG2 == 0)
		startTimer(NULL, 5000, UART0_requestTime);

	LCD_printInt(++LPC_RTC->GPREG2, LCD_ROW_2, 6, 3);
}
