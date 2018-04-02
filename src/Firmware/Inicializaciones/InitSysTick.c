#include <RegsLPC1769.h>


void InicSysTick ( void );

void InicSysTick(void)
{
	// N=1 para 10ms
	// si divido x 4, interrumpe cada 2,5ms
	STRELOAD  = ( STCALIB / 4 ) - 1;
	STCURR = 0;

	CLKSOURCE = 1;
	TICKINT = 1;
	ENABLE = 1;
}
