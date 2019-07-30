#ifndef APLICACION_APLICACION_H_
#define APLICACION_APLICACION_H_

#include "../Drivers/regsLPC1769.h"

#define N_OPTIONS 3


#define msg_1		"1- CHANGE OUTPUT"
#define desc_1		"Turn the state of an output. Hold to enter."
#define msg_1_1		"switch LED0"
#define msg_1_2		"switch LED1"
#define msg_1_3		"switch LED2"
#define msg_1_4		"switch LED3"
#define desc_1_1	"       ON "
#define desc_1_2	"       OFF"
#define msg_2		"2- ADC Read"
#define desc_2		"get value of \"Ent Analog 0\". Hold to enter"
#define msg_2_1		"AD0.5"
#define desc_2_1	"val:      (   %)"
#define msg_3		"3- UART"
#define desc_3		"enable/send through UART0. Hold to enter."
#define msg_3_1		"Send <F0RRo>"
#define msg_3_2		"Send time req"
#define msg_3_3_1	"set down"
#define msg_3_3_2	"set up"
#define desc_3_1	" UART0 is up  "
#define desc_3_2	" UART0 is down"



struct menu_level {
	char msg[17];
	char desc[44];
	uint8_t pos;
} ;

typedef struct {
	struct menu_level level[2];
	uint8_t curr_level;	// si estoy en menu general (0), o submenú (1)
	int8_t  timerId;
} menu_t;

void showClock(void);
void ledBlink(void);
void restoreScreen(void);
void showMenu(void);
void enterMenu(void);
void showADC(void);
void stopADC(void);
void tramaRecibida(char *msg);

#endif /* APLICACION_APLICACION_H_ */
