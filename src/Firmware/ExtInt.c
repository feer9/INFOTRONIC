#include <TPO.h>

extern __RW uint8_t FlagTeclado;

void EINT3_IRQHandler (void)
{
	if(IOIntStatus == 0x01)			// interrupcion por puerto 0
	{
		if(IO0IntStatF & (0x01 << 18))		// en P0.18, SW2, presionado
		{
			FlagTeclado = 2;				// Seteo flag del SW2
			IO0IntEnF &= ~(0x01 << 18);		// deshabilito interrupcion de SW2
			IO0IntEnR &= ~(0x01 << 18);
			IO0IntClr |= (0x01 << 18);		// Limpia flag de interrupcion
		}
		else if(IO0IntStatR & (0x01 << 18)) // en P0.18, SW2, soltado
		{
			IO0IntEnF |= (0x01 << 18);		// habilito interrupcion de SW2
			IO0IntClr |= (0x01 << 18);		// Limpia flag de interrupcion
		}
		if(IO0IntStatF & (0x01 << 11))		// en P0.11, SW3, presionado
		{
			FlagTeclado = 3;				// Seteo flag del SW3
			IO0IntEnF &= ~(0x01 << 11);		// deshabilito interrupcion de SW3
			IO0IntEnR &= ~(0x01 << 11);
			IO0IntClr |= (0x01 << 11);		// Limpia flag de interrupcion
		}
		else if(IO0IntStatR & (0x01 << 11)) // en P0.18, SW3, soltado
		{
			IO0IntEnF |= (0x01 << 11);		// habilito interrupcion de SW3
			IO0IntClr |= (0x01 << 11);		// Limpia flag de interrupcion
		}
	}
	if(IOIntStatus == 0x04)			// interrupcion por puerto 2
	{
		if(IO2IntStatF & (0x01 << 10))		// en P2.10, SW1, presionado
		{
			FlagTeclado = 1;				// Seteo flag del SW1
			IO2IntEnF &= ~(0x01 << 10);		// deshabilito interrupcion de SW1
			IO2IntEnR &= ~(0x01 << 10);
			IO2IntClr |= (0x01 << 10);		// Limpia flag de interrupcion
		}
		else if(IO2IntStatR & (0x01 << 10)) // en P2.10, SW1, soltado
		{
			IO2IntEnF |= (0x01 << 10);		// habilito interrupcion de SW1
			IO2IntClr |= (0x01 << 10);		// Limpia flag de interrupcion
		}
		if(IO2IntStatF & (0x01 << 13))		// en P2.13, SW4, presionado
		{
			FlagTeclado = 4;				// Seteo flag del SW4
			IO2IntEnF &= ~(0x01 << 13);		// deshabilito interrupcion de SW4
			IO2IntEnR &= ~(0x01 << 13);
			IO2IntClr |= (0x01 << 13);		// Limpia flag de interrupcion
		}
		else if(IO2IntStatR & (0x01 << 13)) // en P2.13, SW4, soltado
		{
			IO2IntEnF |= (0x01 << 13);		// habilito interrupcion de SW4
			IO2IntClr |= (0x01 << 13);		// Limpia flag de interrupcion
		}
	}

	EXTINT |= (0x01 << EINT3);	// Limpia flag de interrupción
}

/*
 	#define		KEY0		PORT2,10		//SW1
	#define		KEY1		PORT0,18		//SW2
	#define		KEY2		PORT0,11		//SW3
	#define		KEY3_RC		PORT2,13		//SW4
	#define		KEY4_RC		PORT1,26		//SW5

	EXTINT		0x400FC140
	EXTMODE		0x400FC148		0:nivel		1:flanco
	EXTPOLAR	0x400FC14C		0:activo bajo o flanco descendente		1:activo alto o flanco ascendente

	IntEnR GPIO Interrupt Enable for Rising edge. 	R/W 0 	IO0IntEnR   - 0x4002 8090
															IO2IntEnR   - 0x4002 80B0
	IntEnF GPIO Interrupt Enable for Falling edge. 	R/W 0 	IO0IntEnF   - 0x4002 8094
															IO2IntEnF   - 0x4002 80B4
	IntStatR GPIO Interrupt Status for Rising edge.  RO 0 	IO0IntStatR - 0x4002 8084
															IO2IntStatR - 0x4002 80A4
	IntStatF GPIO Interrupt Status for Falling edge. RO 0 	IO0IntStatF - 0x4002 8088
															IO2IntStatF - 0x4002 80A8
	IntClr   GPIO Interrupt Clear. 					 WO 0 	IO0IntClr   - 0x4002 808C
															IO2IntClr   - 0x4002 80AC
	IntStatus GPIO overall Interrupt Status. 		 RO 0 	IOIntStatus - 0x4002 8080

 */
