#include "UART.h"

static status	pushRx (uint8_t data);
static status  pushRxPackage(uint8_t size);
//static int16_t	popTx  (void);
static void     sendOn (void);



static bool isRxEmpty(void);
static bool isTxEmpty(void);
static bool isRxFull(void);
static bool isTxFull(void);


static uart_t uart0 = {
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

		.TxStart        = false,
		.status         = U0_INIT_STATUS
};


inline bool isRxEmpty (void) {
	return (bool) (uart0.bufferRx.quantity == 0);
}

inline bool isTxEmpty (void) {
	return (bool) (uart0.bufferTx.quantity == 0);
}

inline bool isRxFull (void) {
	return (bool) (uart0.bufferRx.quantity == BUFFER_UART_SIZE);
}

inline bool isTxFull (void) {
	return (bool) (uart0.bufferTx.quantity == BUFFER_UART_SIZE);
}

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

		pushRxPackage(U0_RX_TRIGGER_LEVEL_BYTES);
		break;

	case IIR_CTI:  //  Character Time-out Indicator
		data = U0RBR;
		pushRx((uint8_t) data);
		break;

	case IIR_THRE: // Transmitter Holding Register Empty

		if(uart0.bufferTx.isEmpty())
			uart0.TxStart = false;
		else
			sendOn();
		break;

	default:
		data = U0RBR; data = U0LSR;
		break;
	}

}

static status pushRx(uint8_t data)
{
	if(uart0.bufferRx.isFull())
		return ERROR;

	uart0.bufferRx.item[uart0.bufferRx.indexIn] = data;

	uart0.bufferRx.indexIn++;
	uart0.bufferRx.indexIn %= BUFFER_UART_SIZE;

	uart0.bufferRx.quantity++;

	return SUCCESS;
}

static status pushRxPackage(uint8_t size)
{
	if(uart0.bufferRx.quantity + size > BUFFER_UART_SIZE)
		return ERROR;

	for(uint8_t i=0; i < size; i++)
	{
		uart0.bufferRx.item[uart0.bufferRx.indexIn] = U0RBR;

		uart0.bufferRx.indexIn++;
		uart0.bufferRx.indexIn %= BUFFER_UART_SIZE;
	}
	uart0.bufferRx.quantity += size;

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

	for(i=0; i<16 && i < uart0.bufferTx.quantity; i++)
	{
		U0THR = uart0.bufferTx.item[uart0.bufferTx.indexOut];

		uart0.bufferTx.indexOut++;
		uart0.bufferTx.indexOut %= BUFFER_UART_SIZE;
	}

	uart0.bufferTx.quantity -= i;
}


void UART0_init(uint8_t st)//,uint32_t baudrate)
{
	PCONP |=  PCONP_UART0;

	setPCLKDiv(PCLKSEL_UART0, PCLKDIV_1);

	// 8-bit ; 1 stop bit ; parity enabled ; odd parity (impar) ; break control disabled ; DLAB = 1
	U0LCR = 0b10001011;

	// Configuracion a 115200 baudios con PCLK_UART0 = 25MHz
//	U0DLM = 0;
//	U0DLL = 9;
	// DivAddVal = 1 ; MulVal = 2
//	U0FDR = 0x01 | (0x02 << 4);

	// Configuracion a 115200 baudios con PCLK_UART0 = 96MHz
	U0DLM = 0;
	U0DLL = 34;
	// DivAddVal = 8 ; MulVal = 15
	U0FDR = 0x08 | (0x0F << 4);

	// enable FIFO Rx & Tx
	U0FCR = 0x07;
	// Rx Trigger Level 2: 8 characters (interrumpe al 8vo byte en el FIFO de Rx)
	U0FCR |= U0FCR_TRIGGER_LEVEL_2;

	// Tx0: P0,2 ; Rx0: P0,3
	set_dir(Tx0, SALIDA);
	set_dir(Rx0, ENTRADA);
	setPINMODE(Tx0, PINMODE_PULLUP);
	setPINMODE(Rx0, PINMODE_PULLUP);

	// DLAB = 0 para acceder a IER
	U0LCR &= ~(0x01 << 7);
	// habilito interrupciones por RBR, THRE y RX Line Status
	U0IER = 0x07;

	UART0_setStruct(&uart0);

	if(st)
		UART0_setUp();
	else
		UART0_setDown();
}

void UART0_setUp()
{
	PCONP |= PCONP_UART0;
	setPINSEL(Tx0, PINSEL_FUNC1);
	setPINSEL(Rx0, PINSEL_FUNC1);
	ISER0 = NVIC_UART0;
	uart0.status = ON;
}

void UART0_setDown()
{
	ICER0 = NVIC_UART0;
	setPINSEL(Tx0, PINSEL_GPIO);
	setPINSEL(Rx0, PINSEL_GPIO);
	PCONP &= ~PCONP_UART0;
	uart0.status = OFF;
}
