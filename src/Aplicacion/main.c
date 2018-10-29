#include <cr_section_macros.h>

#include "../Drivers/LCD.h"
#include "../Drivers/Teclado.h"
#include "../Drivers/Timer.h"
#include "Aplicacion.h"

extern void kit_init(void);

static void dummy(void) {}

int main(void)
{
	kit_init();
	LCD_printCentered("WELCOME", LCD_ROW_1);
	ledBlink();
	startTimer(1000, showClock); // reloj en 1s
	while(1) {
		dummy();
	}
}
