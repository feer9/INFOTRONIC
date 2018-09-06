#include "UART.h"

__RW uint8_t bufferRx[BUFFER_RX_SIZE];
__RW uint8_t bufferTx[BUFFER_TX_SIZE];

__RW uint8_t indexRxIn  = 0;
__RW uint8_t indexRxOut = 0;
__RW uint8_t indexTxIn  = 0;
__RW uint8_t indexTxOut = 0;

__RW uint8_t bufferRxFull  = 0;
__RW uint8_t bufferRxEmpty = 1;
__RW uint8_t bufferTxFull  = 0;
__RW uint8_t bufferTxEmpty = 1;

__RW uint8_t TxStart = 0;

void UART0_IRQHandler()
{
	uint8_t iir, int_id;
	int16_t data;

	do {
		iir = (uint8_t) U0IIR;
		int_id = (iir & 0x06);

		if(int_id == 0x02) // TX
		{
			data = popTx();
			if(data != -1)
				U0THR = (uint8_t) data;
			else
				TxStart = 0;
		}
		if(int_id == 0x04) // RX
		{
			data = U0RBR;
			pushRx((uint8_t) data);
		}
		if(int_id == 0x06) // error: analizar LSR
		{
			int_id = (uint8_t) U0LSR;
			// do stuff
		}
	}
	while(!(iir & 0x01));
}

void UART0_init()//uint32_t baudrate)
{
	PCONP |= (0x01 << PCONP_UART0);

	PCLKSEL0 &= ~(0x03 << PCLKSEL_UART0);
	PCLKSEL0 |= (PCLK_CCLK_4 << PCLKSEL_UART0);

	U0LCR = 0b10001011;
	// 8-bit ; 1 stop bit ; parity enabled ; odd parity (impar) ; break control disabled ; DLAB = 1

	// Configuracion a 115200 baudios
	U0DLM = 0;
	U0DLL = 9;
	U0FDR = 0x01 | (0x02 << 4);
//	DivAddVal = 1 ; MulVal = 2

	// enable FIFO Rx & Tx
	U0FCR = 0x07;

	set_dir(Tx0, SALIDA);
	set_dir(Rx0, ENTRADA);
	// pinsel pins P0,2 y P0,3 (Tx0 y Rx0)
	setPINSEL(Tx0, PINSEL_FUNC1);
	setPINSEL(Rx0, PINSEL_FUNC1);
	setPINMODE(Tx0, PINMODE_PULLUP);
	setPINMODE(Rx0, PINMODE_PULLUP);

	// habilito las interrupciones
	// (DLAB = 0 para acceder a IER)
	U0LCR &= ~(0x01 << 7);
	// interrupciones por RBR, THRE y RX Line Status
	U0IER = 0x07;
	// y por ultimo en el NVIC
	ISER0 |= (0x01 << NVIC_UART0);
}

uint8_t pushRx(uint8_t data)
{
	if(bufferRxFull)
		return 1;

	bufferRx[indexRxIn] = data;

	indexRxIn++;
	indexRxIn %= BUFFER_RX_SIZE;

	bufferRxEmpty = 0;
	if(indexRxIn == indexRxOut)
		bufferRxFull = 1;

	return 0;
}

int16_t popTx(void)
{
	int16_t data = -1;

	if(!bufferTxEmpty)
	{
		data = bufferTx[indexTxOut];

		indexTxOut++;
		indexTxOut %= BUFFER_TX_SIZE;

		bufferTxFull = 0;
		if(indexTxIn == indexTxOut)
			bufferTxEmpty = 1;
	}

	return data;
}
//  drivers:
// void pushRx(uint8_t dato) y int32_t popTx(void)

//  primitivas:
// int32_t popRx(void) y void pushTx(uint32_t dato)
// void transmitir(char *p)
