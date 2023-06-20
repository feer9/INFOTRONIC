#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include "chip.h"
#include "regsLPC1769.h"
#include "GPIO.h"
#include "KitInfo2_BaseBoard.h"

#define U0_INIT_STATUS OFF

#define UART0_CLK		(CORE_CLK / 4)

#define BUFFER_UART_SIZE 256

typedef enum {
	UART0_REQUEST_NONE, UART0_REQUEST_TIME, UART0_REQUEST_TOP
} UART0_requests_t;


// interruption identifiers
#define		IIR_RLS		0x03
#define		IIR_RDA		0x02
#define		IIR_CTI		0x06
#define		IIR_THRE	0x01


// RX TRIGGER LEVELS
#define		U0FCR_TRIGGER_LEVEL_0		(0x0 << 6)  /* 1  character  or 0x01 */
#define		U0FCR_TRIGGER_LEVEL_1		(0x1 << 6)  /* 4  characters or 0x04 */
#define		U0FCR_TRIGGER_LEVEL_2		(0x2 << 6)  /* 8  characters or 0x08 */
#define		U0FCR_TRIGGER_LEVEL_3		(0x3 << 6)  /* 14 characters or 0x0E */

// n bytes of selected trigger level
#define U0_RX_TRIGGER_LEVEL_BYTES	8


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



void		UART0_init(uint8_t);
void		UART0_setStruct(uart_t *puart0);
void		UART0_setUp(void);
void		UART0_setDown(void);
bool		UART0_isUp(void);
bool		UART0_isRXBuffEmpty(void);
bool		UART0_isRXBuffFull(void);
bool		UART0_isTXBuffEmpty(void);
bool		UART0_isTXBuffFull(void);

size_t		UART0_send(uint8_t *msg, size_t len);
size_t		UART0_sendString(char *msg);
status 		UART0_sendChar(char c);
void		UART0_receive(void);
size_t		UART0_sendRequest(UART0_requests_t id);
void		UART0_requestTime(void);



#endif /* DRIVERS_UART_H_ */
