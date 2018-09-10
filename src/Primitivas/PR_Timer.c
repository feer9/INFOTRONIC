#include <Timer.h>

/**  FUNCIONES DE USUARIO */

// devuelve 1 si el timer "n" NO está activo
uint8_t isTimerEnd(uint8_t n)
{
	return timers(IS_TIMER_END, n, 0,NULL);
}

// time: tiempo en ms , handler: funcion de callback
uint8_t startTimer(uint32_t time, callback_t handler)
{
	return timers(ON, _ANY_TIMER, time, handler);
}

// time: tiempo en ms , n: numero de timer , handler: funcion de callback
uint8_t startnTimer(uint8_t n, uint32_t time, callback_t handler)
{
	return timers(ON, n, time, handler);
}

// detiene el timer "n"
void stopTimer(uint8_t n)
{
	timers(OFF, n, 0,NULL);
}

/**  END FUNCIONES DE USUARIO  */


// accion, numero de timer, tiempo y funcion de callback
uint8_t timers(uint8_t act, int8_t n, uint32_t time, callback_t handler)
{
	static bool initialized = FALSE;
	static m_timers_t t;
	static uint8_t reserved = 0;
	int8_t err=0;
	callback_t callback;

	if (!initialized)
	{
		timer_init(&t);
		initialized = TRUE;
	}

	// si no me piden ningun timer en especifico
	if(n == _ANY_TIMER)
	{	// busco un timer que no este en uso
		for(n=0 ; n<N_TIMERS ; n++)
			if( !t.timer[n].state && !((reserved>>n)&0x01) )
				break;
		if(n == N_TIMERS)
			return 1;
	}
	// reservo los que vaya usando a peticion
	else if(act == ON)
		reserved |= (0x01 << n);


	switch (act)
	{
	case OFF:
		timerOff(n,&t);
		break;

	case ON:
		err = timerOn(time, n, &t, handler);
		break;

	case TIMER_FINISHED:
		do
		{
			n = t.MR0isOn;
			callback = t.timer[n].handler;
			timerOff(n, &t);
			if(callback != NULL)
				callback();
		}
		while ( (T0->TCR & _BIT(0)) && (T0->MR0 < T0->TC) );
		break;

	case IS_TIMER_END:
		err = !t.timer[n].state;
		break;

	default:
		err++;
		break;
	}

	return err;
}

void timer_init(m_timers_t* t)
{
	int i;
	for(i=0; i<N_TIMERS; i++)
	{
		t->timer[i].count = 0;
		t->timer[i].state = 0;
		t->timer[i].handler = NULL;
	}
	t->active = 0;
	t->MR0isOn = 0;
}

uint8_t timerOn (uint32_t time, int8_t n, m_timers_t* t, callback_t handler)
{
	if(t->active >= N_TIMERS)
		return 1;

	t->timer[n].count = T0->TC + time;
	t->timer[n].handler = handler;

	if(!(t->active))			// si no habia ninguno prendido
	{
		T0->MR0 = t->timer[n].count;	// Seteo temporizacion
		T0->MCR _SET_BIT(0);			// Interrumpe en match0
		t->MR0isOn = n;
		T0->TCR = 1;					// Enciendo el temporizador
		ISER0 _SET_BIT(NVIC_TIMER0);	// Habilito Interrupcion TIMER0
	}
	else
	{
		// si el timer actual termina antes que el seteado en Match Register
		// o si volvi a encender el mismo timer, actualizo  MR0
		if(T0->MR0 > t->timer[n].count || t->MR0isOn == n)
		{
			T0->MR0 = t->timer[n].count;
			t->MR0isOn = n;
		}
	}
	if(t->timer[n].state == OFF)
	{
		t->timer[n].state = ON;
		t->active ++;
	}

	return 0;
}

void timerOff(uint8_t n, m_timers_t *t)
{
	if(t->timer[n].state) // si esta encendido
	{
		t->timer[n].count = 0;
		t->timer[n].state = 0;
		t->timer[n].handler = NULL;
		t->active --;
		if(!t->active) // apago t0do
		{
			ICER0 = (0x01 << 1);	// Deshabilito Interrupcion TIMER0
			T0->MCR _RESET_BIT(0);	// Desactivo interrupcion match 0
			T0->TCR = 2;			// Apago y reseteo el temporizador
			T0->MR0 = 0;			// Restablezco el Match Register
		}
		else
		{
			t->MR0isOn = nextTimer(t);
			T0->MR0 = t->timer[t->MR0isOn].count;
		}
	}
}

int8_t nextTimer(m_timers_t* t)
{
	uint32_t nextNum;
	uint8_t nextPos;
	uint8_t i=0;

	if(!t->active)
		return -1;

	while(!t->timer[i].state && i < N_TIMERS) i++; // busco el primer timer en uso

	nextNum = t->timer[i].count;
	nextPos = i;

	for(; i<N_TIMERS; i++)
	{
		if( t->timer[i].state && t->timer[i].count < nextNum)
		{
			nextNum = t->timer[i].count;
			nextPos =  i;
		}
	}
	return nextPos;
}
