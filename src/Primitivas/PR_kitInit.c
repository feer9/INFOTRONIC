#include "../Drivers/chip.h"
#include "../Drivers/regsLPC1769.h"
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/Timer.h"
#include "../Drivers/RTC.h"
#include "../Drivers/LCD.h"
#include "../Drivers/UART.h"
#include "../Drivers/ADC.h"
#include "../Drivers/digital_outputs.h"
#include "../vcom/cdc_vcom.h"


void ExtInt_init	( void );
void SysTick_init	( uint32_t us );
void RIT_init		( uint32_t ms );

void kit_init ( void )
{
//	PLL_init();
	power_init();
//	setupIrcClocking();
	setupClocking();
	SystemCoreClockUpdate();

	PIN_init();
	D_OUT_init();

	usbd_init();
	UART0_init(U0_INIT_STATUS);
	ADC_init();
	RTC_init();
	ExtInt_init();

	TIMER0_init(1000);  // 1ms
	RIT_init(200);		// 200us
	SysTick_init(2500); // 2.5ms

#ifdef DEBUG
	LCD_init(LCD_INTERNAL_RESET_DISABLED);
#else
	LCD_init(LCD_INTERNAL_RESET_ENABLED);
#endif
}
