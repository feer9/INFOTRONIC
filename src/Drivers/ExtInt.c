#include <Aplicacion.h>

__RW uint8_t key_change = 0;

/*		DEFINES Y REGISTROS USADOS

 	#define		KEY0		PORT2,10		//SW1
	#define		KEY1		PORT0,18		//SW2
	#define		KEY2		PORT0,11		//SW3
	#define		KEY3_RC		PORT2,13		//SW4
	#define		KEY4_RC		PORT1,26		//SW5

	EXTINT		0x400FC140		External Interrupt Flag Register
	EXTMODE		0x400FC148		0:nivel		1:flanco
	EXTPOLAR	0x400FC14C		0:activo bajo o flanco descendente		1:activo alto o flanco ascendente

	IntEnR GPIO Interrupt Enable for Rising edge. 	R/W 0 	IO0IntEnR   - 0x4002 8090
															IO2IntEnR   - 0x4002 80B0
	IntEnF GPIO Interrupt Enable for Falling edge. 	R/W 0 	IO0IntEnR   - 0x4002 8094
															IO2IntEnR   - 0x4002 80B4
	IntStatR GPIO Interrupt Status for Rising edge.  RO 0 	IO0IntStatR - 0x4002 8084
															IO2IntStatR - 0x4002 80A4
	IntStatF GPIO Interrupt Status for Falling edge. RO 0 	IO0IntStatF - 0x4002 8088
															IO2IntStatF - 0x4002 80A8
	IntClr   GPIO Interrupt Clear. 					 WO 0 	IO0IntClr   - 0x4002 808C
															IO2IntClr   - 0x4002 80AC
	IntStatus GPIO overall Interrupt Status. 		 RO 0 	IOIntStatus - 0x4002 8080


		ISER0
	18 ISE_EINT0 External Interrupt 0 Interrupt Enable.
	19 ISE_EINT1 External Interrupt 1 Interrupt Enable.
	20 ISE_EINT2 External Interrupt 2 Interrupt Enable.
	21 ISE_EINT3 External Interrupt 3 Interrupt Enable.
		Write: writing 0 has no effect, writing 1 enables the interrupt.
		Read: 0 indicates that the interrupt is disabled, 1 indicates that the interrupt is enabled.
 */


// Configuración de interrupciones externas
void initExtInt(void)
{
	set_dir(KEY0, ENTRADA);
	set_dir(KEY1, ENTRADA);
	set_dir(KEY2, ENTRADA);
	set_dir(KEY3, ENTRADA);
	SetPINSEL(KEY0, PINSEL_GPIO);
	SetPINSEL(KEY1, PINSEL_GPIO);
	SetPINSEL(KEY2, PINSEL_GPIO);
	SetPINSEL(KEY3, PINSEL_GPIO);

	EXTMODE  |=  (0x0F << 0);			// Todas por flanco
	EXTPOLAR &= ~(0x0F << 0);			// Todas por flanco descendente

	// Habilito interrupcion en GPIOs del kit
	// Falling edge
	IO2IntEnF |= (0x01 << SW4_PIN);		// SW4
	IO0IntEnF |= (0x01 << SW3_PIN); 	// SW3
	IO0IntEnF |= (0x01 << SW2_PIN);		// SW2
	IO2IntEnF |= (0x01 << SW1_PIN);		// SW1

	// Rising edge
	IO2IntEnR |= (0x01 << SW4_PIN);		// SW4
	IO0IntEnR |= (0x01 << SW3_PIN);		// SW3
	IO0IntEnR |= (0x01 << SW2_PIN);		// SW2
	IO2IntEnR |= (0x01 << SW1_PIN);		// SW1

	ISER0     |= (0x01 << NVIC_EINT3);	// Habilito Interrupcion externa 3

	// Limpio flags de interrupciones
	IO2IntClr |= (0x01 << SW4_PIN);		// SW4
	IO0IntClr |= (0x01 << SW3_PIN);		// SW3
	IO0IntClr |= (0x01 << SW2_PIN);		// SW2
	IO2IntClr |= (0x01 << SW1_PIN);		// SW1
	EXTINT    |= (0x01 << EINT3);
}


void EINT3_IRQHandler (void)
{
	if(IOIntStatus & IOInt_P0)					// interrupcion por puerto 0
	{
		if(IO0IntStatF & (0x01 << SW2_PIN))		// en P0.18, SW2, presionado
		{
			IO0IntEnF &= ~(0x01 << SW2_PIN);	// deshabilito interrupcion de SW2
			IO0IntEnR &= ~(0x01 << SW2_PIN);
			IO0IntClr |=  (0x01 << SW2_PIN);	// Limpia flag de interrupcion
			enableDebounce(SW2, key_change);				// Seteo flag del SW2
		}
		else if(IO0IntStatR & (0x01 << SW2_PIN))// en P0.18, SW2, soltado
		{
			IO0IntEnF |= (0x01 << SW2_PIN);		// habilito interrupcion de SW2
			IO0IntClr |= (0x01 << SW2_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW2, key_change);
		}
		if(IO0IntStatF & (0x01 << SW3_PIN))		// en P0.11, SW3, presionado
		{
			IO0IntEnF &= ~(0x01 << SW3_PIN);	// deshabilito interrupcion de SW3
			IO0IntEnR &= ~(0x01 << SW3_PIN);
			IO0IntClr |=  (0x01 << SW3_PIN);	// Limpia flag de interrupcion
			enableDebounce(SW3, key_change);				// Seteo flag del SW3
		}
		else if(IO0IntStatR & (0x01 << SW3_PIN))// en P0.18, SW3, soltado
		{
			IO0IntEnF |= (0x01 << SW3_PIN);		// habilito interrupcion de SW3
			IO0IntClr |= (0x01 << SW3_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW3, key_change);
		}
	}
	if(IOIntStatus & IOInt_P2)					// interrupcion por puerto 2
	{
		if(IO2IntStatF & (0x01 << SW1_PIN))		// en P2.10, SW1, presionado
		{
			IO2IntEnF &= ~(0x01 << SW1_PIN);	// deshabilito interrupcion de SW1
			IO2IntEnR &= ~(0x01 << SW1_PIN);
			IO2IntClr |=  (0x01 << SW1_PIN);	// Limpia flag de interrupcion
			enableDebounce(SW1, key_change);				// Seteo flag del SW1
		}
		else if(IO2IntStatR & (0x01 << SW1_PIN))// en P2.10, SW1, soltado
		{
			IO2IntEnF |= (0x01 << SW1_PIN);		// habilito interrupcion de SW1
			IO2IntClr |= (0x01 << SW1_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW1, key_change);
		}
		if(IO2IntStatF & (0x01 << SW4_PIN))		// en P2.13, SW4, presionado
		{
			IO2IntEnF &= ~(0x01 << SW4_PIN);	// deshabilito interrupcion de SW4
			IO2IntEnR &= ~(0x01 << SW4_PIN);
			IO2IntClr |=  (0x01 << SW4_PIN);	// Limpia flag de interrupcion
			enableDebounce(SW4, key_change);				// Seteo flag del SW4
		}
		else if(IO2IntStatR & (0x01 << SW4_PIN))// en P2.13, SW4, soltado
		{
			IO2IntEnF |= (0x01 << SW4_PIN);		// habilito interrupcion de SW4
			IO2IntClr |= (0x01 << SW4_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW4, key_change);
		}
	}

	EXTINT |= (0x01 << EINT3);		// Limpia flag de interrupción
}
