#ifndef FW_GPIO_H_
#define FW_GPIO_H_

#include "regsLPC1769.h"


//!< ------ Actividad en lectura de pines
#define GPIO_ACTIVE_LOW  0
#define GPIO_ACTIVE_HIGH 1

//!< -------- Estados de PINSEL:
typedef enum {
PINSEL_GPIO,
PINSEL_FUNC0 = PINSEL_GPIO,
PINSEL_FUNC1,
PINSEL_FUNC2,
PINSEL_FUNC3,
} PINSEL_STATE_T;

//!< ----------- Estados de PINMODE
typedef enum {
PINMODE_PULLUP,  //!< 00	Pull Up resistor enable (reset value)
PINMODE_REPEAT,  //!< 01	repeated mode enable
PINMODE_NONE,    //!< 10	ni Pull Up ni Pull DOwn
PINMODE_PULLDOWN //!< 11	Pull Down resistor enable
} PINMODE_STATE_T;

//!< ------ Dirección en FIODIR:
typedef enum {
GPIO_INPUT,
GPIO_OUTPUT
}GPIO_IO_STATE_T;


void gpio_init (void);


static inline void gpio_writePin (uint8_t port, uint8_t pin, uint8_t state)
{
	state ? (FIO[port].SET |= (1UL << pin)) : (FIO[port].CLR |= (1UL << pin)) ;
}

static inline void gpio_setPin (uint8_t port, uint8_t pin)
{
	FIO[port].SET |= (1UL << pin);
}

static inline void gpio_clearPin (uint8_t port, uint8_t pin)
{
	FIO[port].CLR |= (1UL << pin);
}

static inline void gpio_togglePin(uint8_t port, uint8_t pin)
{
	FIO[port].PIN ^= (1UL << pin) ;
}

static inline bool gpio_readPinState (uint8_t port, uint8_t pin, uint8_t state)
{
	return (bool) ((FIO[port].PIN >> pin) & 1UL) == state;
}

static inline bool gpio_readPin (uint8_t port, uint8_t pin)
{
	return (bool) ((FIO[port].PIN >> pin) & 1UL);
}

static inline void gpio_pinDir (uint8_t port, uint8_t pin, GPIO_IO_STATE_T dir)
{
	dir	?	(FIO[port].DIR |=  (1UL << pin)) : (FIO[port].DIR &= ~(1UL << pin)) ;
}

static inline void gpio_pinDir_input (uint8_t port, uint8_t pin)
{
	FIO[port].DIR &= ~(1UL << pin);
}

static inline void gpio_pinDir_output (uint8_t port, uint8_t pin)
{
	FIO[port].DIR |=  (1UL << pin);
}

void gpio_pinsel 	(uint8_t port, uint8_t pin, PINSEL_STATE_T func);

void gpio_pinmode	(uint8_t port, uint8_t pin, PINMODE_STATE_T mode);

static inline void gpio_pinmode_OD (uint8_t port, uint8_t pin, uint8_t mode)
{
	mode ?	(PINMODE_OD[port] |=  (1UL << pin)) : (PINMODE_OD[port] &= ~(1UL << pin));
}

static inline void gpio_pinmode_OD_enable (uint8_t port, uint8_t pin)
{
	PINMODE_OD[port] |=  (1UL << pin);
}

static inline void gpio_pinmode_OD_disable (uint8_t port, uint8_t pin)
{
	PINMODE_OD[port] &= ~(1UL << pin);
}

// mode = PINMODE_X ; func = PINSEL_FUNCx
void gpio_configPin	(uint8_t port, uint8_t pin, PINMODE_STATE_T mode, PINSEL_STATE_T func );




#if 0

/* only valid if state equals 1 or 0 */
/* #define __write_pin__(port,pin,state)	(*(FIOCLR - ((bool)(state)) + 8*(port)) |= (1<<(pin))) */

#define __write_pin__(port,pin,state)	((state) ?	(FIO[port].SET = (1UL << (pin))) : \
									 				(FIO[port].CLR = (1UL << (pin))))

#define __set_pin__(port,pin)	( FIO[port].SET = (1UL << (pin)) )

#define __clear_pin__(port,pin)	( FIO[port].CLR = (1UL << (pin)) )

#define __toggle_pin__(port,pin)	( FIO[port].PIN ^= (1UL << (pin)) )

#define __read__(port,pin)		( ( FIO[port].PIN >> (pin) ) & 1UL )

#define __read_state__(port,pin,state)	((((FIO[port].PIN >> (pin)) & 1UL) == (state)) ? 1:0 )

#define __set_dir__(port,pin,dir)	( (dir)	?	( FIO[port].DIR |=  (1UL << (pin)) ) : \
												( FIO[port].DIR &= ~(1UL << (pin)) ) )

#define __dir_input__(port,pin) 	( FIO[port].DIR &= ~(1UL << (pin)) )

#define __dir_output__(port,pin)	( FIO[port].DIR |=  (1UL << (pin)) )

#define __setMODE_OD__(port,pin,mode)	((mode) ?	( PINMODE_OD[port] |=  (1UL << (pin)) ) : \
													( PINMODE_OD[port] &= ~(1UL << (pin)) )     )

#define __MODE_OD_enable__(port,pin)	( PINMODE_OD[port] |=  (1UL << (pin)) )

#define __MODE_OD_disable__(port,pin)	( PINMODE_OD[port] &= ~(1UL << (pin)) )



#define gpio_writePin(...)           __write_pin__(__VA_ARGS__)

#define gpio_setPin(...)             __set_pin__(__VA_ARGS__)

#define gpio_clearPin(...)           __clear_pin__(__VA_ARGS__)

#define gpio_togglePin(...)          __toggle_pin__(__VA_ARGS__)

#define gpio_readPin(...)            __read__(__VA_ARGS__)

#define gpio_readPinState(...)       __read_state__(__VA_ARGS__)

#define gpio_pinDir(...)             __set_dir__(__VA_ARGS__)

#define gpio_pinDir_input(...)       __dir_input__(__VA_ARGS__)

#define gpio_pinDir_output(...)      __dir_output__(__VA_ARGS__)

#define gpio_pinmode_OD(...)         __setMODE_OD__(__VA_ARGS__)

#define gpio_pinmode_OD_enable(...)  __MODE_OD_enable__(__VA_ARGS__)

#define gpio_pinmode_OD_disable(...) __MODE_OD_disable__(__VA_ARGS__)

#endif


#endif /* FW_GPIO_H_ */
