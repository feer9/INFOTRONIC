#ifndef _LCD_H
#define _LCD_H

#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"

#define		LCD_INTERNAL_RESET_ENABLED  1
#define		LCD_INTERNAL_RESET_DISABLED 0

#define		LCD_BUFFER_SIZE	128

#define 	LCD_ROW_1		0U
#define 	LCD_ROW_2		1U
#define		LCD_CONTROL		0U	// RS pin = 0
#define		LCD_DATA		1U	// RS pin = 1
#define		LCD_DELAY		2U

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
} LCD_buffer_t;

typedef struct {
	__RW char		string[LCD_MAX_MSG_SIZE];
	__RW uint8_t	len;
	__RW uint8_t	index;
	__RW bool		isScrolling;
	__RW uint8_t	line;
	__RW timer_id_t	timerId;
} LCD_scroll_t;

typedef struct {
	__RW bool		isOn;
	__RW bool		isInClock;
	__RW bool		isInMenu;
	__RW timer_id_t	restore_timerId;

	LCD_buffer_t	send;
	LCD_scroll_t	scroll;

} lcd_t;

#define		LCD_printClockTemplate() LCD_print(" Hour:   :  :   Date:     /  /  ")

// la operacion clear display (0x01) y return home (control 0x02) requiere 1.52ms
#define		LCD_clear()		{pushLCD( 0x01 , LCD_CONTROL ); pushLCD(10, LCD_DELAY);}
#define		LCD_OFF()		pushLCD( 0x08 , LCD_CONTROL )
#define		LCD_ON()		pushLCD( 0x0C , LCD_CONTROL )

void		LCD_WelcomeMessage	(void);
uint8_t		LCD_pushString		(const char* msg, uint8_t row, uint8_t pos);
uint8_t		LCD_pushLine		(const char* msg, uint8_t row);
void		LCD_printDOWN		(const char* msg);
void		LCD_printUP			(const char* msg);
void		LCD_printCentered	(const char* msg, uint8_t row);
void		LCD_print			(const char* msg);
void 		LCD_printReceived	(const char* msg);
void		LCD_printInt		(int num, uint8_t row, uint8_t pos, uint8_t digits);
void		LCD_displayClock	(void);
void		LCD_updateClock		(void);
void		LCD_scrollMessage	(const char* msg, uint8_t line);
void		LCD_stopScroll		(void);

void		LCD_send				(void);
status		pushLCD					(uint8_t dato, uint8_t control);
void		LCD_init				(lcd_t *lcdp, uint8_t internalReset);
void		LCD_scroll				(void);
bool		LCD_isOn		(void);
bool		LCD_isInClock	(void);
bool		LCD_isInMenu	(void);
void		LCD_setOff		(void);
void		LCD_setOn		(void);
void		LCD_setClockOff	(void);
void		LCD_setClockOn	(void);
void		LCD_showNothing	(void);

#endif //_LCD_H
