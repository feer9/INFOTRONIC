#ifndef _TIMER_H
#define _TIMER_H

#include <lpc_types.h>
#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include "clock.h"

//#define _ANY_TIMER -1


// Acciones en match register
#define		MR0I	0  /* Interrupt on MRn */
#define		MR0R	1  /* Reset     on MRn */
#define		MR0S	2  /* Stop      on MRn */
#define		MR1I	3
#define		MR1R	4
#define		MR1S	5
#define		MR2I	6
#define		MR2R	7
#define		MR2S	8
#define		MR3I	9
#define		MR3R	10
#define		MR3S	11

#define N_TIMERS 10
//#define TIMER_FINISHED 2
//#define IS_TIMER_END 3

// struct de cada instancia de timer
struct timer
{
	uint32_t MR;			// numero en el match register
	uint32_t timeSet;		// tiempo seteado
	uint8_t state;			// encendido o apagado
	void (*handler)(void);	// funcion que se ejecutará al finalizar
} ;

// struct de la máquina de timers
typedef struct
{
	struct timer timer[N_TIMERS];
	uint8_t active;			// numero de timers activos
	uint8_t MR0isOn;		// numero de timer mas proximo a terminar
} m_timers_t;

/** funcioness drivers **/
void TIMER0_init(uint32_t us);
extern void timerEnded();

/** funciones de usuario */
uint8_t	isTimerEnd	(int8_t);
int8_t  startTimer 	(uint32_t time, callback_t handler);
uint8_t	restartTimer(int8_t);
uint8_t stopTimer	(__RW int8_t *);


#endif // _TIMER_H
