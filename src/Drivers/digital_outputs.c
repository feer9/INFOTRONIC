#include "chip.h"
#include "digital_outputs.h"

#include "GPIO.h"
#include "Timer.h"


void D_OUT_init( void )
{
	gpio_pinDir_output(RGB_R);
	gpio_pinDir_output(RGB_G);
	gpio_pinDir_output(RGB_B);
	gpio_pinDir_output(LEDLPC_R);
	gpio_pinDir_output(LEDLPC_G);
	gpio_pinDir_output(LEDLPC_B);
	gpio_pinDir_output(LED0);
	gpio_pinDir_output(LED1);
	gpio_pinDir_output(LED2);
	gpio_pinDir_output(LED3);
	gpio_pinDir_output(BUZZ);

	gpio_writePin(RGB_R, OFF);
	gpio_writePin(RGB_G, OFF);
	gpio_writePin(RGB_B, OFF);
	gpio_writePin(LEDLPC_R, LEDLPC_OFF);
	gpio_writePin(LEDLPC_G, LEDLPC_OFF);
	gpio_writePin(LEDLPC_B, LEDLPC_OFF);
	gpio_writePin(LED0, OFF);
	gpio_writePin(LED1, OFF);
	gpio_writePin(LED2, OFF);
	gpio_writePin(LED3, OFF);
	gpio_writePin(BUZZ, BUZZER_OFF);

	// turn off glitch filtering and slew rate control
	// for both P0.27 & P0.28 (I2C features)
	I2CPADCFG = (_BIT(1) | _BIT(3));
}

void D_OUT_toggle(uint8_t n)
{
	switch (n)
	{
	case 0: gpio_togglePin(LED0); break;
	case 1: gpio_togglePin(LED1); break;
	case 2: gpio_togglePin(LED2); break;
	case 3: gpio_togglePin(LED3); break;
	case 4: gpio_togglePin(LED4); break;
	default: break;
	}
}

bool D_OUT_getStatus(uint8_t n)
{
	switch (n)
	{
	case 0: return (bool) gpio_readPin(LED0);
	case 1: return (bool) gpio_readPin(LED1);
	case 2: return (bool) gpio_readPin(LED2);
	case 3: return (bool) gpio_readPin(LED3);
	case 4: return (bool) gpio_readPin(LED4);
	}
	return false;
}

void turnLedsLpcOff(void)
{
	gpio_writePin(LEDLPC_R, LEDLPC_OFF);
	gpio_writePin(LEDLPC_G, LEDLPC_OFF);
	gpio_writePin(LEDLPC_B, LEDLPC_OFF);
}

void flashLedLpc(uint8_t port, uint8_t pin)
{
	static timer_id_t t = -1;
	gpio_writePin(port,pin, LEDLPC_ON);
	startTimer(&t, 5, turnLedsLpcOff);
}

