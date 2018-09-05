#include <Aplicacion.h>

__RW uint8_t tick = 0;
extern __RW uint8_t LCD_Delay;

// recibe el tiempo en milisegundos
void initSysTick(float ms)
{
	// STCALIB valor por defecto para 10ms
	// si divido x 4, interrumpe cada 2,5ms
//	SYSTICK->STRELOAD  = ( SYSTICK->STCALIB / 4 );

	// 100.000 para un clock de 100MHz, llama cada 1ms
	// para temporizaciones repetitivas se resta 1
	SYSTICK->STRELOAD = (100000 * ms) - 1;
	SYSTICK->STCURR = 0;

	SYSTICK->CLKSOURCE = 1;
	SYSTICK->TICKINT = 1;
	SYSTICK->ENABLE = 1;
}

void SysTick_Handler(void)		// systick cada 2,5ms
{/*
	static uint16_t i = 0;
	if(++i == 200) {
		tick = 1;   // tick cada 500ms
		i = 0;
	}*/

	LCD_send();
	if(LCD_Delay)
		LCD_Delay--;

	Debounce_Teclado();
}
