
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <TPO.h>


int main(void)
{
	InicializarKit();
	setearTimers();

	while(1)
	{
		Teclado();
	}
}




/** boludeces del timer */
void timerEnd_Handler(uint8_t n)
{
	if (n == 0)
		write_pin(0,22,1);
	else if(n == 1)
		write_pin(0,22,0);

//	timer(7,ON,20000);
	if(n==0 || n==7)
		setearTimers();

}
void setearTimers()
{
	timer(0,ON,2000);
	timer(1,ON,1900);
	timer(7,ON,20000);
}
