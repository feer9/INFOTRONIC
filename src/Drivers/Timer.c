#include "Timer.h"

//TODO: agregar un contador con el Timer1. Ver que onda el RIT

// recibe el tiempo del prescaler en microsegundos
void TIMER0_init(uint32_t us)
{
	PCONP |= PCONP_TIMER0;						// Enciendo Timer 0
	setPCLKDiv(PCLKSEL_TIMER0, PCLKDIV_1);   	// Clock for timer = Core clock
	T0->PR = ((SystemCoreClock / 1e6) * us) - 1;// Prescaler
//	T0->PR = 99999;								// Prescaler para 1ms con PCLK_T0 = 100MHz
	T0->TCR = 2;								// Apago y reseteo el temporizador
	T0->MR1 = 0xFFFFFFFF;						// Configuro match 1 para detectar overflow
	T0->MCR _SET_BIT(MR1I);						// Interrumpe si se produce overflow
	ISER0 = NVIC_TIMER0;						// Habilito interrupcion en el NVIC
}


void TIMER0_IRQHandler (void)
{
	if(T0->IR & _BIT(0))
	{
		// Interrumpió match 0 -> terminó un timer
		T0->IR _SET_BIT(0);		// Borro flag del Match 0
		timerEnded();
	}

    if(T0->IR & _BIT(1))
    {
    	// Interrumpió match 1 -> overflow
    	T0->IR _SET_BIT(1); 	// Borro flag del Match 1
		// do stuff
    }
}

