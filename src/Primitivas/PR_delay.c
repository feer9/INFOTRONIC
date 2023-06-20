#include "../Drivers/delay.h"




/*
  Delay by the provided number of micro seconds.
  Limitation: "us" * System-Freq in MHz must not overflow in 32 bit.
  Values between 0 and 1.000.000 (1 second) are ok.

  Important: Call SystemCoreClockUpdate() before calling this function.
*/
void delay_us(uint32_t us)
{
  uint32_t sys_ticks;

  sys_ticks = SystemCoreClock;
  sys_ticks /=1000000UL;
  sys_ticks *= us;
  delay_system_ticks(sys_ticks);
}

void delay_ms(uint32_t ms)
{
	uint32_t sys_ticks;

	sys_ticks = SystemCoreClock;
	sys_ticks /=1000UL;
	sys_ticks *= ms;
	delay_system_ticks(sys_ticks);
}

void delay(uint32_t seconds)
{
	for( ; seconds ; --seconds )
		delay_ms(1000);
}
