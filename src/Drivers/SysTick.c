#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include "UART.h"
#include "LCD.h"
#include "Teclado.h"



volatile uint32_t decimas_oled = 0;


// recibe el tiempo en microsegundos
void SysTick_init(uint32_t us)
{
	// STCALIB valor por defecto para 10ms
	// si divido x 4, interrumpe cada 2,5ms
//	SYSTICK->RELOAD  = ( SYSTICK->CALIB / 4 );

	// 100.000 para un clock de 100MHz, llama cada 1ms
	// para temporizaciones repetitivas se resta 1
	SysTick->RELOAD = (100 * us) - 1;
	SysTick->CURR = 0;

	SysTick->CLKSOURCE = 1;
	SysTick->TICKINT = 1;
	SysTick->ENABLE = 1;
}

void SysTick_Handler(void)		// systick cada 2,5ms
{
	static int cnt = 40;
	LCD_send();
	debounceTeclado();

	if(--cnt == 0) {
		cnt = 40;
		if(decimas_oled)
			decimas_oled--;
	}
}
