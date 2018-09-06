#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include <Aplicacion.h>


#define UART0_CLK		(CORE_CLK / 4)

#define BUFFER_RX_SIZE	256
#define BUFFER_TX_SIZE	256

void	UART0_init();
uint8_t	pushRx(uint8_t data);
int16_t	popRx(void);
uint8_t	pushTx(uint8_t data);
int16_t	popTx(void);
uint8_t	UART0_sendString(char *msg);
void	UART0_receive(void);

#endif /* DRIVERS_UART_H_ */
