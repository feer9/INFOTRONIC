#include "UART.h"

static uint8_t	pushRx (uint8_t);
//static int16_t	popTx  (void);
static void     sendOn (void);
static uint8_t  pushRxPackage(uint8_t);

uart_t uart0 = {
		.bufferRx.indexIn     = 0,
		.bufferRx.indexOut    = 0,
		.bufferRx.quantity    = 0,
		.bufferRx.isEmpty     = isRxEmpty,
		.bufferRx.isFull      = isRxFull,

		.bufferTx.indexIn     = 0,
		.bufferTx.indexOut    = 0,
		.bufferTx.quantity    = 0,
		.bufferTx.isEmpty     = isTxEmpty,
		.bufferTx.isFull      = isTxFull,

		.TxStart        = FALSE,
		.status         = U0_INIT_STATUS
};


void UART0_IRQHandler()
{
	int16_t data;
	uint8_t intId = (uint8_t) (U0IIR  >> 1) & 0x07UL;

	switch(intId)
	{
	case IIR_RLS:  // Receive Line Status (Rx error)

		// analizo LSR (limpia flag)
		data = U0LSR;
		// ...
		break;

	case IIR_RDA:  // Receive data available

		pushRxPackage(U0_RX_TRIGGER_LEVEL_CHARS);
		break;

	case IIR_CTI:  //  Character Time-out Indicator
		data = U0RBR;
		pushRx((uint8_t) data);
		break;

	case IIR_THRE: // Transmitter Holding Register Empty

		sendOn();
		break;

	default:
		data = U0RBR; data = U0LSR;
		break;
	}

}

static uint8_t pushRx(uint8_t data)
{
	if(uart0.bufferRx.isFull())
		return ERROR;

	uart0.bufferRx.item[uart0.bufferRx.indexIn] = data;

	uart0.bufferRx.indexIn++;
	uart0.bufferRx.indexIn %= BUFFER_UART_SIZE;

	uart0.bufferRx.quantity++;

	return SUCCESS;
}

static uint8_t pushRxPackage(uint8_t pSize)
{
	if(uart0.bufferRx.quantity + pSize > BUFFER_UART_SIZE)
		return ERROR;

	for(uint8_t i=0; i < pSize; i++)
	{
		uart0.bufferRx.item[uart0.bufferRx.indexIn] = U0RBR;

		uart0.bufferRx.indexIn++;
		uart0.bufferRx.indexIn %= BUFFER_UART_SIZE;
	}
	uart0.bufferRx.quantity += pSize;

	return SUCCESS;
}

#if 0
static int16_t popTx(void)
{
	int16_t data = -1;

	if(!uart0.bufferTx.isEmpty())
	{
		data = uart0.bufferTx.item[uart0.bufferTx.indexOut];

		uart0.bufferTx.indexOut++;
		uart0.bufferTx.indexOut %= BUFFER_UART_SIZE;

		uart0.bufferTx.quantity--;
	}

	return data;
}
#endif

extern inline void
sendPackage (char data[17])
{
	for(int i=0; i<16; i++)
		U0THR = data[i];
}

static void sendOn(void)
{
	uint8_t i;
	if(uart0.bufferTx.isEmpty())
	{
		uart0.TxStart = FALSE;
	}
	else
	{
		for(i=0; i<16 && i < uart0.bufferTx.quantity; i++)
		{
			U0THR = uart0.bufferTx.item[uart0.bufferTx.indexOut];

			uart0.bufferTx.indexOut++;
			uart0.bufferTx.indexOut %= BUFFER_UART_SIZE;
		}

		uart0.bufferTx.quantity -= i;
	}
}


void UART0_init(uint8_t st)//,uint32_t baudrate)
{
	PCONP |= (0x01 << PCONP_UART0);

	PCLKSEL0 &= ~(0x03 << PCLKSEL_UART0);
	PCLKSEL0 |= (PCLK_CCLK_4 << PCLKSEL_UART0);

	// 8-bit ; 1 stop bit ; parity enabled ; odd parity (impar) ; break control disabled ; DLAB = 1
	U0LCR = 0b10001011;

	// Configuracion a 115200 baudios
	U0DLM = 0;
	U0DLL = 9;
	// DivAddVal = 1 ; MulVal = 2
	U0FDR = 0x01 | (0x02 << 4);

	// enable FIFO Rx & Tx
	U0FCR = 0x07;
	// Rx Trigger Level 2: 8 characters (interrumpe al 8vo byte en el FIFO de Rx)
	U0FCR |= (TRIGGER_LEVEL << U0FCR_TRIGGER_LEVEL);

	// Tx0: P0,2 ; Rx0: P0,3
	set_dir(Tx0, SALIDA);
	set_dir(Rx0, ENTRADA);
	setPINMODE(Tx0, PINMODE_PULLUP);
	setPINMODE(Rx0, PINMODE_PULLUP);

	// DLAB = 0 para acceder a IER
	U0LCR &= ~(0x01 << 7);
	// habilito interrupciones por RBR, THRE y RX Line Status
	U0IER = 0x07;

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
	uart0.status = ON;
}

void UART0_down()
{
	ICER0 = (0x01 << NVIC_UART0);
	setPINSEL(Tx0, PINSEL_GPIO);
	setPINSEL(Rx0, PINSEL_GPIO);
	PCONP &= ~(0x01 << PCONP_UART0);
	uart0.status = OFF;
}
