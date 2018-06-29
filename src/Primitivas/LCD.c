#include <Aplicacion.h>

void displayLCD(char* msg, uint8_t row, uint8_t pos)
{
	uint8_t i ;

	if (row == RENGLON_1)
		pushLCD( pos + 0x80 , LCD_CONTROL );

	else
		pushLCD( pos + 0xC0 , LCD_CONTROL );


	for( i = 0 ; msg[ i ] != '\0' ; i++ )
		pushLCD( msg [ i ] , LCD_DATA );
}
