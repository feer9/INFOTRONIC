#ifndef FW_GPIO_H_
#define FW_GPIO_H_

#include <RegsLPC1769.h>

void 		SetPINSEL 	( uint8_t  , uint8_t  ,uint8_t );
void 		SetPINMODE	( uint8_t  , uint8_t  ,uint8_t );
void 		SetDIR		( uint8_t  , uint8_t  , uint8_t  );
void 		SetPIN		( uint8_t  , uint8_t  , uint8_t  );
uint8_t 	GetPIN		( uint8_t  , uint8_t  , uint8_t );
void 		TooglePIN	( uint8_t  , uint8_t);

void		InicLED		( void );


/*
#define write_pin(port,pin,state)	{if(state)	*(FIOSET + 8*port) |= (1 << pin); \
									 else		*(FIOCLR + 8*port) |= (1 << pin);}
*/

// definicion solo valida para state = 0 o 1
#define write_pin(port,pin,state)	*(FIOCLR - state + 8*port) |= (1<<pin)

#define read_pin(port,pin)		(( *(FIOPIN + 8*port) >> pin) & 0x01)

#define read(port,pin,activ)	((((*(FIOPIN + 8*port) >> pin) & 0x01) == activ) ? 1:0)

#define set_dir(port,pin,dir)	{if(dir)	*(FIODIR + 8*port) |=  (1 << pin); \
								 else		*(FIODIR + 8*port) &= ~(1 << pin); }

#define toggle_pin(port,pin)	{if(read_pin(port,pin))	write_pin(port,pin, 0); \
								 else					write_pin(port,pin, 1); }


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
