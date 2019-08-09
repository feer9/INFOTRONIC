#include "../Drivers/Timer.h"

//static m_timers_t	timer_init (void);
static timer_id_t	seekAvailableTimer (void);
static timer_id_t	nextTimer (void);

static m_timers_t t = { 0 };
/*{
		.timer[0].state = 0, .timer[1].state = 0, .timer[2].state = 0,
		.timer[3].state = 0, .timer[4].state = 0, .timer[5].state = 0,
		.timer[6].state = 0, .timer[7].state = 0, .timer[8].state = 0,
		.timer[9].state = 0,
		.active = 0,
		.MR0isOn = 0
};*/



/**  FUNCIONES DE USUARIO */

// devuelve 0 si el timer está activo
//          1 si terminó (NO está activo)
//          -1 id invalida
inline int8_t isTimerEnd(timer_id_t id)
{
	return (id >= 0 && id < N_TIMERS) ?	!t.timer[id].state : -1;
}


// t_id: variable donde se guardará la id del timer iniciado, debe ser inicializada en -1
// time: tiempo en ms , handler: funcion de callback, puede ser NULL
// devuelve SUCCESS si lo pudo iniciar o ERROR caso contrario
int8_t startTimer (pTimer_id_t t_id, uint32_t time, timer_cb_t handler)
{
	timer_id_t aux = seekAvailableTimer();

	if(aux != -1) // si hay timers disponibles
	{
		if(t_id)
		{
			if(*t_id >= 0 && *t_id < N_TIMERS) {
				// checkeo que la referencia del timer corresponda a ese timer
				// ya que caso contrario ésta fue modificada
				if(t.timer[*t_id].id != t_id)
					return ERROR;
				if(t.timer[*t_id].state == true)
					stopTimer(t_id);
			}
			*t_id = aux;
		}

		uint8_t id = (uint8_t) aux;
		t.timer[id].MR = T0->TC + time;
		t.timer[id].timeSet = time;
		t.timer[id].id = t_id;
		t.timer[id].looping = false;
		t.timer[id].handler = handler;
		t.timer[id].handler2 = NULL;
		t.timer[id].data = NULL;

		if(!(t.active))			// si no habia ninguno prendido
		{
			T0->MR0 = t.timer[id].MR;	// Seteo temporizacion
			T0->MCR _SET_BIT(MR0I);		// Interrumpe en match0
			t.MR0isOn = aux;
			T0->TCR = 1;				// Enciendo el temporizador
			ISER0 = NVIC_TIMER0;		// Habilito Interrupcion TIMER0
		}
		else
		{
			// si el timer actual termina antes que el seteado en Match Register
			// actualizo  MR0
			if(t.timer[id].MR < T0->MR0)
			{
				T0->MR0 = t.timer[id].MR;
				t.MR0isOn = aux;
			}
		}
		if(t.timer[id].state == false)
		{
			t.timer[id].state = true;
			t.active ++;
		}
	}
#ifdef DEBUG
	else while(1); // no deberia pasar, todos los timers en uso
#endif

	return (aux == -1) ? ERROR : SUCCESS;
}

int8_t startTimer2(pTimer_id_t t_id, uint32_t time, timer_cb2_t handler, void *data)
{
	if(startTimer(t_id, time, NULL) == SUCCESS) {
		t.timer[*t_id].handler2 = handler;
		t.timer[*t_id].data = data;
		return SUCCESS;
	}
	return ERROR;
}

// funcion que vuelve a iniciar la cuenta atrás del timer [id]
// id: numero de timer
// retorna SUCCESS si lo reinició, ERROR si no estaba encendido
uint8_t restartTimer(timer_id_t id)
{
	if(id < 0 || id >= N_TIMERS || !t.timer[id].state)
		return ERROR;

	t.timer[id].MR = T0->TC + t.timer[id].timeSet;

	t.MR0isOn = nextTimer();
	T0->MR0 = t.timer[t.MR0isOn].MR;

	return SUCCESS;
}

// devuelve SUCCESS si detuvo el timer, ERROR si no estaba encendido
uint8_t stopTimer(pTimer_id_t id)
{
	int8_t n = *id;

	if(t.timer[n].id != NULL)
	{
		if(n == *t.timer[n].id) // checkeo el valor de id
			*id = -1;
		else
			return ERROR;
	}

	if(n >= 0 && n < N_TIMERS && t.timer[n].state == true)
	{
		memset(&t.timer[n], 0, sizeof (struct timer));
		t.active --;
		if(!t.active) // apago t0do
		{
			ICER0 = NVIC_TIMER0;		// Deshabilito Interrupcion TIMER0
			T0->MCR _RESET_BIT(MR0I);	// Desactivo interrupcion match 0
			T0->TCR = 2;				// Apago y reseteo el temporizador
			T0->MR0 = 0;				// Restablezco el Match Register
		}
		else
		{
			t.MR0isOn = nextTimer();
			T0->MR0 = t.timer[t.MR0isOn].MR;
		}
		return SUCCESS;
	}
	return ERROR;
}

uint8_t timerLoop(pTimer_id_t id, bool loop_state)
{
	if(*id < 0 || *id >= N_TIMERS || !t.timer[*id].state)
		return ERROR;

	t.timer[*id].looping = loop_state;
	return SUCCESS;
}

bool timer_isLooping(pTimer_id_t id)
{
	if(*id < 0 || *id >= N_TIMERS)
		return false;
	return t.timer[*id].looping;
}

/**  END FUNCIONES DE USUARIO  */


// las proximas funciones podrian (o no) estar en Drivers
/*
static m_timers_t timer_init(void)
{
	int8_t i;
	for(i=0; i<N_TIMERS; i++)
	{
		t.timer[i].MR = 0;
		t.timer[i].timeSet  = 0;
		t.timer[i].state = 0;
		t.timer[i].handler = NULL;
	}
	t.active = 0;
	t.MR0isOn = 0;
	return t;
}*/

// busca el primer timer que no este en uso
static timer_id_t seekAvailableTimer(void)
{
	uint8_t n;

	if(t.active < N_TIMERS)
	{
		for(n=0 ; n<N_TIMERS ; n++)
		{
			if( !t.timer[n].state )
			{
				return (timer_id_t) n;
			}
		}
	}

	return -1; // not found
}

// TODO: creo que esta funcion no contempla overflow
// busca el timer activo más próximo a terminar
static timer_id_t nextTimer()
{
	uint32_t nextMR;
	uint8_t nextPos;
	uint8_t i=0;

	if(!t.active)
		return -1;

	// busco el primer timer en uso
	while(!t.timer[i].state && i < N_TIMERS) i++;

	nextMR = t.timer[i].MR;
	nextPos = i;

	for(; i<N_TIMERS; i++)
	{
		if( t.timer[i].state && t.timer[i].MR < nextMR)
		{
			nextMR = t.timer[i].MR;
			nextPos =  i;
		}
	}
	return (timer_id_t) nextPos;
}

void timerEnded(void)
{
	do {
		timer_id_t   id = t.MR0isOn;
		timer_cb_t   cb = t.timer[id].handler;
		timer_cb2_t cb2 = t.timer[id].handler2;
		void*      data = t.timer[id].data;
		if(t.timer[id].looping) {
			// restart timer
			t.timer[id].MR = T0->TC + t.timer[id].timeSet;

			t.MR0isOn = nextTimer();
			T0->MR0 = t.timer[t.MR0isOn].MR;
		}
		else {
			if(t.timer[id].id)
				stopTimer(t.timer[id].id);
			else
				stopTimer(&id);
		}
		if(cb)
			cb();
		else if(cb2)
			cb2(data);
	}
	// mientras haya temporizaciones corriendo, y el match ya haya pasado
	while ( (T0->TCR & 0x01) && (T0->TC >= T0->MR0) );
}
/*T0->MR0 <= T0->TC*/
