#ifndef _LCD_H
#define _LCD_H

#define		LCD_BUFFER_SIZE	128

#define 	LCD_ROW_1		0
#define 	LCD_ROW_2		1
#define		LCD_CONTROL		1
#define		LCD_DATA		0

#define		LCD_LINE_SIZE	16

#define		LCD_POS_HOUR	0,7
#define		LCD_POS_MIN		0,10
#define		LCD_POS_SEC		0,13
#define		LCD_POS_YEAR	1,6
#define		LCD_POS_MONTH	1,11
#define		LCD_POS_DAY		1,14

#define		LCD_printClockTemplate() LCD_print(" Hour:   :  :   Date:     /  /  ")

#define		LCD_clear()	pushLCD( 0x01 , LCD_CONTROL )
#define		LCD_OFF()	pushLCD( 0x08 , LCD_CONTROL )
#define		LCD_ON()	pushLCD( 0x0C , LCD_CONTROL )

void		LCD(char* msg, uint8_t row, uint8_t pos);
void		LCD_printCentered(char* msg, uint8_t row);
void		LCD_printDOWN(char* msg);
void		LCD_printUP(char* msg);
void		LCD_print(char* msg);
void		LCD_line(const char* src, char* dest, \
					 uint8_t start, uint8_t len);

void		LCD_displayClock(void);
void		LCD_updateClock(void);


void		LCD_send(void);
uint8_t		pushLCD(uint8_t dato, uint8_t control);
int32_t		popLCD(void);
void		initLCD(void);
void		LCD_config(void);
void		LCD_init4Bits(void);
void		LCD_init4Bits_IR(void);

#endif //_LCD_H
