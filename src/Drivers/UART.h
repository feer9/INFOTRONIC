#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"

#define U0_INIT_STATUS ON

#define UART0_CLK		(CORE_CLK / 4)

#define BUFFER_UART_SIZE 256

enum UART0_requests {UART0_REQUEST_NULL, UART0_REQUEST_TIME, UART0_REQUEST_MAX};


#define TRIGGER_LEVEL		TRIGGER_LEVEL_2
#define U0_RX_TRIGGER_LEVEL_CHARS	8


struct buffer_UART {
	__RW uint8_t  item[BUFFER_UART_SIZE];

	__RW uint8_t  indexIn;
	__RW uint8_t  indexOut;

	__RW uint16_t quantity;
	bool (*isEmpty) (void);
	bool (*isFull)  (void);
};

typedef struct {
	struct buffer_UART bufferRx;
	struct buffer_UART bufferTx;

	__RW bool	 TxStart;
	__RW bool    status;
}uart_t;


extern uart_t uart0;

static inline bool isRxEmpty (void) {
	return (uart0.bufferRx.quantity == 0) ? TRUE : FALSE;
}

static inline bool isTxEmpty (void) {
	return (uart0.bufferTx.quantity == 0) ? TRUE : FALSE;
}

static inline bool isRxFull (void) {
	return (uart0.bufferRx.quantity == BUFFER_UART_SIZE) ? TRUE : FALSE;
}

static inline bool isTxFull (void) {
	return (uart0.bufferTx.quantity == BUFFER_UART_SIZE) ? TRUE : FALSE;
}


void		UART0_init(uint8_t);
void		UART0_setUp(void);
void		UART0_setDown(void);
uint8_t		UART0_sendString(char *msg);
uint8_t 	UART0_sendChar(char c);
void		UART0_receive(void);
uint8_t		UART0_sendRequest(uint8_t id);
void		UART0_requestTime(void);



#endif /* DRIVERS_UART_H_ */
