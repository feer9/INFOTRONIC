#include <cr_section_macros.h>

#include "../Drivers/LCD.h"
#include "../Drivers/Teclado.h"
#include "../Drivers/Timer.h"
#include "../Drivers/UART.h"
#include "../Drivers/RTC.h"
#include "../Drivers/i2c.h"
#include "../Drivers/delay.h"
#include "../Drivers/ssd1306.h"
#include "Aplicacion.h"
//#include "cdc_vcom.h"

extern void kit_init(void);

// todo: deshacerme de las structs globales (hacerlas static) y reemplazar por funciones primitivas
extern uart_t uart0;


//static void dummy(void) {}


const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

static u8g2_t u8g2;





int main(void)
{
	kit_init();
//	setupIrcClocking();

#if 0
	configurePin(0,   0,  PINMODE_NONE, PINSEL_FUNC2),	/* TXD3 */
	configurePin(0,   1,  PINMODE_NONE, PINSEL_FUNC2),	/* RXD3 */
	configurePin(0,   4,  PINMODE_NONE, PINSEL_FUNC2),	/* CAN-RD2 */
	configurePin(0,   5,  PINMODE_NONE, PINSEL_FUNC2),	/* CAN-TD2 */
	configurePin(0,  22,  PINMODE_NONE, PINSEL_GPIO ),	/* Led 0 */
	configurePin(0,  23,  PINMODE_NONE, PINSEL_FUNC1),	/* ADC 0 */
	configurePin(0,  26,  PINMODE_NONE, PINSEL_FUNC2),	/* DAC */
	configurePin(1,  27,  PINMODE_NONE, PINSEL_FUNC1);
	setupClocking();
	SystemCoreClockUpdate();
	usbd_init();
#endif
	ssd1306_init(&u8g2);
//	u8x8_SendF(&u8g2.u8x8, "c", 0xae); // turn off OLED

	ledBlink();
	LCD_WelcomeMessage();
	startTimer(1000, showClock); // reloj en 1s

#if 0
	RTC_resetTime();
	startTimer(5000, UART0_requestTime);
#endif


	while(1)
	{
		if(uart0.status && !uart0.bufferRx.isEmpty())
			UART0_receive();

		sm_ssd1306(&u8g2);

//		sm_usbd();

		__asm volatile ("wfi"); /* sleep until next interruption */
	}
}

/*
__asm volatile ("wfi"); // wait for interrupt
__asm volatile ("wfe"); // wait for event
__asm volatile ("sev"); // send event
 */
