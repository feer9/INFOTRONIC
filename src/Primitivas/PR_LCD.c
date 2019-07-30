#include <string.h>
#include "../Drivers/varios.h"
#include "../Drivers/LCD.h"
#include "../Drivers/lpc_types.h"
#include "../Drivers/Timer.h"
#include "../Aplicacion/Aplicacion.h"

static void makeLine (const char* src, char* dest, \
						uint8_t start, uint8_t len);

extern LCD_t LCD;


void LCD_scrollMessage(const char* msg, uint8_t line)
{
	uint8_t len = (uint8_t) strlen(msg);

	LCD_stopScroll();
	if(len > 16)
	{
		strncpy((char*) LCD.scroll.string, msg, len);
		LCD.scroll.len = len;
		LCD.scroll.line = line;
		LCD.scroll.index = 0;
		LCD.scroll.isScrolling = true;

		LCD_scroll();
	}
	else
	{
		LCD_pushString(msg, line, 0);
	}
}

inline void LCD_stopScroll()
{
	if(LCD.scroll.isScrolling)
	{
		LCD.scroll.isScrolling = false;
		stopTimer(&LCD.scroll.timerId);
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
	LCD.isOn = true;
	LCD.isInClock = false;
	LCD.isInMenu = false;
	if(LCD.scroll.isScrolling) {
		LCD.scroll.isScrolling = false;
		stopTimer(&LCD.scroll.timerId);
	}
	LCD_clear();
	LCD_printCentered("UART0 Received:", LCD_ROW_1);
	LCD_scrollMessage(msg, LCD_ROW_2);

	if(LCD.restore_timerId >= 0)
		stopTimer(&LCD.restore_timerId);
	LCD.restore_timerId = startTimer(10000,showClock);
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
	LCD_printCentered("WELCOME", LCD_ROW_1);
	LCD_printInt(++LPC_RTC->GPREG2, LCD_ROW_2, 6, 3);
}
