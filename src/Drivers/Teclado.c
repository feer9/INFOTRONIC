#include <Aplicacion.h>


__RW uint8_t g_Teclado = 0;
__RW uint8_t FlagTeclado = 0;
extern __RW uint8_t LCD_Action;

void Debounce_Teclado(void)
{
#if _5_ENTRADAS
	static uint8_t sw5ant = 0;
	uint8_t sw5 = 0;
	static uint8_t t[N_ENTRADAS] = {0,0,0,0,0};
	static uint8_t contador[N_ENTRADAS] = {0,0,0,0,0};
#else
	static uint8_t t[N_ENTRADAS] = {0,0,0,0};
	static uint8_t contador[N_ENTRADAS] = {0,0,0,0};
#endif
	static uint8_t debounceActivo = 0;

	if(FlagTeclado)							// si se genero interrupcion por teclado
	{
		t[FlagTeclado - 1] = T_DEBOUNCE;	// seteo temporizacion T_DEBOUNCE * 2,5ms (=50ms)
		FlagTeclado = 0;					// limpio flag
		debounceActivo ++;					// seteo flag de que estoy haciendo el debounce
	}
	if(debounceActivo)
	{
		uint8_t i;
		for(i=0; i<N_ENTRADAS; i++)
		{
			if(t[i])
			{
				t[i] --;
				contador[i] += readSW(i+1);
				if(!t[i])
				{
					if(contador[i] > ACEPTAReSTADO)
					{
						g_Teclado = i+1;
//						LCD_Action = g_Teclado;
					}
					contador[i] = 0;
					debounceActivo --;
					// habilito interrupciones por soltar tecla
					enableReleaseKeyInt(i);
				}
			}
		}
	}
#if _5_ENTRADAS
	if(!debounceActivo)
	{
		sw5 = read_pin(KEY4_RC, ACTIVO_BAJO);
		if(!sw5ant && sw5)
			FlagTeclado = 5;
		sw5ant = sw5;
	}
#endif

}

uint8_t readSW(uint8_t n)
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

void enableReleaseKeyInt(uint8_t i)
{
	switch (i)
	{
	case 0: IO2IntEnR |= (0x01 << SW1_PIN); break; // SW1
	case 1: IO0IntEnR |= (0x01 << SW2_PIN); break; // SW2
	case 2: IO0IntEnR |= (0x01 << SW3_PIN); break; // SW3
	case 3: IO2IntEnR |= (0x01 << SW4_PIN); break; // SW4
	default: break;
	}
}
