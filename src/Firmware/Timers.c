#include <TPO.h>




void timerEnd_Handler(uint8_t n)
{
	if (n == 0)
		write_pin(0,22,1);
	else if(n == 1)
		write_pin(0,22,0);

//	timer(7,ON,20000);
	if(n==0 || n==7)
		setearTimers();

}

void setearTimers()
{
	timer(0,ON,2000);
	timer(1,ON,1900);
	timer(7,ON,20000);
}

void TIMER0_IRQHandler (void)
{
	if(T0->IR_.MR0)
	{
		// Interrumpió match 0 -> terminó un timer
		T0->IR_.MR0 = 1;	// Borro flag del Match 0
		timerEnded();
	}

    if(T0->IR_.MR1)
    {
    	// Interrumpió match 1 -> overflow
    	T0->IR_.MR1 = 1; 	// Borro flag del Match 1
		// do stuff
    }
}


int32_t timer(int8_t n, uint8_t action, uint32_t time) // time [ms]
{
	static uint8_t timerState[N_TIMERS] = {0,0,0,0,0,0,0,0};
	static uint8_t timersActivos = 0;
	static uint32_t timerCount[N_TIMERS];
	static int8_t MR0isOnTimer = 0;

	switch (action)
	{
	case OFF:
		if(timerState[n]) // si esta encendido
		{
			timerState[n] = 0;
			timersActivos --;
			if(!timersActivos) // apago t0do
			{
				ICER0 |= (0x01 << 1);	// Deshabilito Interrupcion TIMER0
				T0->MCR_.MR0I = 0;		// Desactivo interrupcion match 0
				T0->TCR = 2;			// Apago y reseteo el temporizador
				T0->MR0 = 0;			// Restablezco el Match Register
			}
			else
			{
				MR0isOnTimer = nextTimer(timerCount,timerState,N_TIMERS);
				T0->MR0 = timerCount[MR0isOnTimer];
			}
		}
		break;
	case ON:
		if(timersActivos == N_TIMERS)
			return -1;

		if(!timerState[n]) // si estaba apagado
		{
			timerState[n] = ON;
			timersActivos++;
		}
		timerCount[n] = T0->TC + time;

		if(!(timersActivos - 1)) // si no habia ninguno prendido
		{
			T0->MR0 = timerCount[n];// Seteo temporizacion
			T0->MCR_.MR0I = 1;		// Interrumpe en match0
			MR0isOnTimer = n;
			T0->TCR = 1;			// Enciendo el temporizador
			ISER0 |= (0x01 << 1);	// Habilito Interrupcion TIMER0
		}
		else
		{
			// si el timer actual termina antes que el seteado en Match Register, actualizo el MR.
			if(T0->MR0 > timerCount[n])
			{
				T0->MR0 = timerCount[n];
				MR0isOnTimer = n;
			}
		}
		break;

	case TIMER_FINISHED:
		do
		{
			n = MR0isOnTimer;
			timer(n, OFF, 0); // stopTimer(n);
			timerEnd_Handler(n);
		} while(T0->TCR_.CE && T0->MR0 < T0->TC);
		break;

	case IS_TIMER_END:
		return !timerState[n];

	default:
		break;
	}

	return 0;
}


int8_t nextTimer(uint32_t* timerCount, uint8_t* timerState, uint8_t size)
{
	uint32_t nextNum;
	uint8_t nextPos;
	uint8_t i=0;

	while(!timerState[i] && i <= size) i++; // busco el primer timer en uso
	if(i > size)
		return -1;
	nextNum = timerCount[i];
	nextPos = i;

	for(; i<size; i++)
	{
		if( timerState[i] && timerCount[i] < nextNum)
		{
			nextNum = timerCount[i];
			nextPos =  i;
		}
	}
	return nextPos;
}

uint8_t isTimerEnd(uint8_t n)
{
	return timer(n,IS_TIMER_END,0);
}

void stopTimer(uint8_t n)
{
	timer(n, OFF, 0);
}

void timerEnded(void)
{
	timer(0,TIMER_FINISHED,0);
}
