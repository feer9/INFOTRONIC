#include <Aplicacion.h>

__RW uint8_t tick = 0;
extern __RW uint8_t LCD_Delay;

// recibe el tiempo en milisegundos
void initSysTick(float ms)
{
	// STCALIB valor por defecto para 10ms
	// si divido x 4, interrumpe cada 2,5ms
	// para temporizaciones repetitivas se resta 1
//	STRELOAD  = ( STCALIB / 4 ) - 1;

	// 100.000 para un clock de 100MHz, llama cada 1ms
	STRELOAD = (100000 * ms) - 1;
	STCURR = 0;

	CLKSOURCE = 1;
	TICKINT = 1;
	ENABLE = 1;
}

void SysTick_Handler(void)		// systick cada 2,5ms
{
	static uint16_t i = 0;
	if(++i == 200) {
		tick = 1;   // tick cada 500ms
		i = 0;
	}

	LCD_send();
	if(LCD_Delay)
		LCD_Delay--;

	Debounce_Teclado();
}
