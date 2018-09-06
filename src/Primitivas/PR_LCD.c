#include <Aplicacion.h>

void LCD_print(char* msg)
{
	char line1[17];
	char line2[17];
	size_t len = strlen(msg);

	if(len <= 16)
	{
		LCD(msg, LCD_ROW_1, 0);
	}
	else
	{
		extract_substr(msg, line1, 0, 15);

		if(len < 32)
			extract_substr(msg, line2, 16, len);
		else
			extract_substr(msg, line2, 16, 32);

		LCD(line1, LCD_ROW_1, 0);
		LCD(line2, LCD_ROW_2, 0);
	}
}

void LCD_printUP(char* msg)
{
	char line[17];
	LCD_line(msg, line, 0, strlen(msg));
	LCD(line, LCD_ROW_1, 0);
}

void LCD_printDOWN(char* msg)
{
	char line[17];
	LCD_line(msg, line, 0, strlen(msg));
	LCD(line, LCD_ROW_2, 0);
}

void LCD_printCentered(char* msg, uint8_t row)
{
	size_t len = strlen(msg);
	char line[17];
	LCD_line(msg, line, (uint8_t) ((16 - len) / 2), len);
	LCD(line, row, 0);
}

void LCD(char* msg, uint8_t row, uint8_t pos)
{
	uint8_t i ;

	if (row == LCD_ROW_1)
		pushLCD( pos + 0x80 , LCD_CONTROL );

	else
		pushLCD( pos + 0xC0 , LCD_CONTROL );


	for( i = 0 ; msg[ i ] != '\0' ; i++ )
		pushLCD( msg [ i ] , LCD_DATA );
}

// copia src en dest, empezando en start, y llenando con espacios
void LCD_line(const char* src, char* dest, uint8_t start, uint8_t len)
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

	LCD( intToStr((int) LPC_RTC->HOUR , aux, 2) , LCD_POS_HOUR );
	LCD( intToStr((int) LPC_RTC->MIN  , aux, 2) , LCD_POS_MIN  );
	LCD( intToStr((int) LPC_RTC->SEC  , aux, 2) , LCD_POS_SEC  );
	LCD( intToStr((int) LPC_RTC->YEAR , aux, 4) , LCD_POS_YEAR );
	LCD( intToStr((int) LPC_RTC->MONTH, aux, 2) , LCD_POS_MONTH);
	LCD( intToStr((int) LPC_RTC->DOM  , aux, 2) , LCD_POS_DAY  );
}
	/*
	char line[17] = "Hour: ";
	intToStr((int)LPC_RTC->HOUR, aux, 2);
	strcat(line, aux);
	strcat(line, ":");
	intToStr((int)LPC_RTC->MIN, aux, 2);
	strcat(line, aux);
	strcat(line, ":");
	intToStr((int)LPC_RTC->SEC, aux, 2);
	strcat(line, aux);
	LCD_printCentered(line, LCD_ROW_1);

	strcpy(line, "Date: ");
	intToStr((int)LPC_RTC->YEAR, aux, 4);
	strcat(line, aux);
	strcat(line, "/");
	intToStr((int)LPC_RTC->MONTH, aux, 2);
	strcat(line, aux);
	strcat(line, "/");
	intToStr((int)LPC_RTC->DOM, aux, 2);
	strcat(line, aux);
	LCD_printCentered(line, LCD_ROW_2);*/


