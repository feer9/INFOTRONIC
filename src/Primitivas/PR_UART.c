#include <UART.h>
#include "LCD.h"

extern __RW uint8_t bufferRx[BUFFER_RX_SIZE];
extern __RW uint8_t bufferTx[BUFFER_TX_SIZE];

extern __RW uint8_t indexRxIn;
extern __RW uint8_t indexRxOut;
extern __RW uint8_t indexTxIn;
extern __RW uint8_t indexTxOut;

extern __RW uint8_t bufferRxFull;
extern __RW uint8_t bufferRxEmpty;
extern __RW uint8_t bufferTxFull;
extern __RW uint8_t bufferTxEmpty;

extern __RW uint8_t TxStart;

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
	static char msg[33];
	while((data = popRx()) != -1)
	{
		if(data == '\r')
		{
			msg[i] = '\0';
			LCD_clear();
			LCD_print(msg);
			i=0;
			break;
		}
		msg[i] = (char) data;
		if(i > 30)
		{
			msg[32] = '\0';
			LCD_clear();
			LCD_print(msg);
			i=0;
			break;
		}
		// lo hice como el orto pero weno tengo sueño
		i++;
	}
}

int16_t popRx(void)
{
	int16_t data = -1;

	if(!bufferRxEmpty)
	{
		data = bufferRx[indexRxOut];

		indexRxOut++;
		indexRxOut %= BUFFER_RX_SIZE;

		bufferRxFull = 0;
		if(indexRxIn == indexRxOut)
			bufferRxEmpty = 1;
	}

	return data;
}

uint8_t pushTx(uint8_t data)
{
	if(bufferTxFull)
		return 1;

	bufferTx[indexTxIn] = data;

	indexTxIn++;
	indexTxIn %= BUFFER_TX_SIZE;

	bufferTxEmpty = 0;
	if(indexTxIn == indexTxOut)
		bufferTxFull = 1;

	if(!TxStart)
	{
		U0THR = popTx();
		TxStart = 1;
	}

	return 0;
}
