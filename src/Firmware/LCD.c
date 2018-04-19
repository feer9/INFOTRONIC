#include <Aplicacion.h>

__RW uint8_t Buffer_LCD[LCD_BUFFER_SIZE];
__RW uint32_t LCD_indexIn = 0;
__RW uint32_t LCD_indexOut = 0;
__RW uint32_t LCD_queueSize = 0;

__RW uint8_t LCD_Delay = 0;

void LCD_send()
{
	int dato;

	if((dato = popLCD()) == -1)
		return;

	write_pin(LCD_D7, ((uint8_t) dato) >> 3 & 0x01);
	write_pin(LCD_D6, ((uint8_t) dato) >> 2 & 0x01);
	write_pin(LCD_D5, ((uint8_t) dato) >> 1 & 0x01);
	write_pin(LCD_D4, ((uint8_t) dato) >> 0 & 0x01);

	if( ((uint8_t) dato ) & 0x80 )
		write_pin(LCD_RS, 0);
	else
		write_pin(LCD_RS, 1);

	write_pin(LCD_E, 1);
	write_pin(LCD_E, 0);
}

uint8_t pushLCD(uint8_t dato, uint8_t control)
{
	if(LCD_queueSize >= LCD_BUFFER_SIZE)
		return -1;

	Buffer_LCD [ LCD_indexIn ] = ( dato >> 4 ) & 0x0F;
	if ( control == LCD_CONTROL )
		Buffer_LCD [ LCD_indexIn ] |= 0x80;

	LCD_indexIn ++;

	Buffer_LCD [ LCD_indexIn ] = dato & 0x0F;
	if ( control == LCD_CONTROL )
		Buffer_LCD [ LCD_indexIn ] |= 0x80;

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

	dato = Buffer_LCD [LCD_indexOut];
	LCD_queueSize --;

	LCD_indexOut ++;
	LCD_indexOut %= LCD_BUFFER_SIZE;

	return dato;
}

void configLCD()
{
	uint8_t i = 0;

	LCD_Delay = 10;
	write_pin(LCD_E, 0);
	while(LCD_Delay);

	for(i=0; i<3; i++)
	{
		write_pin(LCD_D4, 1);
		write_pin(LCD_D5, 1);
		write_pin(LCD_D6, 0);
		write_pin(LCD_D7, 0);

		write_pin(LCD_RS, 0);
		write_pin(LCD_E, 1);
		write_pin(LCD_E, 0);

		LCD_Delay = 2;
		while(LCD_Delay);
	}
	// Configuracion en 4 bits
	write_pin(LCD_D4,0);
	write_pin(LCD_D5,1);
	write_pin(LCD_D6,0);
	write_pin(LCD_D7,0);

	write_pin(LCD_RS,0);
	write_pin(LCD_E,1);
	write_pin(LCD_E,0);

	LCD_Delay = 1;
	while(LCD_Delay);

	// lcd seteado en 4 bits
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

void initLCD()
{
	SetPINSEL(LCD_D4, PINSEL_GPIO);
	SetPINSEL(LCD_D4, PINSEL_GPIO);
	SetPINSEL(LCD_D4, PINSEL_GPIO);
	SetPINSEL(LCD_D4, PINSEL_GPIO);
	SetPINSEL(LCD_RS, PINSEL_GPIO);
	SetPINSEL(LCD_E , PINSEL_GPIO);

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

	configLCD();
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













/*

__RW uint8_t LCD_Action = 0;
void LCD()
{
	static uint8_t st = 0;
	static uint8_t stInit = 0;

	switch(st)
	{
	case INIT:
		if(LCD_Action == 1 && stInit == 0)
		{
			LCD_Action = 0;
			write_pin(LCD_RS, 0);
			write_pin(LCD_D4, 0);
			write_pin(LCD_D5, 1);
			write_pin(LCD_D6, 1);
			write_pin(LCD_D7, 0);

			startTimer(0, 4000); // 40us
			stInit++;
		}
		else if(flagTimerLCD && stInit == 1)
		{
			flagTimerLCD = 0;
			write_pin(LCD_D5, 0);
			write_pin(LCD_D6, 0);

			write_pin(LCD_D4, 1);
			write_pin(LCD_D5, 1);
			write_pin(LCD_D6, 1);
			write_pin(LCD_D7, 1);

			startTimer(0, 4000); // 40us
			stInit++;
		}
		else if(flagTimerLCD && stInit == 2)
		{
			write_pin(LCD_D4, 0);
			write_pin(LCD_D5, 0);
			write_pin(LCD_D6, 0);
			write_pin(LCD_D7, 0);

			write_pin(LCD_D4, 1);

			startTimer(0, 200000); // 2ms
			stInit++;
		}
		else if(flagTimerLCD && stInit == 3)
		{
			write_pin(LCD_D4, 0);

			write_pin(LCD_D5, 1);
			write_pin(LCD_D6, 1);
		}
		st = ST1;
		break;

	case ST1:
		if(flagTimerLCD & 0x01)
		{
			flagTimerLCD &= ~0x01;
			write_pin(LCD_E, 1);

			st++;
		}
		break;

	case ST2:
		if(flagTimerLCD & 0x01)
		{
			flagTimerLCD &= ~0x01;
			write_pin(LCD_E, 0);
			st++;
		}
		break;

	case ST3:
		write_pin(LCD_RS, 0);
		write_pin(LCD_D5, 0);
		write_pin(LCD_D6, 0);
		st++;
		break;

	case ST4:
		if(flagTimerLCD & 0x02)
		{
			flagTimerLCD &= ~0x02;
			write_pin(LCD_E, 1);
			write_pin(LCD_D4, 1);
			startTimer(0, 250);
			startTimer(1, 500);
			st++;
		}
		break;

	case ST5:
		if(flagTimerLCD & 0x01)
		{
			flagTimerLCD &= ~0x01;
			write_pin(LCD_E, 0);
			st++;
		}
		break;

	case ST6:
		write_pin(LCD_D4, 0);
		st++;
		break;

	case ST7:
		if(flagTimerLCD & 0x02)
		{
			flagTimerLCD &= ~0x02;
			st=0;
		}

	default:
		break;
	}
}
*/

