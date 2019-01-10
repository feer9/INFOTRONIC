#ifndef APLICACION_APLICACION_H_
#define APLICACION_APLICACION_H_

#include "../Drivers/regsLPC1769.h"

#define N_OPTIONS 3

#if 0
#define OPTION0 "CHANGE OUTPUT"
#define OPTION0_DESC "Turn the state of an output. Hold to enter."
#define OPTION1 "ADC Read"
#define OPTION1_DESC "get value of \"Ent Analog 0\". Hold to enter"
#define OPTION2 "opt2"
#define OPTION2_DESC "opt2 desc. Hold to enter."
#endif


typedef struct {
	char msg[13];
//	char desc[];
} sub_option_t;

typedef struct {
	char msg[17];
	char desc[44];
	sub_option_t sub_op[4];
} option_t;

typedef struct {
	uint8_t level;	// si estoy en menu general (0), o submenú (1)
	uint8_t pos[2];	// pos[0] pos en el menu gral, pos[1] pos en el submenú actual
	int8_t  timerId;
	option_t op[N_OPTIONS];
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
