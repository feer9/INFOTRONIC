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

	case SW4:
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
		{
			int err = 0, msjs=0;
			char str[17] = "err: ";
			char aux[4];
			for(int i=0; i < BUFFER_TX_SIZE/2 + 1; i++) {
				err += UART0_sendChar(0x55);
				err += UART0_sendChar(0xAA);
				msjs += 2;
			}
			LCD_clear();
			LCD_printUP(strcat(str, intToStr(err, aux, 1)));
			strcpy(str,"buf:");
			strcat(str, intToStr(BUFFER_TX_SIZE, aux, 1));
			strcat(str," msjs:");
			strcat(str, intToStr(msjs, aux, 1));
			LCD_printDOWN(str);
		}
		break;

	case SW3:
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
	if(key == SW3)
	{
//		stopTimer(0);
		if(displayClockStatus)
		{
			//LCD_OFF();
			LCD_status = OFF;
			displayClockStatus = OFF;
			LCD_clear();
		}
		else
		{
		//	LCD_ON();
			LCD_status = ON;
			displayClockStatus = ON;
			LCD_displayClock();
		}
	}
}
