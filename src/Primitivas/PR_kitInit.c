#include "../Drivers/clock.h"
#include "../Drivers/regsLPC1769.h"
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/Timer.h"
#include "../Drivers/RTC.h"
#include "../Drivers/LCD.h"
#include "../Drivers/UART.h"
#include "../Drivers/ADC.h"
#include "../Drivers/clock.h"

void D_IN_init		( void );
void ExtInt_init	( void );
void SysTick_init	( uint32_t us );


void kit_init ( void )
{
	PLL_init();
	power_init();
	SystemCoreClockUpdate();
	PIN_init();
	SysTick_init(2500); // 2.5 ms
	TIMER0_init(1000);  // 1.0 ms

//	delay_ms(500); // temporal, para evitar problemas

	RTC_init();
	D_IN_init();
	ExtInt_init();

/*#ifdef DEBUG
	LCD_init(LCD_INTERNAL_RESET_DISABLED);
#else
	LCD_init(LCD_INTERNAL_RESET_ENABLED);
#endif*/

	UART0_init(U0_INIT_STATUS);
	ADC_init();
}
