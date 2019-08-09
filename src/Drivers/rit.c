/*
 * rit.c
 *
 *  Created on: 26 jul. 2019
 *      Author: feer
 */

#include "chip.h"
#include "rit.h"

// receive time in miliseconds
void RIT_init(uint32_t us)
{
	// ensure interrupts are disabled
	ICER0 = NVIC_RIT;

	// enable peripheral
	PCONP |= PCONP_RIT;

	// RIT position in PCLKSEL1 = [27:26]
	uint32_t tmp = PCLKSEL1 &= ~(3U << 26);			// clear RIT PCLKSEL field
	PCLKSEL1 = tmp | ((uint32_t)PCLKDIV_8 << 26);	// set clock divisor: CCLK / 8 = 12,5MHz

	LPC_RITIMER->COMPVAL = ((SystemCoreClock / 8) / 1e6) * us;
	// (CCLK / 8) -> 1s   =>  that / 1000000 -> 1us

	LPC_RITIMER->MASK  = 0x0UL;
	LPC_RITIMER->COUNTER   = 0x0UL;

	// timer enabled, stopped when debugger halts the processor,
	//  and counter cleared when reaches compval
	LPC_RITIMER->CTRL = RIT_CTRL_TEN | RIT_CTRL_ENBR | RIT_CTRL_ENCLR;

	// clear interrupt flag
	LPC_RITIMER->CTRL |= RIT_CTRL_INT;
}

inline void RIT_enableInterrupts(void)
{
	ISER0 = NVIC_RIT;
}

inline void RIT_disableInterrupts(void)
{
	ICER0 = NVIC_RIT;
}
