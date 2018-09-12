#include <cr_section_macros.h>

#include "../Drivers/LCD.h"
#include "../Drivers/Teclado.h"
#include "../Drivers/Timer.h"

void showClock(void);
void ledBlink(void);
void kit_init(void);

void dummy(void){}

int main(void)
{
	kit_init();
	LCD_printCentered("WELCOME", LCD_ROW_1);
	ledBlink();
	startnTimer(7, 1000, showClock); // reloj en 1s
	while(1) {
		dummy();
	}
}
