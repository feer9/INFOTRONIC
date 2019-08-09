#include "chip.h"
#include "digital_outputs.h"

#include "GPIO.h"
#include "Timer.h"


void D_OUT_init( void )
{
	set_dir_output(RGB_R);
	set_dir_output(RGB_G);
	set_dir_output(RGB_B);
	set_dir_output(LEDLPC_R);
	set_dir_output(LEDLPC_G);
	set_dir_output(LEDLPC_B);
	set_dir_output(LED0);
	set_dir_output(LED1);
	set_dir_output(LED2);
	set_dir_output(LED3);
	set_dir_output(BUZZ);

	write_pin(RGB_R, OFF);
	write_pin(RGB_G, OFF);
	write_pin(RGB_B, OFF);
	write_pin(LEDLPC_R, LEDLPC_OFF);
	write_pin(LEDLPC_G, LEDLPC_OFF);
	write_pin(LEDLPC_B, LEDLPC_OFF);
	write_pin(LED0, OFF);
	write_pin(LED1, OFF);
	write_pin(LED2, OFF);
	write_pin(LED3, OFF);
	write_pin(BUZZ, BUZZER_OFF);

	// turn off glitch filtering and slew rate control
	// for both P0.27 & P0.28 (I2C features)
	I2CPADCFG = (_BIT(1) | _BIT(3));
}

void D_OUT_toggle(uint8_t n)
{
	switch (n)
	{
	case 0: toggle_pin(LED0); break;
	case 1: toggle_pin(LED1); break;
	case 2: toggle_pin(LED2); break;
	case 3: toggle_pin(LED3); break;
	case 4: toggle_pin(LED4); break;
	default: break;
	}
}

bool D_OUT_getStatus(uint8_t n)
{
	switch (n)
	{
	case 0: return (bool) read(LED0);
	case 1: return (bool) read(LED1);
	case 2: return (bool) read(LED2);
	case 3: return (bool) read(LED3);
	case 4: return (bool) read(LED4);
	}
	return false;
}

void turnLedsLpcOff(void)
{
	set_pin(LEDLPC_R);
	set_pin(LEDLPC_G);
	set_pin(LEDLPC_B);
}

void flashLedLpc(uint8_t port, uint8_t pin)
{
	static timer_id_t t = -1;
	write_pin(port,pin, LEDLPC_ON);
	startTimer(&t, 5, turnLedsLpcOff);
}

