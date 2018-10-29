#include <Timer.h>


//static m_timers_t	timer_init (void);
static int8_t		seekAvailableTimer (void);
static int8_t		nextTimer (void);

m_timers_t t = {
		.timer[0].state = 0, .timer[1].state = 0, .timer[2].state = 0,
		.timer[3].state = 0, .timer[4].state = 0, .timer[5].state = 0,
		.timer[6].state = 0, .timer[7].state = 0, .timer[8].state = 0,
		.timer[9].state = 0,
		.active = 0,
		.MR0isOn = 0
};



/**  FUNCIONES DE USUARIO */

// devuelve 1 si el timer NO está activo
uint8_t isTimerEnd(uint8_t id)
{
	return !t.timer[id].state;
}

// time: tiempo en ms , handler: funcion de callback, puede ser NULL
// devuelve el id del timer que se inició o -1 en caso de error
int8_t startTimer (uint32_t time, callback_t handler)
{
	int8_t id;

	id = seekAvailableTimer();
	if(id != -1) // si hay timers disponibles
	{
		t.timer[id].MR = T0->TC + time;
		t.timer[id].timeSet = time;
		t.timer[id].handler = handler;

		if(!(t.active))			// si no habia ninguno prendido
		{
			T0->MR0 = t.timer[id].MR;	// Seteo temporizacion
			T0->MCR _SET_BIT(0);			// Interrumpe en match0
			t.MR0isOn = id;
			T0->TCR = 1;					// Enciendo el temporizador
			ISER0 _SET_BIT(NVIC_TIMER0);	// Habilito Interrupcion TIMER0
		}
		else
		{
			// si el timer actual termina antes que el seteado en Match Register
			// actualizo  MR0
			if(t.timer[id].MR < T0->MR0)
			{
				T0->MR0 = t.timer[id].MR;
				t.MR0isOn = id;
			}
		}
		if(t.timer[id].state == OFF)
		{
			t.timer[id].state = ON;
			t.active ++;
		}
	}
	return id;
}

// funcion que vuelve a iniciar la cuenta atrás del timer [id]
// id: numero de timer
// retorna 0 si lo reinició, 1 si no estaba encendido (no tiene datos para encenderlo)
uint8_t restartTimer(uint8_t id)
{
	if(!t.timer[id].state)
		return 1;

	t.timer[id].MR = T0->TC + t.timer[id].timeSet;

	if(t.MR0isOn == id)
		T0->MR0 = t.timer[id].MR;

	return 0;
}

// devuelve 0 si detuvo el timer, 1 si no estaba encendido
uint8_t stopTimer(uint8_t n)
{
	if(!t.timer[n].state) // si esta apagado
		return 1;

	t.timer[n].state = 0;
	t.timer[n].MR = 0;
	t.timer[n].timeSet  = 0;
	t.timer[n].handler = NULL;
	t.active --;
	if(!t.active) // apago t0do
	{
		ICER0 = (0x01 << 1);	// Deshabilito Interrupcion TIMER0
		T0->MCR _RESET_BIT(0);	// Desactivo interrupcion match 0
		T0->TCR = 2;			// Apago y reseteo el temporizador
		T0->MR0 = 0;			// Restablezco el Match Register
	}
	else
	{
		t.MR0isOn = nextTimer();
		T0->MR0 = t.timer[t.MR0isOn].MR;
	}
	return 0;
}

/**  END FUNCIONES DE USUARIO  */


//TODO: las proximas funciones podrian (o no) estar en Drivers
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
static int8_t seekAvailableTimer(void)
{
	uint8_t i = -1;

	if(t.active < N_TIMERS)
		for(i=0 ; i<N_TIMERS ; i++)
			if( !t.timer[i].state )
				break;

	return i;
}

// TODO: creo que esta funcion no contempla overflow
// busca el timer activo más próximo a terminar
static int8_t nextTimer()
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
	return nextPos;
}

void timerEnded(void)
{
	uint8_t id;
	callback_t callback;
	do
	{
		id = t.MR0isOn;
		callback = t.timer[id].handler;
		stopTimer(id);
		if(callback != NULL)
			callback();
	}
	while ( (T0->TCR & _BIT(0)) && (T0->MR0 < T0->TC) );
}
