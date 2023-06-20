#include "Aplicacion.h"
#include "chip.h"
#include "KitInfo2_BaseBoard.h"
#include "../Drivers/Timer.h"
#include "../Drivers/RTC.h"
#include "../Drivers/LCD.h"
#include "../Drivers/UART.h"
#include "../Drivers/ADC.h"
#include "../Drivers/digital_outputs.h"
#include "../Drivers/rit.h"
#include "../vcom/cdc_vcom.h"


void ExtInt_init	( void );
void SysTick_init	( uint32_t us );

static u8g2_t u8g2;
static lcd_t lcd;

void kit_init ( void )
{
	chip_init();

	gpio_init();
	D_OUT_init();

	usbd_init();
	UART0_init(U0_INIT_STATUS);
	ADC_init();
	ExtInt_init();

	APP_setGlobalSymbols(&lcd, &u8g2);

	TIMER0_init(1000);   // 1ms
	SysTick_init(10000); // 10ms
	RIT_init(200);		 // 200us
	RTC_init();

	LCD_init(&lcd, LCD_INTERNAL_RESET_MODE);
	ssd1306_init(&u8g2);
	ssd1306_off();

	RIT_enableInterrupts();
	SysTick_enableInterrupts();

	ledBlink(true);
	LCD_WelcomeMessage();
	startTimer(NULL, 1000, showClock); // reloj en 1s
}
