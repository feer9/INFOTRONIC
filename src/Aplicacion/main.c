//#include <cr_section_macros.h>
#include "../Drivers/UART.h"
#include "../Drivers/oled_ssd1306.h"
#include "../vcom/cdc_vcom.h"
#include "Aplicacion.h"

extern void kit_init(void);

__attribute__((__noreturn__))
int main(void)
{
	kit_init();

	while(1)
	{
		UART0_receive();

		ssd1306_loop();//todo: change this to oled_loop()

		usbd_cdc_vcom();

		__asm volatile ("wfi"); /* sleep until next interruption */
	}
}

/*
__asm volatile ("wfi"); // wait for interrupt
__asm volatile ("wfe"); // wait for event
__asm volatile ("sev"); // send event
 */
