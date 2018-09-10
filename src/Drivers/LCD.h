#ifndef _LCD_H
#define _LCD_H

#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"

#define		LCD_BUFFER_SIZE	160

#define 	LCD_ROW_1		0
#define 	LCD_ROW_2		1
#define		LCD_CONTROL		1
#define		LCD_DATA		0

#define		LCD_LINE_SIZE	16
#define		LCD_MAX_MSG_SIZE 80

#define		LCD_POS_HOUR	0,7
#define		LCD_POS_MIN		0,10
#define		LCD_POS_SEC		0,13
#define		LCD_POS_YEAR	1,6
#define		LCD_POS_MONTH	1,11
#define		LCD_POS_DAY		1,14

typedef struct {
	__RW uint8_t	buffer[LCD_BUFFER_SIZE];
	__RW uint32_t	indexIn;
	__RW uint32_t	indexOut;
	__RW uint8_t	queueSize;
} LCD_send_t;

typedef struct {
	__RW char		string[LCD_MAX_MSG_SIZE];
	__RW uint8_t	len;
	__RW uint8_t	index;
	__RW bool		isScrolling;
	__RW uint8_t	line;
} LCD_scroll_t;

typedef struct {
	__RW bool		isOn;
	__RW bool		isInClock;

	LCD_send_t		send;

	LCD_scroll_t	scroll;
} LCD_t;

#define		LCD_printClockTemplate() LCD_print(" Hour:   :  :   Date:     /  /  ")

#define		LCD_clear()	pushLCD( 0x01 , LCD_CONTROL )
#define		LCD_OFF()	pushLCD( 0x08 , LCD_CONTROL )
#define		LCD_ON()	pushLCD( 0x0C , LCD_CONTROL )

uint8_t		LCD_pushString(char* msg, uint8_t row, uint8_t pos);
uint8_t		LCD_pushLine(__RW char* msg, uint8_t row);
void		LCD_printCentered(char* msg, uint8_t row);
void		LCD_printDOWN(char* msg);
void		LCD_printUP(char* msg);
void		LCD_print(char* msg);
void		makeLine(const char* src, char* dest, \
					 uint8_t start, uint8_t len);
void 		LCD_printReceived(char *msg);
void		LCD_displayClock(void);
void		LCD_updateClock(void);


void		LCD_send(void);
uint8_t		pushLCD(uint8_t dato, uint8_t control);
int32_t		popLCD(void);
void		LCD_init(uint8_t);
void		LCD_config(void);
void		LCD_init4Bits(void);
void		LCD_init4Bits_IR(void);
void		LCD_restart(void);
void		LCD_scroll(void);

#endif //_LCD_H
