#include "chip.h"

// recibe el tiempo en microsegundos
void SysTick_init(uint32_t us)
{
	// STCALIB valor por defecto para 10ms
	// si divido x 4, interrumpe cada 2,5ms
//	SYSTICK->RELOAD  = ( SYSTICK->CALIB / 4 );

	// 100.000 para un clock de 100MHz, llama cada 1ms
	// para temporizaciones repetitivas se resta 1
	SysTick->RELOAD = ((SystemCoreClock / 1e6) * us) - 1;
	SysTick->CURR = 0;

	SysTick->CLKSOURCE = 1;
	SysTick->TICKINT = 0;
	SysTick->ENABLE = 1;
}
