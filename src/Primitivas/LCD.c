#include <Aplicacion.h>

void LCD_print(char* msg)
{
	size_t len = strlen(msg);
	char line1[17];
	char line2[17];
	char aux[17];
	uint8_t i;

	if(len <= 16)
	{
		strncpy(aux, msg, len);
		LCD_line(aux, line1, 0, len);
		for(i = 0; i < 16; i++)
			line2[i] = ' ';
		line2[16] = '\0';
	}
	else
	{
		strncpy(line1, msg, 15);
		line1[16] = '\0';
		if(len < 32)
		{
			extract_substr(msg, aux, 16, len);
			LCD_line(aux, line2, 0, len - 16);
		}
		else
		{
			extract_substr(msg, aux, 16, 32);
			LCD_line(aux, line2, 0, 16);
		}
	}

	LCD(line1, RENGLON_1, 0);
	LCD(line2, RENGLON_2, 0);
}

void LCD_printUP(char* msg)
{
	char line[17];
	LCD_line(msg, line, 0, strlen(msg));
	LCD(line, RENGLON_1, 0);
}

void LCD_printDOWN(char* msg)
{
	char line[17];
	LCD_line(msg, line, 0, strlen(msg));
	LCD(line, RENGLON_2, 0);
}

void LCD_printCentered(char* msg, uint8_t row)
{
	size_t len = strlen(msg);
	char line[17];
	LCD_line(msg, line, (uint8_t) ((16 - len) / 2), len);
	LCD(line, row, 0);
}

void LCD_clear()
{
	LCD("                ", RENGLON_1, 0);
	LCD("                ", RENGLON_2, 0);
}

void LCD(char* msg, uint8_t row, uint8_t pos)
{
	uint8_t i ;

	if (row == RENGLON_1)
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
