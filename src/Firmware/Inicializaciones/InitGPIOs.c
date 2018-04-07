#include <TPO.h>

void InicLED ( void )
{
	set_dir(RGBR, SALIDA);
	set_dir(RGBG, SALIDA);
	set_dir(RGBB, SALIDA);
	set_dir(LEDLPC_R, SALIDA);
	set_dir(LEDLPC_G, SALIDA);
	set_dir(LEDLPC_B, SALIDA);
	set_dir(LED1, SALIDA);
	set_dir(LED2, SALIDA);
	set_dir(LED3, SALIDA);
	set_dir(LED4, SALIDA);
	set_dir(BUZZ, SALIDA);

	write_pin(RGBR, OFF);
	write_pin(RGBG, ON);
	write_pin(RGBG, OFF);
	write_pin(RGBB, OFF);
	write_pin(LEDLPC_R, LEDLPC_OFF);
	write_pin(LEDLPC_G, LEDLPC_OFF);
	write_pin(LEDLPC_B, LEDLPC_OFF);
	write_pin(LED1, OFF);
	write_pin(LED2, OFF);
	write_pin(LED3, OFF);
	write_pin(LED4, OFF);
	write_pin(BUZZ, BUZZ_OFF);
}

void InicLCD ( void )
{
	set_dir(LCD_D4, SALIDA);
	set_dir(LCD_D5, SALIDA);
	set_dir(LCD_D6, SALIDA);
	set_dir(LCD_D7, SALIDA);
	set_dir(LCD_RS, SALIDA);
	set_dir(LCD_BF, ENTRADA);
	set_dir(LCD_E , SALIDA);

	write_pin(LCD_D4, 0);
	write_pin(LCD_D5, 0);
	write_pin(LCD_D6, 0);
	write_pin(LCD_D7, 0);
	write_pin(LCD_RS, 0);
	write_pin(LCD_E , 0);
}
/*
	#define		LCD_D4		PORT0,5			//GPIO0
	#define		LCD_D5		PORT0,10		//GPIO0
	#define		LCD_D6		PORT2,4			//GPIO2
	#define		LCD_D7		PORT2,5			//GPIO2

	#define		LCD_RS		PORT2,6			//GPIO2
	#define		LCD_BF		PORT0,28		//GPIO1
	#define		LCD_E		PORT0,4			//GPIO0

	RS = 0: IR write as an internal operation (display clear, etc.)
	RS = 1: Write data to DDRAM or CGRAM (DR to DDRAM or CGRAM)

	RS: register selector			BF: busy flag		E: chip enable signal
	IR: instruction register		DR: data register
	DDRAM: display data ram			CGRAM: character generator ram
*/
