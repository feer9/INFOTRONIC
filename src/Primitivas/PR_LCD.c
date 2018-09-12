#include "../Drivers/LCD.h"
#include "../Drivers/types.h"
#include "../Drivers/string.h"
#include "../Drivers/Timer.h"
#include "../Aplicacion/Aplicacion.h"

extern LCD_t LCD;

void LCD_scrollMessage(char* msg, uint8_t line)
{
	size_t len = strlen(msg);

	if(len > 16)
	{
		strncpy((char*) LCD.scroll.string, msg, len);
		LCD.scroll.len = len;
		LCD.scroll.line = line;
		LCD.scroll.index = 0;
		LCD.scroll.isScrolling = TRUE;

		LCD_scroll();
//		startnTimer(6, 6000 + 2 * 200 * (len-16), restoreScreen);
	}
	else
	{
		LCD_pushString(msg, line, 0);
//		startnTimer(6, 5000, restoreScreen);
	}
}

void LCD_print(char* msg)
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

		if(len < 32)
			extract_substr(msg, line2, 16, len);
		else
			extract_substr(msg, line2, 16, 32);

		LCD_pushString(line1, LCD_ROW_1, 0);
		LCD_pushString(line2, LCD_ROW_2, 0);
	}
}

void LCD_printUP(char* msg)
{
	char line[17];
	makeLine(msg, line, 0, strlen(msg));
	LCD_pushLine(line, LCD_ROW_1);
}

void LCD_printDOWN(char* msg)
{
	char line[17];
	makeLine(msg, line, 0, strlen(msg));
	LCD_pushLine(line, LCD_ROW_2);
}

void LCD_printCentered(char* msg, uint8_t row)
{
	size_t len = strlen(msg);
	char line[17];
	makeLine(msg, line, (uint8_t) ((16 - len) / 2), len);
	LCD_pushLine(line, row);
}

uint8_t LCD_pushString(char* msg, uint8_t row, uint8_t pos)
{
	uint8_t i , err=0;

	if (row == LCD_ROW_1)
		err += pushLCD( pos + 0x80 , LCD_CONTROL );

	else
		err += pushLCD( pos + 0xC0 , LCD_CONTROL );


	for( i = 0 ; msg[i] != '\0' && pos+i < 16 ; i++ )
		err += pushLCD( msg[i] , LCD_DATA );

	return err;
}

uint8_t LCD_pushLine(__RW char* msg, uint8_t row)
{
	uint8_t i , err=0;

	if (row == LCD_ROW_1)
		err += pushLCD( 0x80 , LCD_CONTROL );

	else
		err += pushLCD( 0xC0 , LCD_CONTROL );


	for( i = 0 ; i < 16 ; i++ )
		err += pushLCD( msg [i] , LCD_DATA );

	return err;
}

// copia src en dest, empezando en start, y llenando con espacios
void makeLine(const char* src, char* dest, uint8_t start, uint8_t len)
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


void LCD_printReceived(char *msg)
{
	LCD_clear();
	LCD_scrollMessage(msg, LCD_ROW_1);
	LCD.isOn = FALSE;
}
