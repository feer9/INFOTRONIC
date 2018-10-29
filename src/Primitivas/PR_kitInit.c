#include "../Drivers/regsLPC1769.h"
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/PLL.h"
#include "../Drivers/Timer.h"
#include "../Drivers/RTC.h"
#include "../Drivers/LCD.h"
#include "../Drivers/UART.h"



void LEDs_init		( void );
void ExtInt_init	( void );
void SysTick_init	( uint32_t us);


void kit_init ( void )
{
	PLL_init();
	power_init();
	TIMER0_init(1000);
	SysTick_init(2500);
	RTC_init();
	ExtInt_init();
	LEDs_init();
	LCD_init(0);
//	UART0_init();
}



