
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <TPO.h>

TIMER* T0_g = (TIMER*) 0x40004000UL;

int main(void)
{
	TIMER* dummy = T0_g;

	InicializarKit();
	setearTimers();

	while(1)
	{
		Teclado();
	}
}



