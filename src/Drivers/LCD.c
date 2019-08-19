#include "LCD.h"
#include "GPIO.h"
#include "Timer.h"
#include "delay.h"

static void		LCD_init4Bits(void);
static void		LCD_init4Bits_IR(void);
static void		LCD_config(void);
static int16_t	popLCD(void);

extern uint8_t	LCD_pushLine	(const char* msg, uint8_t row);
extern void set_lcd_pointer(lcd_t *lcdp);

static lcd_t *lcd = NULL;


// nota: podria scrollear con el comando 0x07, pero eso desplaza las dos
// lineas al mismo tiempo, y lo que yo quiero es desplazar una sola
void LCD_scroll(void)
{
	if(lcd->scroll.isScrolling)
	{
		const char * line = (char *) (lcd->scroll.string + lcd->scroll.index);
		LCD_pushLine(line , lcd->scroll.line);

		// cuando empieza
		if(lcd->scroll.index == 0)
		{
			lcd->scroll.index++;
			startTimer(&lcd->scroll.timerId, 1500, LCD_scroll);
		}

		// fin de scrolleo
		else if(lcd->scroll.index + 16 >= lcd->scroll.len)
		{
			lcd->scroll.index = 0;
			startTimer(&lcd->scroll.timerId, 1500, LCD_scroll);
		}

		// desplazamiento
		else
		{
			lcd->scroll.index++;
			startTimer(&lcd->scroll.timerId, 200, LCD_scroll);
		}
	}
}

static void init_struct(lcd_t *plcd)
{
	lcd = plcd; // sets local global pointer

	lcd->isOn = false;
	lcd->isInClock = false;
	lcd->isInMenu = false;
	lcd->restore_timerId = -1;

	lcd->send.indexIn = 0;
	lcd->send.indexOut = 0;
	lcd->send.queueSize = 0;
	lcd->send.buffer[0] = '\0';

	lcd->scroll.string[0] = '\0';
	lcd->scroll.len = 0;
	lcd->scroll.index = 0;
	lcd->scroll.isScrolling = false;
	lcd->scroll.line = LCD_ROW_1;
	lcd->scroll.timerId = -1;
}

void LCD_init(lcd_t *plcd, uint8_t internalReset)
{
	init_struct(plcd);
	set_lcd_pointer(plcd); // set primitive's file lcd_t struct pointer

	gpio_pinsel(LCD_D4, PINSEL_GPIO);
	gpio_pinsel(LCD_D5, PINSEL_GPIO);
	gpio_pinsel(LCD_D6, PINSEL_GPIO);
	gpio_pinsel(LCD_D7, PINSEL_GPIO);
	gpio_pinsel(LCD_RS, PINSEL_GPIO);
	gpio_pinsel(LCD_E , PINSEL_GPIO);

	gpio_pinDir_output(LCD_D4);
	gpio_pinDir_output(LCD_D5);
	gpio_pinDir_output(LCD_D6);
	gpio_pinDir_output(LCD_D7);
	gpio_pinDir_output(LCD_RS);
	gpio_pinDir_output(LCD_E );

	gpio_clearPin(LCD_D4);
	gpio_clearPin(LCD_D5);
	gpio_clearPin(LCD_D6);
	gpio_clearPin(LCD_D7);
	gpio_clearPin(LCD_RS);
	gpio_clearPin(LCD_E );

	if(internalReset)
		LCD_init4Bits_IR();
	else
		LCD_init4Bits();
	LCD_config();
}

static void LCD_init4Bits()
{
	gpio_writePin(LCD_E, 0);
	gpio_writePin(LCD_RS, 0);
	// Wait for more than 15 ms after VCC rises to 4.5 V
	delay_ms(20);

	gpio_writePin(LCD_E, 1);

	gpio_writePin(LCD_D4, 1);
	gpio_writePin(LCD_D5, 1);
	gpio_writePin(LCD_D6, 0);
	gpio_writePin(LCD_D7, 0);

	gpio_writePin(LCD_E, 0);

	// Wait for more than 4.1 ms
	delay_ms(5);

	gpio_writePin(LCD_E, 1);
	gpio_writePin(LCD_E, 0);

	// Wait for more than 100 μs
	delay_us(200);

	gpio_writePin(LCD_E, 1);
	gpio_writePin(LCD_E, 0);


	// Set interface to be 4 bits long
	gpio_writePin(LCD_E,1);

	gpio_writePin(LCD_D4,0);
	gpio_writePin(LCD_D5,1);
	gpio_writePin(LCD_D6,0);
	gpio_writePin(LCD_D7,0);

	gpio_writePin(LCD_E,0);
}

static void LCD_init4Bits_IR()
{
	delay_ms(25);

	// Initialization by internal reset
	// Sets to 4-bit operation
	gpio_writePin(LCD_E, 1);

	gpio_writePin(LCD_D4, 0);
	gpio_writePin(LCD_D5, 1);
	gpio_writePin(LCD_D6, 0);
	gpio_writePin(LCD_D7, 0);
	gpio_writePin(LCD_RS, 0);

	gpio_writePin(LCD_E, 0);

//	delay_ms(5);
}

static void LCD_config()
{
	// Function Set
	pushLCD( 0x28 , LCD_CONTROL );	// DL = 0: 4 bits de datos
									// N = 1 : 2 lineas
									// F = 0 : 5x7 puntos

	// Display On/Off Control
	pushLCD( 0x08 , LCD_CONTROL);	// D = 0 : display OFF
									// C = 0 : Cursor OFF
									// B = 0 : Blink OFF

	// Clear Display
	pushLCD( 0x01 , LCD_CONTROL);   // 1.53ms

	pushLCD( 10 , LCD_DELAY );

	// Entry Mode Set
	pushLCD( 0x06 , LCD_CONTROL);	// I/D = 1 : Incrementa puntero
									// S = 0   : NO Shift Display

	// Display On
	pushLCD( 0x0C , LCD_CONTROL);	// D = 1 : display ON
									// C = 0 : Cursor OFF
									// B = 0 : Blink OFF
}

static uint8_t lcd_buff_overflow = 0; // for debug purposes.

status pushLCD(uint8_t dato, uint8_t control)
{
	if(lcd->send.queueSize + 2 > LCD_BUFFER_SIZE) {
		lcd_buff_overflow++;
		return ERROR;
	}

	if ( control == LCD_DELAY )
	{
		for(int i=0; i<dato && lcd->send.queueSize < LCD_BUFFER_SIZE; i++)
		{
			lcd->send.buffer [ lcd->send.indexIn ] = 0xF0;
			lcd->send.indexIn ++;
			lcd->send.indexIn %= LCD_BUFFER_SIZE;
			lcd->send.queueSize ++;
		}
		return SUCCESS;
	}

	lcd->send.buffer [ lcd->send.indexIn ] = (dato >> 4) | (control << 7);

	lcd->send.indexIn ++;
	lcd->send.indexIn %= LCD_BUFFER_SIZE;

	lcd->send.buffer [ lcd->send.indexIn ] = (dato & 0x0F) | (control << 7);

	lcd->send.queueSize += 2;

	lcd->send.indexIn ++;
	lcd->send.indexIn %= LCD_BUFFER_SIZE;

	return SUCCESS;
}

static int16_t popLCD()
{
	uint8_t dato;

	if ( lcd->send.queueSize == 0 )
		return -1;

	dato = lcd->send.buffer [lcd->send.indexOut];
	lcd->send.queueSize --;

	lcd->send.indexOut ++;
	lcd->send.indexOut %= LCD_BUFFER_SIZE;

	return dato;
}

void LCD_send()
{
	int16_t s_data;
	uint8_t data, i;

	for(i=0; i<2; i++)
	{
		if(((s_data = popLCD()) == -1) || (((uint8_t) s_data ) == 0xF0) )
			return;

		data = (uint8_t) s_data;

		// data [bit 7] has register select
		gpio_writePin(LCD_RS, data >> 7);

		gpio_writePin(LCD_E, 1);

		gpio_writePin(LCD_D7, (data >> 3) & 0x01);
		gpio_writePin(LCD_D6, (data >> 2) & 0x01);
		gpio_writePin(LCD_D5, (data >> 1) & 0x01);
		gpio_writePin(LCD_D4, (data >> 0) & 0x01);

		gpio_writePin(LCD_E, 0);
	}
}





