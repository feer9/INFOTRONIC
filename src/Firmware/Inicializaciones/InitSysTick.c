#include <RegsLPC1769.h>

void initSysTick(void)
{
	// STCALIB valor por defecto para 10ms
	// si divido x 4, interrumpe cada 2,5ms
	// para temporizaciones repetitivas se resta 1
	STRELOAD  = ( STCALIB / 4 ) - 1;
	STCURR = 0;

	CLKSOURCE = 1;
	TICKINT = 1;
	ENABLE = 1;
}
