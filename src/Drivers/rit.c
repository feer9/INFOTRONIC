/*
 * rit.c
 *
 *  Created on: 26 jul. 2019
 *      Author: feer
 */

#include "chip.h"

// receive time in miliseconds
void RIT_init(uint32_t us)
{
	PCONP |= PCONP_RIT;     // habilito el periferico

	// pos de RIT en PCLKSEL1 = [27:26]
	uint32_t tmp = PCLKSEL1 &= ~(3U << 26);			// limpio el campo del RIT
	PCLKSEL1 = tmp | ((uint32_t)PCLKDIV_8 << 26);	// seteo el divisor de clock CCLK / 8 = 12,5MHz

	LPC_RITIMER->COMPVAL = ((SystemCoreClock / 8) / 1e6) * us;
	// (CCLK / 8) -> 1s   =>  that / 1000000 -> 1us

	LPC_RITIMER->MASK  = 0x0UL;
	LPC_RITIMER->COUNTER   = 0x0UL;

	// timer enabled, stopped when debugger halts the processor,
	//  and counter cleared when reaches compval
	LPC_RITIMER->CTRL = RIT_CTRL_TEN | RIT_CTRL_ENBR | RIT_CTRL_ENCLR;

	// clear interrupt flag
	LPC_RITIMER->CTRL |= RIT_CTRL_INT;

	// enables interrupt
	ISER0 = NVIC_RIT;
}
