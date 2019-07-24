#include <clock.h>
#include "regsLPC1769.h"

uint32_t SystemCoreClock;



CCLKSRC_T getCPUClockSource(void)
{
	/* LPC175x/6x CPU clock source is based on PLL connect status */
	if (isMainPLLConnected()) {
		return CCLKSRC_MAINPLL;
	}
	else {
		return CCLKSRC_SYSCLK;
	}
}


/* On 175x/6x devices, this is the input clock to the CPU divider. */
uint32_t getMainClockRate(void)
{
	switch (getCPUClockSource()) {
	case CCLKSRC_MAINPLL:
		return getMainPLLOutClockRate();

	case CCLKSRC_SYSCLK:
		return getSYSCLKRate();

	default:
		return 0;
	}
}

/* Returns the main PLL output clock rate */
uint32_t getMainPLLOutClockRate(void)
{
	uint32_t clkhr = 0;

	/* Only valid if enabled */
	if (isMainPLLEnabled()) {
		uint32_t msel, nsel;

		/* PLL0 rate is (FIN * 2 * MSEL) / NSEL, get MSEL and NSEL */
		msel = 1 + (PLL0CFG & 0x7FFF);
		nsel = 1 + ((PLL0CFG >> 16) & 0xFF);
		clkhr = (getMainPLLInClockRate() * 2 * msel) / nsel;
	}

	return (uint32_t) clkhr;
}

uint32_t getSYSCLKRate(void)
{
	/* Determine clock input rate to SYSCLK based on input selection */
	switch (getMainPLLSource())
	{
	case (uint32_t) PLLCLKSRC_IRC:
		return SYSCTL_IRC_FREQ;

	case (uint32_t) PLLCLKSRC_MAINOSC:
		return OscRateIn;

	case (uint32_t) PLLCLKSRC_RTC:
		return RTCOscRateIn;
	default:
		break;
	}
	return 0;
}

/* Selects a clock divider for a peripheral */
void setPCLKDiv(PCLKSEL_T clk, PCLKDIV_T div)
{
	uint32_t temp, bitIndex, regIndex = (uint32_t) clk;

	/* Get register array index and clock index into the register */
	bitIndex = ((regIndex % 16) * 2);
	regIndex = regIndex / 16;

	/* Mask and update register */
	temp = PCLKSEL[regIndex] & ~(0x3U << bitIndex);
	temp |= (((uint32_t) div) << bitIndex);
	PCLKSEL[regIndex] = temp;
}

/* Gets a clock divider for a peripheral */
uint32_t getPCLKDiv(PCLKSEL_T clk)
{
	uint32_t div = 1, bitIndex, regIndex = ((uint32_t) clk) * 2;

	/* Get register array index and clock index into the register */
	bitIndex = regIndex % 32;
	regIndex = regIndex / 32;

	/* Mask and update register */
	div = PCLKSEL[regIndex];
	div = (div >> bitIndex) & 0x3;
	if (div == PCLKDIV_4) {
		div = 4;
	}
	else if (div == PCLKDIV_1) {
		div = 1;
	}
	else if (div == PCLKDIV_2) {
		div = 2;
	}
	else {
		/* Special case for CAN clock divider */
		if ((clk == PCLKSEL_CAN1) || (clk == PCLKSEL_CAN2) || (clk == PCLKSEL_ACF)) {
			div = 6;
		}
		else {
			div = 8;
		}
	}

	return div;
}

void PLL_init ( void )
{
	SCS       = SCS_Value;

	if (SCS_Value & (1 << 5))       /* If Main Oscillator is enabled      */
		while ((SCS & (1<<6)) == 0);/* Wait for Oscillator to be ready    */

	CCLKCFG   = CCLKCFG_Value;      /* Setup Clock Divider                */

	PCLKSEL0  = PCLKSEL0_Value;     /* Peripheral Clock Selection         */
	PCLKSEL1  = PCLKSEL1_Value;

	CLKSRCSEL = CLKSRCSEL_Value;    /* Select Clock Source for PLL0       */

	PLL0CFG   = PLL0CFG_Value;      /* configure PLL0                     */
	PLL0FEED  = 0xAA;
	PLL0FEED  = 0x55;

	PLL0CON   = 0x01;               /* PLL0 Enable                        */
	PLL0FEED  = 0xAA;
	PLL0FEED  = 0x55;

	while (!(PLL0STAT & (1<<26)));  /* Wait for PLOCK0                    */

	PLL0CON   = 0x03;               /* PLL0 Enable & Connect              */
	PLL0FEED  = 0xAA;
	PLL0FEED  = 0x55;

	while (!(PLL0STAT & ((1<<25) | (1<<24))));/* Wait for PLLC0_STAT & PLLE0_STAT */

	PLL1CFG   = PLL1CFG_Value;
	PLL1FEED  = 0xAA;
	PLL1FEED  = 0x55;

	PLL1CON   = 0x01;               /* PLL1 Enable                        */
	PLL1FEED  = 0xAA;
	PLL1FEED  = 0x55;

	while (!(PLL1STAT & (1<<10)));  /* Wait for PLOCK1                    */

	PLL1CON   = 0x03;               /* PLL1 Enable & Connect              */
	PLL1FEED  = 0xAA;
	PLL1FEED  = 0x55;

	while (!(PLL1STAT & ((1<< 9) | (1<< 8))));/* Wait for PLLC1_STAT & PLLE1_STAT */

	PCONP   = PCONP_Value;          /* Power Control for Peripherals      */

	CLKOUTCFG = CLKOUTCFG_Value;    /* Clock Output Configuration         */

	FLASHCFG  = (FLASHCFG & ~0x0000F000UL) | FLASHCFG_Value;
}


void power_init()
{
	PCONP = PCONP_RTC | PCONP_GPIO;
//	PCONP =   0b00000000000000001000001000000000;
/*	solo dejo encendidos los perifericos:
	bit	9	RTC
	bit	15	GPIO  */
}
