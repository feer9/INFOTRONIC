#include <Aplicacion.h>


/**  FUNCIONES DE USUARIO */

uint8_t isTimerEnd(uint8_t n)
{
	return timer(n,IS_TIMER_END,0);
}

void startTimer(uint8_t n, uint32_t time)
{
	timer(n,ON,time);
}

void stopTimer(uint8_t n)
{
	timer(n, OFF, 0);
}


