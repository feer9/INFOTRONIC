#include <string.h>
#include "../Drivers/UART.h"
#include "../Drivers/LCD.h"
#include "../Aplicacion/Aplicacion.h"

static int16_t	popRx(void);
static uint8_t	pushTx(uint8_t data);
static uint8_t	UART0_pushTx_16(char data[17]);
void sendPackage(char data[17]);

extern uart_t uart0;

uint8_t UART0_sendChar(char c)
{
	if( pushTx(c) != SUCCESS )
		return ERROR;

	return SUCCESS;
}

uint8_t UART0_sendString(char *msg)
{
	uint32_t i;

	for(i=0; msg[i]; i++)
	{
		if( pushTx(msg[i]) != SUCCESS )
			return ERROR;
	}
	return SUCCESS;
}

uint8_t UART0_sendRequest(uint8_t id)
{
	uint8_t ret = 0;
	switch(id)
	{
	case UART0_REQUEST_TIME:
		ret = UART0_pushTx_16("<RT>");
		//TODO: si el mensaje es mas corto que 16 caracteres, llenar con 0xFFs
		break;
	}

	return ret;
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
	static bool recibiendoTrama = false;
	static char msg[LCD_MAX_MSG_SIZE];

	if ((data = popRx()) != -1)
	{
		c = (char) data;
		if(recibiendoTrama)
		{
			if(c == '>')
			{
				recibiendoTrama = false;
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
				recibiendoTrama = true;
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
				recibiendoTrama = false;

			else
				LCD_printReceived(msg);

			i = 0;
		}
	}
}

static int16_t popRx(void)
{
	int16_t data = -1;

	if(!uart0.bufferRx.isEmpty())
	{
		data = uart0.bufferRx.item[uart0.bufferRx.indexOut];

		uart0.bufferRx.indexOut++;
		uart0.bufferRx.indexOut %= BUFFER_UART_SIZE;

		uart0.bufferRx.quantity--;
	}

	return data;
}

static uint8_t pushTx(uint8_t data)
{
	if(!uart0.TxStart)
	{
		U0THR = data;
		uart0.TxStart = true;
	}
	else
	{
		if(uart0.bufferTx.isFull())
			return ERROR;

		uart0.bufferTx.item[uart0.bufferTx.indexIn] = data;

		uart0.bufferTx.indexIn++;
		uart0.bufferTx.indexIn %= BUFFER_UART_SIZE;

		uart0.bufferTx.quantity++;
	}

	return SUCCESS;
}


static uint8_t UART0_pushTx_16(char data[17])
{
	uint8_t i;
	if(! uart0.TxStart )
	{
		sendPackage(data);
		uart0.TxStart = true;
	}
	else
	{
		if(uart0.bufferTx.quantity + 16 > BUFFER_UART_SIZE)
			return ERROR;

		for(i=0; i<16; i++)
		{
			uart0.bufferTx.item[uart0.bufferTx.indexIn] = data[i];

			uart0.bufferTx.indexIn++;
			uart0.bufferTx.indexIn %= BUFFER_UART_SIZE;
		}
		uart0.bufferTx.quantity += 16;
	}

	return SUCCESS;
}
