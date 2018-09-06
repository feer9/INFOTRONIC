#include <Aplicacion.h>


/**  FUNCIONES DE USUARIO */

// devuelve 1 si el timer "n" NO está activo
uint8_t isTimerEnd(uint8_t n)
{
	return timer(n,IS_TIMER_END,0);
}

// n: numero de timer ; time: tiempo en ms
void startTimer(uint8_t n, uint32_t time)
{
	timer(n,ON,time);
}

// detiene el timer "n"
void stopTimer(uint8_t n)
{
	timer(n, OFF, 0);
}


