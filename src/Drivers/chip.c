#include "chip.h"
#include "regsLPC1769.h"
#include "GPIO.h"

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
#if 0
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
#endif
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




void setupIrcClocking(void)
{
	/* Disconnect the Main PLL if it is connected already */
	if (isMainPLLConnected()) {
		disablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_CONNECT);
	}

	/* Disable the PLL if it is enabled */
	if (isMainPLLEnabled()) {
		disablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);
	}

	setCPUClockDiv(0);
	setMainPLLSource(PLLCLKSRC_IRC);

	/* FCCO = ((44+1) * 2 * 4MHz) / (0+1) = 360MHz */
	setupPLL(SYSCTL_MAIN_PLL, 44, 0);

	enablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);

	setCPUClockDiv(2);
	while (!isMainPLLLocked()) {} /* Wait for the PLL to Lock */

	enablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_CONNECT);
}

void setupXtalClocking(void)
{
	/* Disconnect the Main PLL if it is connected already */
	if (isMainPLLConnected()) {
		disablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_CONNECT);
	}

	/* Disable the PLL if it is enabled */
	if (isMainPLLEnabled()) {
		disablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);
	}

	/* Enable the crystal */
	if (!isCrystalEnabled())
		enableCrystal();
	while(!isCrystalEnabled()) {}

	/* Set PLL0 Source to Crystal Oscillator */
	setCPUClockDiv(0);
	setMainPLLSource(PLLCLKSRC_MAINOSC);

	/* FCCO = ((15+1) * 2 * 12MHz) / (0+1) = 384MHz */
	setupPLL(SYSCTL_MAIN_PLL, 15, 0);

	enablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_ENABLE);

	/* 384MHz / (3+1) = 96MHz */
	setCPUClockDiv(3);
	while (!isMainPLLLocked()) {} /* Wait for the PLL to Lock */

	enablePLL(SYSCTL_MAIN_PLL, SYSCTL_PLL_CONNECT);
}

/* Setup system clocking */
void setupClocking(void)
{
	setupXtalClocking();

	/* Setup FLASH access to 4 clocks (100MHz clock) */
	setFLASHAccess(FLASHTIM_100MHZ_CPU);
}



/* Sets up USB PLL, all needed clocks and enables USB PHY on the chip. USB pins which are
	muxed to different pads are not initialized here. This routine assumes that the XTAL
	OSC is enabled and running prior to this call. */
void USB_Init(void)
{
	/* Setup USB PLL1 for a 48MHz clock
	   Input clock rate (FIN) is main oscillator = 12MHz
	   PLL1 Output = USBCLK = 48MHz = FIN * MSEL, so MSEL = 4.
	   FCCO = USBCLK = USBCLK * 2 * P. It must be between 156 MHz to 320 MHz.
	   so P = 2 and FCCO = 48MHz * 2 * 2 = 192MHz */
	setupPLL(SYSCTL_USB_PLL, 3, 1);	/* Multiply by 4, Divide by 2 */

	/* Use PLL1 output as USB Clock Source */
	/* Enable PLL1 */
	enablePLL(SYSCTL_USB_PLL, SYSCTL_PLL_ENABLE);

	/* Wait for PLL1 to lock */
	while (!isUSBPLLLocked()) {}

	/* Connect PLL1 */
	enablePLL(SYSCTL_USB_PLL, SYSCTL_PLL_ENABLE | SYSCTL_PLL_CONNECT);

	/* Wait for PLL1 to be connected */
	while (!isUSBPLLConnected()) {}

	/* Enable AHB clock to the USB block and USB RAM. */
	PCONP |= PCONP_USB;
}

void USBD_Init(uint32_t port)
{
	/* VBUS is not connected on the NXP LPCXpresso LPC1769, so leave the pin at default setting. */
	/*Chip_IOCON_PinMux(LPC_IOCON, 1, 30, IOCON_MODE_INACT, IOCON_FUNC2);*/ /* USB VBUS */
	setPINSEL(1, 30, PINSEL_FUNC2);
	setPINMODE(1, 30, PINMODE_NONE);

	/* P0.29 D1+, P0.30 D1- */
	setPINSEL(0, 29, PINSEL_FUNC1);
	setPINMODE(0, 29, PINMODE_NONE);
	setPINSEL(0, 30, PINSEL_FUNC1);
	setPINMODE(0, 30, PINMODE_NONE);

	USBClkCtrl = 0x12;                /* Dev, AHB clock enable */
	while ((USBClkSt & 0x12) != 0x12);
}

void disablePLL(SYSCTL_PLL_T PLLNum, uint32_t flags) {
	uint32_t temp;

	temp = PLL[PLLNum].PLLCON;
	temp &= ~flags;
	PLL[PLLNum].PLLCON = temp;
	feedPLL(PLLNum);
}

/* Enables or connects a PLL */
void enablePLL(SYSCTL_PLL_T PLLNum, uint32_t flags) {
	uint32_t temp;

	temp = PLL[PLLNum].PLLCON;
	temp |= flags;
	PLL[PLLNum].PLLCON = temp;
	feedPLL(PLLNum);
}
/* Sets up a PLL */
void setupPLL(SYSCTL_PLL_T PLLNum, uint32_t msel, uint32_t psel) {
	uint32_t PLLcfg;

	/* PLL0 and PLL1 are slightly different */
	if (PLLNum == SYSCTL_MAIN_PLL) {
		PLLcfg = (msel) | (psel << 16);
	}
	else {
		PLLcfg = (msel) | (psel << 5);
	}

	PLL[PLLNum].PLLCFG = PLLcfg;
	PLL[PLLNum].PLLCON = 0x1;
	feedPLL(PLLNum);
}
