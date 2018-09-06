#include <Aplicacion.h>

__RW uint8_t LCD_Delay = 0;

__RW uint8_t buffer_LCD[LCD_BUFFER_SIZE];
__RW uint32_t LCD_indexIn = 0;
__RW uint32_t LCD_indexOut = 0;
__RW uint32_t LCD_queueSize = 0;

void initLCD(uint8_t IR)
{
	setPINSEL(LCD_D4, PINSEL_GPIO);
	setPINSEL(LCD_D4, PINSEL_GPIO);
	setPINSEL(LCD_D4, PINSEL_GPIO);
	setPINSEL(LCD_D4, PINSEL_GPIO);
	setPINSEL(LCD_RS, PINSEL_GPIO);
	setPINSEL(LCD_E , PINSEL_GPIO);

	set_dir(LCD_D4, SALIDA);
	set_dir(LCD_D5, SALIDA);
	set_dir(LCD_D6, SALIDA);
	set_dir(LCD_D7, SALIDA);
	set_dir(LCD_RS, SALIDA);
	set_dir(LCD_E , SALIDA);

	write_pin(LCD_D4, 0);
	write_pin(LCD_D5, 0);
	write_pin(LCD_D6, 0);
	write_pin(LCD_D7, 0);
	write_pin(LCD_RS, 0);
	write_pin(LCD_E , 0);

	if(IR)
		LCD_init4Bits_IR();
	else
		LCD_init4Bits();

	LCD_config();
}

void LCD_init4Bits()
{
	uint8_t i = 0;

	LCD_Delay = 10;
	write_pin(LCD_E, 0);
	while(LCD_Delay);

	for(i=0; i<3; i++)
	{
		write_pin(LCD_E, 1);

		write_pin(LCD_D4, 1);
		write_pin(LCD_D5, 1);
		write_pin(LCD_D6, 0);
		write_pin(LCD_D7, 0);
		write_pin(LCD_RS, 0);

		write_pin(LCD_E, 0);

		LCD_Delay = 2;
		while(LCD_Delay);
	}

	// Configuracion en 4 bits
	write_pin(LCD_E,1);

	write_pin(LCD_D4,0);
	write_pin(LCD_D5,1);
	write_pin(LCD_D6,0);
	write_pin(LCD_D7,0);
	write_pin(LCD_RS,0);

	write_pin(LCD_E,0);

	LCD_Delay = 1;
	while(LCD_Delay);
}

void LCD_init4Bits_IR()
{
	LCD_Delay = 10;
	while(LCD_Delay);

	// Initialization by internal reset
	// Sets to 4-bit operation
	write_pin(LCD_E, 1);

	write_pin(LCD_D4, 0);
	write_pin(LCD_D5, 1);
	write_pin(LCD_D6, 0);
	write_pin(LCD_D7, 0);
	write_pin(LCD_RS, 0);

	write_pin(LCD_E, 0);

	LCD_Delay = 2;
	while(LCD_Delay);
}

void LCD_config()
{
	pushLCD( 0x28 , LCD_CONTROL );	// DL = 0: 4 bits de datos
									// N = 1 : 2 lineas
									// F = 0 : 5x7 puntos

	pushLCD( 0x08 , LCD_CONTROL);	// D = 0 : display OFF
									// C = 0 : Cursor OFF
									// B = 0 : Blink OFF

	pushLCD( 0x01 , LCD_CONTROL);	// clear display

	pushLCD( 0x06 , LCD_CONTROL);	// I/D = 1 : Incrementa puntero
									// S = 0 : NO Shift Display

	// Activo el LCD y listo para usar !
	pushLCD( 0x0C , LCD_CONTROL);	// D = 1 : display ON
									// C = 0 : Cursor OFF
									// B = 0 : Blink OFF
}

void LCD_send()
{
	int dato;
	uint8_t i;

	for(i=0; i<2; i++)
	{
		if((dato = popLCD()) == -1)
			return;

		if( ((uint8_t) dato ) & 0x80 )
			write_pin(LCD_RS, 0);
		else
			write_pin(LCD_RS, 1);

		write_pin(LCD_E, 1);

		write_pin(LCD_D7, ((uint8_t) dato) >> 3 & 0x01);
		write_pin(LCD_D6, ((uint8_t) dato) >> 2 & 0x01);
		write_pin(LCD_D5, ((uint8_t) dato) >> 1 & 0x01);
		write_pin(LCD_D4, ((uint8_t) dato) >> 0 & 0x01);

		write_pin(LCD_E, 0);
	}
}

uint8_t pushLCD(uint8_t dato, uint8_t control)
{
	if(LCD_queueSize >= LCD_BUFFER_SIZE)
		return -1;

	buffer_LCD [ LCD_indexIn ] = ( dato >> 4 ) & 0x0F;
	if ( control == LCD_CONTROL )
		buffer_LCD [ LCD_indexIn ] |= 0x80;

	LCD_indexIn ++;

	buffer_LCD [ LCD_indexIn ] = dato & 0x0F;
	if ( control == LCD_CONTROL )
		buffer_LCD [ LCD_indexIn ] |= 0x80;

	LCD_queueSize += 2;

	LCD_indexIn ++;
	LCD_indexIn %= LCD_BUFFER_SIZE;

	return 0;
}

int32_t popLCD()
{
	uint8_t dato;

	if ( LCD_queueSize == 0 )
		return -1;

	dato = buffer_LCD [LCD_indexOut];
	LCD_queueSize --;

	LCD_indexOut ++;
	LCD_indexOut %= LCD_BUFFER_SIZE;

	return dato;
}

//uint8_t LCD_getIndex()




