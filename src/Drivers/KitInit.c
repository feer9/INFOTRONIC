#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include "PLL.h"
#include "Timer.h"
#include "RTC.h"
#include "LCD.h"
#include "UART.h"


void kit_init		( void );
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



