#include <TPO.h>

void InicTimer0(void);

void InicTimer0(void)
{
	PCONP->TIM0 = 1; 		// Habilitar Timer 0
	PCLKSEL->TIMER0 = 1; 	// Clock for timer PCLK = CCLK Selecciono clock
//	T0->PR = 100000;		// Prescaler para 1ms
	T0->PR = 0;
	T0->TCR = 2;			// Apago y reseteo el temporizador
	T0->MR1 = 0xFFFFFFFF;	// Configuro match 1 para detectar overflow
	T0->MCR_.MR1I = 1;		// Interrumpe si se produce overflow
}