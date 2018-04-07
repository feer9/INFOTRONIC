
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <TPO.h>


int main(void)
{
	InicializarKit();
	setearTimers();

	while(1)
	{
		Teclado();
		LCD();
	}
}


extern __RW uint8_t flagTimerLCD;

/** boludeces del timer */
void timerEnd_Handler(uint8_t n)
{
	switch (n)
	{
	case 6:
		write_pin(0,22,1); setearTimers(); break;
	case 7:
		write_pin(0,22,0); break;

	case 0:
		flagTimerLCD |= 0x01; break;
	case 1:
		flagTimerLCD |= 0x02; break;

	default: break;
	}
}
void setearTimers()
{
	timer(6,ON,200000000);
	timer(7,ON,190000000);
}
