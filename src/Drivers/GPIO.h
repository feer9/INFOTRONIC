#ifndef FW_GPIO_H_
#define FW_GPIO_H_

#include "regsLPC1769.h"

void		configurePin(uint8_t port, uint8_t pin, uint32_t mode, uint32_t func );
void		setPINSEL 	(uint8_t port, uint8_t pin, uint8_t func);
void		setPINMODE	(uint8_t port, uint8_t pin, uint8_t mode);

void		PIN_init	(void);

static inline void setMODE_OD (uint8_t port, uint8_t pin, uint8_t mode)
{
	mode ?	(PINMODE_OD[port] |=  (1UL << pin)) : (PINMODE_OD[port] &= ~(1UL << pin));
}

static inline void setDIR (uint8_t port, uint8_t pin, uint8_t dir)
{
	dir	?	(FIODIR[8*port] |=  (1UL << pin)) : (FIODIR[8*port] &= ~(1UL << pin)) ;
}

static inline void setPIN (uint8_t port, uint8_t pin, uint8_t state)
{
	state ? (FIOSET[8*port] |= (1UL << pin)) : (FIOCLR[8*port] |= (1UL << pin)) ;
}

static inline bool getPIN (uint8_t port, uint8_t pin, uint8_t activ)
{
	return (((FIOPIN[8*port] >> pin) & 1UL) == activ) ? true : false ;
}

static inline void tooglePIN(uint8_t port, uint8_t pin)
{
	FIOPIN[8*port] ^= (1UL << pin) ;
}



/* only valid if state equals 1 or 0 */
// #define __write_pin__(port,pin,state)	(*(FIOCLR - state + 8*port) |= (1<<pin))

#define __write_pin__(port,pin,state)	((state) ?	(FIO[port].SET = (1UL << pin)) : \
									 				(FIO[port].CLR = (1UL << pin)))
#define write_pin(...) \
		__write_pin__(__VA_ARGS__)

#define __set_pin__(port,pin)	( FIO[port].SET = (1UL << (pin)) )
#define set_pin(...) \
		__set_pin__(__VA_ARGS__)

#define __clear_pin__(port,pin)	( FIO[port].CLR = (1UL << (pin)) )
#define clear_pin(...) \
		__clear_pin__(__VA_ARGS__)

#define __toggle_pin__(port,pin)	( FIO[port].PIN ^= (1UL << (pin)) )
#define toggle_pin(...) \
		__toggle_pin__(__VA_ARGS__)


#define __read__(port,pin)		( ( FIO[port].PIN >> (pin) ) & 1UL )
#define read(...) \
		__read__(__VA_ARGS__)

#define __read_pin__(port,pin,activ)	((((FIO[port].PIN >> (pin)) & 1UL) == (activ)) ? 1:0 )
#define read_pin(...) \
		__read_pin__(__VA_ARGS__)



#define __set_dir__(port,pin,dir)	( (dir)	?	( FIO[port].DIR |=  (1UL << (pin)) ) : \
												( FIO[port].DIR &= ~(1UL << (pin)) ) )
#define set_dir(...) \
		__set_dir__(__VA_ARGS__)

#define __set_input__(port,pin) 	( FIO[port].DIR &= ~(1UL << (pin)) )
#define set_dir_input(...) \
		__set_input__(__VA_ARGS__)

#define __set_output__(port,pin)	( FIO[port].DIR |=  (1UL << (pin)) )
#define set_dir_output(...) \
		__set_output__(__VA_ARGS__)



#define __setMODE_OD__(port,pin,mode)	((mode) ?	( PINMODE_OD[port] |=  (1UL << (pin)) ) : \
													( PINMODE_OD[port] &= ~(1UL << (pin)) )     )
#define set_MODE_OD(...) \
		__setMODE_OD__(__VA_ARGS__)

#define __MODE_OD_enable__(port,pin)	( PINMODE_OD[port] |=  (1UL << (pin)) )
#define MODE_OD_enable(...) \
		__MODE_OD_enable__(__VA_ARGS__)

#define __MODE_OD_disable__(port,pin)	( PINMODE_OD[port] &= ~(1UL << (pin)) )
#define MODE_OD_disable(...) \
		__MODE_OD_disable__(__VA_ARGS__)



//!< ------ Actividad en lectura de pines
#define		ACTIVO_BAJO			0
#define		ACTIVO_ALTO			1

//!< -------- Estados de PINSEL:
typedef enum {
PINSEL_GPIO,
PINSEL_FUNC0 = PINSEL_GPIO,
PINSEL_FUNC1,
PINSEL_FUNC2,
PINSEL_FUNC3,
} PINSEL_STATE;

//!< ----------- Estados de PINMODE
typedef enum {
PINMODE_PULLUP,  //!< 00	Pull Up resistor enable (reset value)
PINMODE_REPEAT,  //!< 01	repeated mode enable
PINMODE_NONE,    //!< 10	ni Pull Up ni Pull DOwn
PINMODE_PULLDOWN //!< 11	Pull Down resistor enable
} PINMODE_STATE;

//!< ------ Dirección en FIODIR:
typedef enum {
GPIO_INPUT,
GPIO_OUTPUT
}GPIO_IO_STATE;

#define 	ENTRADA				0
#define 	SALIDA				1


#endif /* FW_GPIO_H_ */
