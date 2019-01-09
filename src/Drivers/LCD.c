#include "LCD.h"
#include "GPIO.h"
#include "Timer.h"


static void		LCD_init4Bits(void);
static void		LCD_init4Bits_IR(void);
static void		LCD_config(void);
static int16_t	popLCD(void);

extern uint8_t	LCD_pushLine	(const char* msg, uint8_t row);

__RW uint8_t LCD_Delay = 0;

LCD_t LCD = {
		.isOn = TRUE,
		.isInClock = FALSE,
		.isInMenu = FALSE,
		.restore_timerId = -1,

		.send.indexIn = 0,
		.send.indexOut = 0,
		.send.queueSize = 0,
		.send.buffer[0] = '\0',

		.scroll.string[0] = '\0',
		.scroll.len = 0,
		.scroll.index = 0,
		.scroll.isScrolling = FALSE,
		.scroll.line = LCD_ROW_1,
		.scroll.timerId = -1
};

void LCD_scroll(void)
{
	if(LCD.scroll.isScrolling)
	{
		const char * line = (char *) (LCD.scroll.string + LCD.scroll.index);
		LCD_pushLine(line , LCD.scroll.line);

		// cuando empieza
		if(LCD.scroll.index == 0)
		{
			LCD.scroll.index++;
			LCD.scroll.timerId = startTimer(1500, LCD_scroll);
		}

		// fin de scrolleo
		else if(LCD.scroll.index + 16 >= LCD.scroll.len)
		{
			LCD.scroll.index = 0;
			LCD.scroll.timerId = startTimer(1500, LCD_scroll);
		}

		// desplazamiento
		else
		{
			LCD.scroll.index++;
			LCD.scroll.timerId = startTimer(200, LCD_scroll);
		}
	}
}


void LCD_init(uint8_t IR)
{
	setPINSEL(LCD_D4, PINSEL_GPIO);
	setPINSEL(LCD_D5, PINSEL_GPIO);
	setPINSEL(LCD_D6, PINSEL_GPIO);
	setPINSEL(LCD_D7, PINSEL_GPIO);
	setPINSEL(LCD_RS, PINSEL_GPIO);
	setPINSEL(LCD_E , PINSEL_GPIO);

	// no deberia ser necesario
	setPINMODE(LCD_D4, PINMODE_NONE);
	setPINMODE(LCD_D5, PINMODE_NONE);
	setPINMODE(LCD_D6, PINMODE_NONE);
	setPINMODE(LCD_D7, PINMODE_NONE);
	setPINMODE(LCD_RS, PINMODE_NONE);
	setPINMODE(LCD_E , PINMODE_NONE);

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

	if(LCD_INTERNAL_RESET_DISABLED)
		LCD_init4Bits_IR();
	else
		LCD_init4Bits();

	LCD_config();
}

static void LCD_init4Bits()
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

	LCD_Delay = 2;
	while(LCD_Delay);
}

static void LCD_init4Bits_IR()
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

static void LCD_config()
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
	if(LCD.send.queueSize + 1 >= LCD_BUFFER_SIZE)
		return 1;

	LCD.send.buffer [ LCD.send.indexIn ] = ( dato >> 4 ) & 0x0F;
	if ( control == LCD_CONTROL )
		LCD.send.buffer [ LCD.send.indexIn ] |= 0x80;

	LCD.send.indexIn ++;

	LCD.send.buffer [ LCD.send.indexIn ] = dato & 0x0F;
	if ( control == LCD_CONTROL )
		LCD.send.buffer [ LCD.send.indexIn ] |= 0x80;

	LCD.send.queueSize += 2;

	LCD.send.indexIn ++;
	LCD.send.indexIn %= LCD_BUFFER_SIZE;

	return 0;
}

static int16_t popLCD()
{
	uint8_t dato;

	if ( LCD.send.queueSize == 0 )
		return -1;

	dato = LCD.send.buffer [LCD.send.indexOut];
	LCD.send.queueSize --;

	LCD.send.indexOut ++;
	LCD.send.indexOut %= LCD_BUFFER_SIZE;

	return dato;
}




