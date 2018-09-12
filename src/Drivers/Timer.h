#ifndef _TIMER_H
#define _TIMER_H

#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include <types.h>

#define _ANY_TIMER -1

#define N_TIMERS 10
#define TIMER_FINISHED 2
#define IS_TIMER_END 3

typedef struct
{
	uint32_t count;			// tiempo seteado para el timer "n"
	uint8_t state;			// estado del timer "n"
	void (*handler)(void);	// funcion que se ejecutará al finalizar
} timer_t;

typedef struct
{
	timer_t timer[N_TIMERS];
	uint8_t active;			// numero de timers activos
	uint8_t MR0isOn;		// numero de timer mas proximo a terminar
} m_timers_t;


void TIMER0_init(uint32_t us);
int8_t nextTimer(m_timers_t* t);
uint8_t timerOn (uint32_t time, int8_t n, m_timers_t* t, callback_t handler);
void timerOff(uint8_t n, m_timers_t *t);
void timer_init(m_timers_t* t);
uint8_t timers(uint8_t act, int8_t n, uint32_t time, callback_t handler);


/** funciones de usuario */
void timerEnd_Handler(uint8_t);
uint8_t isTimerEnd(uint8_t);
void stopTimer(uint8_t);
uint8_t startTimer(uint32_t time, callback_t handler);
uint8_t startnTimer(uint8_t n, uint32_t time, callback_t handler);

#endif // _TIMER_H
