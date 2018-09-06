#ifndef FW_GPIO_H_
#define FW_GPIO_H_

#include <RegsLPC1769.h>

void 		setPINSEL 	( uint8_t, uint8_t, uint8_t );
void 		setPINMODE	( uint8_t, uint8_t, uint8_t );
void 		setDIR		( uint8_t, uint8_t, uint8_t );
void 		setPIN		( uint8_t, uint8_t, uint8_t );
uint8_t 	getPIN		( uint8_t, uint8_t, uint8_t );
void 		tooglePIN	( uint8_t, uint8_t );

//void		InicLED		( void );


// definicion solo valida para state = 0 o 1
// #define __write_pin__(port,pin,state)	(*(FIOCLR - state + 8*port) |= (1<<pin))
#define __write_pin__(port,pin,state)	((state) ?	(FIOSET[8*port] = (0x01 << pin)) : \
									 				(FIOCLR[8*port] = (0x01 << pin)))
#define write_pin(...)			__write_pin__(__VA_ARGS__)

#define __set_pin__(port,pin)	( FIOSET[8*port] = (0x01 << pin) )
#define set_pin(...)			__set_pin__(__VA_ARGS__)

#define __clear_pin__(port,pin)	( FIOCLR[8*port] = (0x01 << pin) )
#define clear_pin(...)			__clear_pin(__VA_ARGS__)

#define __read__(port,pin)		( ( FIOPIN[8*port] >> pin ) & 0x01 )
#define read(...)				__read__(__VA_ARGS__)

#define __read_pin__(port,pin,activ)	((((FIOPIN[8*port] >> pin) & 0x01) == activ) ? 1:0 )
#define read_pin(...)			__read_pin__(__VA_ARGS__)

#define __set_dir__(port,pin,dir)	( (dir)	?	( FIODIR[8*port] |=  (0x01 << pin) ) : \
												( FIODIR[8*port] &= ~(0x01 << pin) ) )
#define set_dir(...)			__set_dir__(__VA_ARGS__)

#define __toggle_pin__(port,pin)	( FIOPIN[8*port] ^= (0x01 << pin) )
#define toggle_pin(...)			__toggle_pin__(__VA_ARGS__)

#define __setMODE_OD__(port,pin,mode)	((mode) ?	PINMODE_OD[port] |=  (0x01 << pin) : \
													PINMODE_OD[port] &= ~(0x01 << pin))
#define set_MODE_OD(...)		__setMODE_OD__(__VA_ARGS__)



#define		ACTIVO_BAJO		0
#define		ACTIVO_ALTO		1

// dirección en FIODIR
#define 	ENTRADA			0
#define 	SALIDA			1

#endif /* FW_GPIO_H_ */
