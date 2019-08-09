#ifndef _TIMER_H
#define _TIMER_H

#include "chip.h"
#include "lpc_types.h"
#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include <string.h>

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
	pTimer_id_t id;			// puntero a la variable que guarda su id
	uint32_t MR;			// valor de match register en que el timer termina
	uint32_t timeSet;		// tiempo seteado al iniciar el timer
	bool state;				// encendido o apagado
	bool looping;			// opcion de timer repetitivo
	timer_cb_t handler;		// funcion de callback sin argumento
	timer_cb2_t handler2;	// funcion de callback con argumento
	void *data;				// dato que será pasado al callback
} ;

// struct de la máquina de timers
typedef struct
{
	struct timer timer[N_TIMERS];
	uint8_t active;			// numero de timers activos
	timer_id_t MR0isOn;		// numero de timer mas proximo a terminar
} m_timers_t;

/** funciones driver **/
void TIMER0_init(uint32_t us);
extern void timerEnded();

/** funciones de usuario **/
int8_t	isTimerEnd	(timer_id_t);
int8_t  startTimer 	(pTimer_id_t t_id, uint32_t time, timer_cb_t handler);
int8_t  startTimer2 (pTimer_id_t t_id, uint32_t time, timer_cb2_t handler, void *data);
uint8_t	restartTimer(timer_id_t);
uint8_t stopTimer	(pTimer_id_t );
uint8_t timerLoop(pTimer_id_t t_id, bool loop_state);

#endif // _TIMER_H
