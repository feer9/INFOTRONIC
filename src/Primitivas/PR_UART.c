#include "../Drivers/UART.h"
#include "../Drivers/LCD.h"
#include "../Aplicacion/Aplicacion.h"
#include "../Drivers/string.h"

static int16_t	popRx(void);
static uint8_t	pushTx(uint8_t data);

extern uart_t uart0;

uint8_t UART0_sendChar(char c)
{
	if( pushTx(c) != OK )
		return ERROR;

	return OK;
}

uint8_t UART0_sendString(char *msg)
{
	uint32_t i;

	for(i=0; msg[i]; i++)
	{
		if( pushTx(msg[i]) != OK )
			return ERROR;
	}
	return OK;
}

uint8_t UART0_sendRequest(uint8_t id)
{
	char msg[20];
	switch(id)
	{
	case UART0_REQUEST_TIME:
		strcpy(msg, "<RT>");
		break;
	}
	return UART0_sendString(msg);
}

inline void UART0_requestTime()
{
	UART0_sendRequest(UART0_REQUEST_TIME);
}

void UART0_receive(void)
{
	int16_t data;
	char c;
	static uint32_t i=0;
	static bool recibiendoTrama = FALSE;
	static char msg[LCD_MAX_MSG_SIZE];

	if ((data = popRx()) != -1)
	{
		c = (char) data;
		if(recibiendoTrama)
		{
			if(c == '>')
			{
				recibiendoTrama = FALSE;
				msg[i] = '\0';
				i = 0;

				tramaRecibida(msg);
			}
			else
			{
				msg[i++] = c;
			}
		}
		else
		{
			if(c == '<')
			{
				i = 0;
				recibiendoTrama = TRUE;
			}
			else if(c == '\r')
			{
				msg[i] = '\0';
				LCD_printReceived(msg);
				i = 0;
			}
			else
			{
				msg[i++] = c;
			}
		}

		if(i >= LCD_MAX_MSG_SIZE-1)
		{
			msg[i] = '\0';
			if(recibiendoTrama) // ?? trama enorme?
				recibiendoTrama = FALSE;

			else
				LCD_printReceived(msg);

			i = 0;
		}
	}
}

static int16_t popRx(void)
{
	int16_t data = -1;

	if(!uart0.bufferRxEmpty)
	{
		data = uart0.bufferRx[uart0.indexRxOut];

		uart0.indexRxOut++;
		uart0.indexRxOut %= BUFFER_RX_SIZE;

		uart0.bufferRxFull = 0;
		if(uart0.indexRxIn == uart0.indexRxOut)
			uart0.bufferRxEmpty = 1;
	}

	return data;
}

static uint8_t pushTx(uint8_t data)
{
	if(uart0.bufferTxFull)
		return ERROR;

	uart0.bufferTx[uart0.indexTxIn] = data;

	uart0.indexTxIn++;
	uart0.indexTxIn %= BUFFER_TX_SIZE;

	uart0.bufferTxEmpty = 0;
	if(uart0.indexTxIn == uart0.indexTxOut)
		uart0.bufferTxFull = 1;

	UART0_startTx();

	return OK;
}
