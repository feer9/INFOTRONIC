#ifndef APLICACION_APLICACION_H_
#define APLICACION_APLICACION_H_

#define N_OPTIONS 3

#if 0
#define OPTION0 "CHANGE OUTPUT"
#define OPTION0_DESC "Turn the state of an output. Hold to enter."
#define OPTION1 "opt1"
#define OPTION1_DESC "opt1 desc. Hold to enter."
#define OPTION2 "opt2"
#define OPTION2_DESC "opt2 desc. Hold to enter."
#endif


typedef struct {
	char msg[12];
//	char desc[];
} sub_option_t;

typedef struct {
	char msg[14];
	char desc[44];
	sub_option_t sub_op[4];
} option_t;

typedef struct {
	uint8_t level;
	uint8_t pos[2];
	option_t op[N_OPTIONS];
} menu_t;

void showClock(void);
void ledBlink(void);
void restoreScreen(void);
void showMenu(void);
void enterMenu(void);



#endif /* APLICACION_APLICACION_H_ */
