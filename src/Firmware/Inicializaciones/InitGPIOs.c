#include <Aplicacion.h>

void initGPIOs ( void )
{
	set_dir(RGBR, SALIDA);
	set_dir(RGBG, SALIDA);
	set_dir(RGBB, SALIDA);
	set_dir(LEDLPC_R, SALIDA);
	set_dir(LEDLPC_G, SALIDA);
	set_dir(LEDLPC_B, SALIDA);
	set_dir(LED1, SALIDA);
	set_dir(LED2, SALIDA);
	set_dir(LED3, SALIDA);
	set_dir(LED4, SALIDA);
	set_dir(BUZZ, SALIDA);

	write_pin(RGBR, OFF);
	write_pin(RGBG, OFF);
	write_pin(RGBB, OFF);
	write_pin(LEDLPC_R, LEDLPC_OFF);
	write_pin(LEDLPC_G, LEDLPC_OFF);
	write_pin(LEDLPC_B, LEDLPC_OFF);
	write_pin(LED1, OFF);
	write_pin(LED2, OFF);
	write_pin(LED3, OFF);
	write_pin(LED4, OFF);
	write_pin(BUZZ, BUZZ_OFF);
}

