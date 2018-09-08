#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include "GPIO.h"
#include "Teclado.h"

__RW uint8_t key_change = 0;


// Configuración de interrupciones externas
void ExtInt_init(void)
{
	set_dir(KEY0, ENTRADA);
	set_dir(KEY1, ENTRADA);
	set_dir(KEY2, ENTRADA);
	set_dir(KEY3, ENTRADA);
	setPINSEL(KEY0, PINSEL_GPIO);
	setPINSEL(KEY1, PINSEL_GPIO);
	setPINSEL(KEY2, PINSEL_GPIO);
	setPINSEL(KEY3, PINSEL_GPIO);

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
