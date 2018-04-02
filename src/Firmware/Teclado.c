#include <TPO.h>

__RW uint8_t tick = 0;
__RW uint8_t g_Teclado = 0;
__RW uint8_t FlagTeclado = 0;
     uint8_t ledActual = 0;


void Teclado()
{
#if _5_ENTRADAS
	static uint8_t blink = OFF;

	if(tick && blink)
	{
		tick = 0;
		ToggleLED(ledActual);
	}
#endif

	if(g_Teclado)
	{
		switch(g_Teclado)
		{
		case 1:
			ledActual = 0;
			SetLED(0);
			break;
		case 2:
			ledDOWN();
			break;
		case 3:
			ledUP();
			break;
		case 4:
			ledActual = 3;
			SetLED(3);
			break;
#if _5_ENTRADAS
		case 5:
			if(!blink) {
				blink = ON;
				tick = 0;
			}
			else {
				blink = OFF;
				SetLED(ledActual);
			}
			break;
#endif
		default:
			break;
		}
		g_Teclado = 0;
	}
}

void Debounce_Teclado(void)
{
	static uint8_t t[N_ENTRADAS] = {0,0,0,0};
	static uint8_t contador[N_ENTRADAS] = {0,0,0,0};
	static uint8_t debounceActivo = 0;
	static uint8_t sw5ant = 0;
	uint8_t sw5 = 0;

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
						g_Teclado = i+1;
					contador[i] = 0;
					debounceActivo --;
					// habilito interrupciones por soltar tecla
					IO0IntEnR |= (0x01 << 18) | (0x01 << 11); 	// SW2 y SW3
					IO2IntEnR |= (0x01 << 10) | (0x01 << 13);	// SW1 y SW4
				}
			}
		}
	}
#if _5_ENTRADAS
	if(!debounceActivo)
	{
		sw5 = read(1,26, ACTIVO_BAJO);
		if(!sw5ant && sw5)
			FlagTeclado = 5;
		sw5ant = sw5;
	}
#endif

}

/*	#define		KEY0		PORT2,10		//SW1
	#define		KEY1		PORT0,18		//SW2
	#define		KEY2		PORT0,11		//SW3
	#define		KEY3_RC		PORT2,13		//SW4
	#define		KEY4_RC		PORT1,26		//SW5 */

uint8_t readSW(uint8_t n)
{
	switch(n)
	{
	case 1: return read(2,10, ACTIVO_BAJO);	// SW1
	case 2:	return read(0,18, ACTIVO_BAJO);	// SW2
	case 3:	return read(0,11, ACTIVO_BAJO);	// SW3
	case 4:	return read(2,13, ACTIVO_BAJO);	// SW4
	case 5: return read(1,26, ACTIVO_BAJO); // SW5
	default: return 0;
	}
}


void ledUP(void)
{
	if(++ledActual == 4)
		ledActual = 0;
	SetLED(ledActual);
}

void ledDOWN(void)
{
	if(!ledActual)
		ledActual = 4;
	ledActual--;
	SetLED(ledActual);
}

void SetLED(uint8_t led)
{
	write_pin(2, 0, OFF);
	write_pin(0,23, OFF);
	write_pin(0,21, OFF);
	write_pin(0,27, OFF);

	switch(led)
	{
	case 0: write_pin(2, 0, ON); break;
	case 1: write_pin(0,23, ON); break;
	case 2: write_pin(0,21, ON); break;
	case 3: write_pin(0,27, ON); break;
	default: break;
	}
}

void ToggleLED(uint8_t led)
{
	switch(led)
	{
	case 0: toggle_pin(2, 0); break;
	case 1: toggle_pin(0,23); break;
	case 2: toggle_pin(0,21); break;
	case 3: toggle_pin(0,27); break;
	default: break;
	}
}
/*
	// Leds de Reles
	#define		LED1		PORT2,0 		//GPIO2
	#define		LED2		PORT0,23		//GPIO0
	#define		LED3		PORT0,21		//GPIO0
	#define		LED4		PORT0,27		//GPIO0


  	#define		LCD_D4		PORT0,5			//GPIO0
	#define		LCD_D5		PORT0,10		//GPIO0
	#define		LCD_D6		PORT2,4			//GPIO2
	#define		LCD_D7		PORT2,5			//GPIO2

	#define		LCD_RS		PORT2,6			//GPIO2
	#define		LCD_BF		PORT0,28		//GPIO1
	#define		LCD_E		PORT0,4			//GPIO0
*/


void pressedKey(uint8_t key)
{
/*	static uint32_t cont = 0;

	switch(key)
	{
	case TECLA_UP:
		if(++cont == 1000000)
			cont = 0;
		ledUP();
		break;

	case TECLA_DOWN:
		if (!cont)
			cont = 999999;
		else
			cont--;
		ledDOWN();
		break;
	}

	Display (cont);
*/
}

void releasedKey(uint8_t i)
{

}
