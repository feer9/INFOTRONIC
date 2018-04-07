#include <TPO.h>

__RW uint8_t flagTimerLCD = 0;
__RW uint8_t LCD_Action = 0;

void LCD()
{
	static uint8_t st = 0;

	switch(st)
	{
	case 0:
		if(LCD_Action == 1)
		{
			LCD_Action = 0;
			write_pin(LCD_RS, 1);
			startTimer(0, 40);
			st++;
		}
		break;
	case 1:
		if(flagTimerLCD & 0x01)
		{
			flagTimerLCD &= ~0x01;
			write_pin(LCD_E, 1);
			write_pin(LCD_D4, 0);
			write_pin(LCD_D5, 1);
			write_pin(LCD_D6, 1);
			write_pin(LCD_D7, 0);
			startTimer(0, 250);
			startTimer(1, 500);
			st++;
		}
		break;
	case 2:
		if(flagTimerLCD & 0x01)
		{
			flagTimerLCD &= ~0x01;
			write_pin(LCD_E, 0);
			st++;
		}
		break;
	case 3:
		write_pin(LCD_RS, 0);
		write_pin(LCD_D5, 0);
		write_pin(LCD_D6, 0);
		st++;
		break;
	case 4:
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
	case 5:
		if(flagTimerLCD & 0x01)
		{
			flagTimerLCD &= ~0x01;
			write_pin(LCD_E, 0);
			st++;
		}
		break;
	case 6:
		write_pin(LCD_D4, 0);
		st++;
		break;
	case 7:
		if(flagTimerLCD & 0x02)
		{
			flagTimerLCD &= ~0x02;
			st=0;
		}
	default: break;
	}
}
