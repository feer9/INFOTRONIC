#ifndef FW_GPIO_H_
#define FW_GPIO_H_

#include <RegsLPC1769.h>

void 		SetPINSEL 	( uint8_t, uint8_t, uint8_t );
void 		SetPINMODE	( uint8_t, uint8_t, uint8_t );
void 		SetDIR		( uint8_t, uint8_t, uint8_t );
void 		SetPIN		( uint8_t, uint8_t, uint8_t );
uint8_t 	GetPIN		( uint8_t, uint8_t, uint8_t );
void 		TooglePIN	( uint8_t, uint8_t );

void		InicLED		( void );





#define __write_pin__(port,pin,state)	((state) ?	(*(FIOSET + 8*port) |= (1 << pin)) : \
									 				(*(FIOCLR + 8*port) |= (1 << pin)))
// definicion solo valida para state = 0 o 1
// #define __write_pin__(port,pin,state)	(*(FIOCLR - state + 8*port) |= (1<<pin))
#define write_pin(...)		__write_pin__(__VA_ARGS__)

#define __read__(port,pin)		(( *(FIOPIN + 8*port) >> pin) & 0x01)
#define read(...)			__read__(__VA_ARGS__)

#define __read_pin__(port,pin,activ)	((((*(FIOPIN + 8*port) >> pin) & 0x01) == activ) ? 1:0 )
#define read_pin(...)		__read_pin__(__VA_ARGS__)

#define __set_dir__(port,pin,dir)	( (dir)	?	(*(FIODIR + 8*port) |=  (1 << pin)) : \
												(*(FIODIR + 8*port) &= ~(1 << pin)) )
#define set_dir(...)		__set_dir__(__VA_ARGS__)

#define __toggle_pin__(port,pin)	( read(port,pin)	?	write_pin(port,pin, 0) : \
															write_pin(port,pin, 1) )
#define toggle_pin(...)		__toggle_pin__(__VA_ARGS__)

#define __setMODE_OD__(port,pin,mode)	((mode) ?	PINMODE_OD [port] |=  (1 << pin) : \
													PINMODE_OD [port] &= ~(1 << pin))
#define setMODE_OD(...)		__setMODE_OD__(__VA_ARGS__)



#define		ACTIVO_BAJO		0
#define		ACTIVO_ALTO		1

// dirección en FIODIR
#define 	ENTRADA			0
#define 	SALIDA			1

// identificadores de los puertos
#define 	PORT0			0
#define 	PORT1			1
#define 	PORT2			2
#define 	PORT3			3
#define 	PORT4			4

#endif /* FW_GPIO_H_ */
