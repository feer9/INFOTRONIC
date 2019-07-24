#include "Teclado.h"
#include "GPIO.h"

static uint8_t readSW(uint8_t);
static void    enableReleaseKeyInt(uint8_t);

static __RW uint8_t key_change = 0;

// TODO: variable global que habilite/deshabilite las teclas

void debounceTeclado(void)
{
	uint8_t i;
#if _5_TECLAS
	static uint8_t sw5ant = 0;
	uint8_t sw5 = 0;
	static uint8_t t[N_TECLAS] = {0,0,0,0,0};
	static uint8_t contador[N_TECLAS] = {0,0,0,0,0};
#else
	static uint8_t t[N_TECLAS] = {0,0,0,0};
	static uint8_t contador[N_TECLAS] = {0,0,0,0};
#endif
	static uint8_t debounceActivo = 0;
	static uint8_t stAnt = 0;
	static void (*keyHandler[N_TECLAS])(uint8_t) = {
			SW1_handler,
			SW2_handler,
			SW3_handler,
			SW4_handler };

	if(key_change)							// si se genero interrupcion por teclado
	{
		for(i=0; i<N_TECLAS; i++)
		{
			if(key_change & _BIT(i))
			{
				t[i] = T_DEBOUNCE;			// seteo temporizacion T_DEBOUNCE * 2,5ms (=50ms)
				key_change _RESET_BIT(i);	// limpio flag
				debounceActivo ++;			// seteo flag de que estoy haciendo el debounce
			}
		}
	}
	if(debounceActivo)
	{
		for(i=0; i<N_TECLAS; i++)
		{
			if(t[i])
			{
				t[i] --;
				contador[i] += (readSW(i+1) ^ ((stAnt >> i) & 0x1));
				if(!t[i])
				{
					if(contador[i] > ACEPTAReSTADO)
					{
						stAnt ^= (0x1 << i);
						keyHandler[i]((stAnt >> i) & 0x1);
					}
					contador[i] = 0;
					debounceActivo --;
					// habilito interrupciones por soltar tecla
					enableReleaseKeyInt(i);
				}
			}
		}
	}
#if _5_TECLAS
	if(!debounceActivo)
	{
		sw5 = read_pin(KEY4_RC, ACTIVO_BAJO);
		if(!sw5ant && sw5)
			key_change = 5;
		sw5ant = sw5;
	}
#endif

}

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

	EXTMODE  |=  (0x0FUL << 0);			// Todas por flanco
	EXTPOLAR &= ~(0x0FUL << 0);			// Todas por flanco descendente

	// Habilito interrupcion en GPIOs del kit
	// Falling edge
	IO2IntEnF |= (1UL << SW4_PIN);		// SW4
	IO0IntEnF |= (1UL << SW3_PIN); 		// SW3
	IO0IntEnF |= (1UL << SW2_PIN);		// SW2
	IO2IntEnF |= (1UL << SW1_PIN);		// SW1

	// Rising edge
	IO2IntEnR |= (1UL << SW4_PIN);		// SW4
	IO0IntEnR |= (1UL << SW3_PIN);		// SW3
	IO0IntEnR |= (1UL << SW2_PIN);		// SW2
	IO2IntEnR |= (1UL << SW1_PIN);		// SW1

	ISER0      = NVIC_EINT3;			// Habilito Interrupcion externa 3

	// Limpio flags de interrupciones
	IO2IntClr |= (1UL << SW4_PIN);		// SW4
	IO0IntClr |= (1UL << SW3_PIN);		// SW3
	IO0IntClr |= (1UL << SW2_PIN);		// SW2
	IO2IntClr |= (1UL << SW1_PIN);		// SW1
	EXTINT    |= (1UL << EINT3);
}


void EINT3_IRQHandler (void)
{
	if(IOIntStatus & IOInt_P0)					// interrupcion por puerto 0
	{
		if(IO0IntStatF & (1UL << SW2_PIN))		// en P0.18, SW2, presionado
		{
			IO0IntEnF &= ~(1UL << SW2_PIN);		// deshabilito interrupcion de SW2
			IO0IntEnR &= ~(1UL << SW2_PIN);
			IO0IntClr |=  (1UL << SW2_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW2, key_change);	// Seteo flag del SW2
		}
		else if(IO0IntStatR & (1UL << SW2_PIN))	// en P0.18, SW2, soltado
		{
			IO0IntEnF |= (1UL << SW2_PIN);		// habilito interrupcion de SW2
			IO0IntClr |= (1UL << SW2_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW2, key_change);
		}
		if(IO0IntStatF & (1UL << SW3_PIN))		// en P0.11, SW3, presionado
		{
			IO0IntEnF &= ~(1UL << SW3_PIN);		// deshabilito interrupcion de SW3
			IO0IntEnR &= ~(1UL << SW3_PIN);
			IO0IntClr |=  (1UL << SW3_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW3, key_change);	// Seteo flag del SW3
		}
		else if(IO0IntStatR & (1UL << SW3_PIN))	// en P0.18, SW3, soltado
		{
			IO0IntEnF |= (1UL << SW3_PIN);		// habilito interrupcion de SW3
			IO0IntClr |= (1UL << SW3_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW3, key_change);
		}
	}
	if(IOIntStatus & IOInt_P2)					// interrupcion por puerto 2
	{
		if(IO2IntStatF & (1UL << SW1_PIN))		// en P2.10, SW1, presionado
		{
			IO2IntEnF &= ~(1UL << SW1_PIN);		// deshabilito interrupcion de SW1
			IO2IntEnR &= ~(1UL << SW1_PIN);
			IO2IntClr |=  (1UL << SW1_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW1, key_change);	// Seteo flag del SW1
		}
		else if(IO2IntStatR & (1UL << SW1_PIN))	// en P2.10, SW1, soltado
		{
			IO2IntEnF |= (1UL << SW1_PIN);		// habilito interrupcion de SW1
			IO2IntClr |= (1UL << SW1_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW1, key_change);
		}
		if(IO2IntStatF & (1UL << SW4_PIN))		// en P2.13, SW4, presionado
		{
			IO2IntEnF &= ~(1UL << SW4_PIN);		// deshabilito interrupcion de SW4
			IO2IntEnR &= ~(1UL << SW4_PIN);
			IO2IntClr |=  (1UL << SW4_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW4, key_change);	// Seteo flag del SW4
		}
		else if(IO2IntStatR & (1UL << SW4_PIN))	// en P2.13, SW4, soltado
		{
			IO2IntEnF |= (1UL << SW4_PIN);		// habilito interrupcion de SW4
			IO2IntClr |= (1UL << SW4_PIN);		// Limpia flag de interrupcion
			enableDebounce(SW4, key_change);
		}
	}

	EXTINT |= (1UL << EINT3);		// Limpia flag de interrupción
	debounceTeclado();				// Comienzo el debounce
}


static uint8_t readSW(uint8_t n)
{
	switch(n)
	{
	case 1: return read_pin(KEY0, ACTIVO_BAJO);	// SW1
	case 2:	return read_pin(KEY1, ACTIVO_BAJO);	// SW2
	case 3:	return read_pin(KEY2, ACTIVO_BAJO);	// SW3
	case 4:	return read_pin(KEY3, ACTIVO_BAJO);	// SW4
	case 5: return read_pin(KEY4, ACTIVO_BAJO);	// SW5
	default: return 0;
	}
}

static void enableReleaseKeyInt(uint8_t i)
{
	switch (i)
	{
	case 0: IO2IntEnR |= (1UL << SW1_PIN); break; // SW1
	case 1: IO0IntEnR |= (1UL << SW2_PIN); break; // SW2
	case 2: IO0IntEnR |= (1UL << SW3_PIN); break; // SW3
	case 3: IO2IntEnR |= (1UL << SW4_PIN); break; // SW4
	default: break;
	}
}
