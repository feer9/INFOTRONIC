#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"

#define U0_ENABLE

#define UART0_CLK		(CORE_CLK / 4)

#define BUFFER_RX_SIZE	256
#define BUFFER_TX_SIZE	256

typedef struct {
	__RW uint8_t bufferRx[BUFFER_RX_SIZE];
	__RW uint8_t bufferTx[BUFFER_TX_SIZE];

	__RW uint8_t indexRxIn;
	__RW uint8_t indexRxOut;
	__RW uint8_t indexTxIn;
	__RW uint8_t indexTxOut;

	__RW uint8_t bufferRxFull;
	__RW uint8_t bufferRxEmpty;
	__RW uint8_t bufferTxFull;
	__RW uint8_t bufferTxEmpty;

	__RW uint8_t TxStart;
} uart_t;

void		UART0_init(void);
uint8_t		UART0_sendString(char *msg);
uint8_t 	UART0_sendChar(char c);
void		UART0_receive(void);
extern void	UART0_startTx(void);

#endif /* DRIVERS_UART_H_ */
