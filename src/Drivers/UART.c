#include "UART.h"

static uint8_t	pushRx(uint8_t data);
static int16_t	popTx(void);

uart_t uart0 = {
		.indexRxIn  = 0,
		.indexRxOut = 0,
		.indexTxIn  = 0,
		.indexTxOut = 0,
		.bufferRxFull  = 0,
		.bufferRxEmpty = 1,
		.bufferTxFull  = 0,
		.bufferTxEmpty = 1,
		.TxStart = 0,
		.status = U0_INIT_STATUS
};
/*
void UART0_IRQHandler()
{
	uint8_t iir, intId;
	int16_t data;

	do
	{
		iir = (uint8_t) U0IIR;
		intId = (iir  >> 1) & 0x03;

		if(intId == 0x03) // error: analizar LSR
		{
			data = U0LSR;
			// do stuff
		}
		else if(intId == 0x02) // RX data available
		{
			data = U0RBR;
			pushRx((uint8_t) data);
		}
		else if(intId == 0x01) // THRE (TX)
		{
			data = popTx();
			if(data != -1)
				U0THR = (uint8_t) data;
			else
				uart0.TxStart = 0;
		}
	}
	while(!(iir & 0x01));
}*/

void UART0_IRQHandler()
{
	uint8_t intId;
	int16_t data;

	intId = (uint8_t) (U0IIR  >> 1) & 0x03;

	if(intId == 0x03) // error: analizar LSR
	{
		data = U0LSR;
		// do stuff
	}
	else if(intId == 0x02) // RX data available
	{
		data = U0RBR;
		pushRx((uint8_t) data);
	}
	else if(intId == 0x01) // THRE (TX)
	{
		data = popTx();
		if(data != -1)
			U0THR = (uint8_t) data;
		else
			uart0.TxStart = 0;
	}
}

void UART0_init(uint8_t st)//,uint32_t baudrate)
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
//	setPINSEL(Tx0, PINSEL_FUNC1);
//	setPINSEL(Rx0, PINSEL_FUNC1);
	setPINMODE(Tx0, PINMODE_PULLUP);
	setPINMODE(Rx0, PINMODE_PULLUP);

	// habilito las interrupciones
	// (DLAB = 0 para acceder a IER)
	U0LCR &= ~(0x01 << 7);
	// interrupciones por RBR, THRE y RX Line Status
	U0IER = 0x07;
	// y por ultimo en el NVIC
//	ISER0 = (0x01 << NVIC_UART0);

	if(st)
		UART0_up();
	else
		UART0_down();
}

void UART0_up()
{
	PCONP |= (0x01 << PCONP_UART0);
	setPINSEL(Tx0, PINSEL_FUNC1);
	setPINSEL(Rx0, PINSEL_FUNC1);
	ISER0 = (0x01 << NVIC_UART0);
	uart0.status = 1;
}

void UART0_down()
{
	ICER0 = (0x01 << NVIC_UART0);
	setPINSEL(Tx0, PINSEL_GPIO);
	setPINSEL(Rx0, PINSEL_GPIO);
	PCONP &= ~(0x01 << PCONP_UART0);
	uart0.status = 0;
}

static uint8_t pushRx(uint8_t data)
{
	if(uart0.bufferRxFull)
		return 1;

	uart0.bufferRx[uart0.indexRxIn] = data;

	uart0.indexRxIn++;
	uart0.indexRxIn %= BUFFER_RX_SIZE;

	uart0.bufferRxEmpty = 0;
	if(uart0.indexRxIn == uart0.indexRxOut)
		uart0.bufferRxFull = 1;

	return 0;
}

static int16_t popTx(void)
{
	int16_t data = -1;

	if(!uart0.bufferTxEmpty)
	{
		data = uart0.bufferTx[uart0.indexTxOut];

		uart0.indexTxOut++;
		uart0.indexTxOut %= BUFFER_TX_SIZE;

		uart0.bufferTxFull = 0;
		if(uart0.indexTxIn == uart0.indexTxOut)
			uart0.bufferTxEmpty = 1;
	}

	return data;
}

inline void UART0_startTx(void)
{
	if(!uart0.TxStart)
	{
		U0THR = popTx();
		uart0.TxStart = 1;
	}
}
