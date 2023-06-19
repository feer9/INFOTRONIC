#ifndef APLICACION_APLICACION_H_
#define APLICACION_APLICACION_H_

#include "chip.h"
#include "../Drivers/LCD.h"
#include "../Drivers/oled_ssd1306.h"

#define N_OPTIONS 4


#define msg_1		"1- CHANGE OUTPUT"
#define desc_1		"Turn the state of an output. Hold to enter."
#define msg_1_1		"switch LED0"
#define msg_1_2		"switch LED1"
#define msg_1_3		"switch LED2"
#define msg_1_4		"switch LED3"
#define desc_1_1	"       ON "
#define desc_1_2	"       OFF"
#define msg_2		"2- ADC Read"
#define desc_2		"Get value of \"Ent Analog 0\". Hold to enter"
#define msg_2_1		"AD0.5"
#define desc_2_1	"val:      (   %)"
#define msg_3		"3- UART"
#define desc_3		"Enable/send through UART0. Hold to enter."
#define msg_3_1		"Send <F0RRo>"
#define msg_3_2		"Send time req."
#define msg_3_3_1	"set down"
#define msg_3_3_2	"set up"
#define desc_3_1	" UART0 is up  "
#define desc_3_2	" UART0 is down"
#define msg_4		"4-OLED Display"
#define desc_4		"Turn on/off the OLED Display. Hold to enter."
#define msg_4_1		"OLED Status: ON"
#define msg_4_2		"OLED Status: OFF"
#define desc_4_1	" hold to change"




struct menu_level {
	char msg[17];
	char desc[48];
	__RW uint8_t pos;
} ;

typedef struct {
	struct menu_level level[2];
	__RW uint8_t curr_level;	// si estoy en menu general (0), o submenú (1)
	__RW timer_id_t  timerId;
} menu_t;

void APP_setGlobalSymbols(lcd_t *lcd, u8g2_t *u8g2);

void showClock(void);
void ledBlink(bool st);
void restoreScreen(void);
void showMenu(void);
void enterMenu(void);
void showADC(void);
void stopADC(void);
void tramaRecibida(char *msg);
void toggle_on_lcd(void);
void toggle_off_lcd(void);

#endif /* APLICACION_APLICACION_H_ */
