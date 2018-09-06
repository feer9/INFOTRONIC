#include <cr_section_macros.h>
#include <Aplicacion.h>

/* TODO: mejorar el driver del LCD para que procese strings con desplazamiento
 * quiero configurar la uart para usarla con la interfaz usb
 * y poder mandar cosas a la pc con qt serial esa wea
 **/

extern __RW uint8_t ledStatus;
uint8_t displayClockStatus = OFF;
uint8_t LCD_status = ON;


int main(void)
{
	inicializarKit();
	ledOFF();
	LCD_printCentered("WELCOME", LCD_ROW_1);
	setBlinkTimers();
	startTimer(1, 1000); // reloj en 1s
	while(1) {}
}

void timerEnd_Handler(uint8_t n)
{
	switch (n)
	{
	case 6:
		write_pin(LEDXpresso, LEDLPC_OFF);
		setBlinkTimers();
		break;
	case 7:
		write_pin(LEDXpresso, LEDLPC_ON);
		break;

	case 1:	// led apagado, encender display de reloj
		if(displayClockStatus == OFF)
		{
			LCD_displayClock();
			displayClockStatus = ON;
		}
		break;
	case 0:
		break;
	default:
		break;
	}
}

void setBlinkTimers()
{
	startTimer(6, 5000);
	startTimer(7, 4900);
}

void pressedKey(uint8_t key)
{
	switch(key)
	{
	case SW1:
		ledON();
		if(LCD_status)
		{
			displayClockStatus = OFF;
			LCD_clear();
			LCD_print("Led ON");
			startTimer(1, 1000);
		}
		break;

	case SW2:
		ledOFF();
		if(LCD_status)
		{
			displayClockStatus = OFF;
			LCD_clear();
			LCD_print("Led OFF");
			startTimer(1, 1000);
		}
		break;

	case SW3:
		if(ledStatus)
		{
			ledUP();
			if(LCD_status)
			{
				displayClockStatus = OFF;
				LCD_clear();
				LCD_printCentered("Swap", LCD_ROW_1);
				startTimer(1, 2000);
			}
		}
		else
			UART0_sendString("caca pepe");
		break;

	case SW4:
		break;

#if _5_ENTRADAS
	case SW5:

		break;
#endif
	default:
		break;
	}
}

void releasedKey(uint8_t key)
{
	if(key == SW4)
	{
//		stopTimer(0);
		if(LCD_status)
		{
			LCD_OFF();
			LCD_status = OFF;
		}
		else
		{
			LCD_clear();
			LCD_ON();
			LCD_status = ON;
			displayClockStatus = ON;
			LCD_displayClock();
		}
	}
}
