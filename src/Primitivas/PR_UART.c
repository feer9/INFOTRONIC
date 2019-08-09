#include <string.h>
#include "../Drivers/UART.h"
#include "../Drivers/LCD.h"
#include "../Aplicacion/Aplicacion.h"

static int16_t	popRx(void);
static uint8_t	pushTx(uint8_t data);
static uint8_t	pushTx_16(uint8_t data[17]);
void sendPackage(uint8_t data[17]);

static uart_t* uart0;

void UART0_setStruct(uart_t *puart0) {
	uart0 = puart0;
}

inline bool UART0_isUp(void) {
	return uart0->status;
}

inline bool UART0_isRXBuffEmpty(void) {
	return uart0->bufferRx.isEmpty();
}

inline bool UART0_isRXBuffFull(void) {
	return uart0->bufferRx.isFull();
}

inline bool UART0_isTXBuffEmpty(void) {
	return uart0->bufferTx.isEmpty();
}

inline bool UART0_isTXBuffFull(void) {
	return uart0->bufferTx.isFull();
}

status UART0_sendChar(char c)
{
	return pushTx(c);
}

size_t UART0_sendString(char *msg)
{
	size_t i;

	for(i=0; msg[i]; i++)
	{
		if( pushTx(msg[i]) != SUCCESS )
			break;
	}
	return i;
}

// return n bytes sent
size_t UART0_send(uint8_t *msg, size_t len)
{
	uint8_t buf[17] = {0};
	size_t i, remain = len%16;
	len -= remain;
	for(i = 0; i < len; i += 16) {
		if(pushTx_16(msg+i) != SUCCESS)
			return i;
	}
	memcpy(buf, msg+len, remain);
	return (pushTx_16(buf) == SUCCESS) ? i+remain : i;
}

size_t UART0_sendRequest(UART0_requests_t id)
{
	size_t ret = 0;
	switch(id)
	{
	case UART0_REQUEST_TIME:
		ret = UART0_send((uint8_t*)"<RT>", 4);
		break;
	default:
		break;
	}

	return ret;
}

inline void UART0_requestTime(void)
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

	if(!uart0->status)
		return;

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

	if(!uart0->bufferRx.isEmpty())
	{
		data = uart0->bufferRx.item[uart0->bufferRx.indexOut];

		uart0->bufferRx.indexOut++;
		uart0->bufferRx.indexOut %= BUFFER_UART_SIZE;

		uart0->bufferRx.quantity--;
	}

	return data;
}

static uint8_t pushTx(uint8_t data)
{
	if(!uart0->TxStart)
	{
		U0THR = data;
		uart0->TxStart = true;
	}
	else
	{
		if(uart0->bufferTx.isFull())
			return ERROR;

		uart0->bufferTx.item[uart0->bufferTx.indexIn] = data;

		uart0->bufferTx.indexIn++;
		uart0->bufferTx.indexIn %= BUFFER_UART_SIZE;

		uart0->bufferTx.quantity++;
	}

	return SUCCESS;
}


static uint8_t pushTx_16(uint8_t data[17])
{
	uint8_t i;
	if(! uart0->TxStart )
	{
		sendPackage(data);
		uart0->TxStart = true;
	}
	else
	{
		if(uart0->bufferTx.quantity + 16 > BUFFER_UART_SIZE)
			return ERROR;

		for(i=0; i<16; i++)
		{
			uart0->bufferTx.item[uart0->bufferTx.indexIn] = data[i];

			uart0->bufferTx.indexIn++;
			uart0->bufferTx.indexIn %= BUFFER_UART_SIZE;
		}
		uart0->bufferTx.quantity += 16;
	}

	return SUCCESS;
}
