#include <TPO.h>

__RW uint8_t tick = 0;

void SysTick_Handler(void)		// systick cada 2,5ms
{
	static uint16_t i = 0;
	if(++i == 200) {
		tick = 1;   // tick cada 500ms
		i = 0;
	}

	Debounce_Teclado();
}
