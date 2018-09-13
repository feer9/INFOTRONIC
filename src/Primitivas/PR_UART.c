#include "../Drivers/UART.h"
#include "../Drivers/LCD.h"

static int16_t	popRx(void);
static uint8_t	pushTx(uint8_t data);

extern uart_t uart0;

uint8_t UART0_sendChar(char c)
{
	if( pushTx(c) )
		return 1;

	return 0;
}

uint8_t UART0_sendString(char *msg)
{
	uint32_t i;

	for(i=0; msg[i]; i++)
	{
		if( pushTx(msg[i]) )
			return 1;
	}
	return 0;
}

void UART0_receive(void)
{
	int16_t data;
	static uint32_t i=0;
	static char msg[LCD_MAX_MSG_SIZE];
	while((data = popRx()) != -1)
	{
		if(data == '\r')
		{
			if(i)
			{
				msg[i] = '\0';
				LCD_printReceived(msg);
				i=0;
			}
			continue;
		}
		msg[i] = (char) data;
/*		if(i+1 >= LCD_MAX_MSG_SIZE)
		{
			msg[i+1] = '\0';
			LCD_printReceived(msg);
			i=0;
			continue;
		}*/
		i++;
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
		return 1;

	uart0.bufferTx[uart0.indexTxIn] = data;

	uart0.indexTxIn++;
	uart0.indexTxIn %= BUFFER_TX_SIZE;

	uart0.bufferTxEmpty = 0;
	if(uart0.indexTxIn == uart0.indexTxOut)
		uart0.bufferTxFull = 1;

	UART0_startTx();

	return 0;
}
