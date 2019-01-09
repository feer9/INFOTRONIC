#include <cr_section_macros.h>

#include "../Drivers/LCD.h"
#include "../Drivers/Teclado.h"
#include "../Drivers/Timer.h"
#include "../Drivers/UART.h"
#include "../Drivers/RTC.h"
#include "Aplicacion.h"

extern void kit_init(void);
extern uart_t uart0;

//static void dummy(void) {}

int main(void)
{
	kit_init();
//	RTC_resetTime();
	LCD_printCentered("WELCOME", LCD_ROW_1);
	ledBlink();
	startTimer(1000, showClock); // reloj en 1s
	startTimer(5000, UART0_requestTime);


	while(1) {

		if(uart0.status)
			UART0_receive();

	}
}
