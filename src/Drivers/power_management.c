#include "chip.h"
#include "power_management.h"

#define PMU_PCON_PM0_FLAG		(1UL << 0)
#define PMU_PCON_PM1_FLAG		(1UL << 1)
#define SCB_SCR_SLEEPDEEP_FLAG	(1UL << 2)

/* Enter MCU Sleep mode */
void Power_Sleep()
{
	/* Select Sleep/Deep Sleep mode */
	PCON &= ~(PMU_PCON_PM1_FLAG | PMU_PCON_PM0_FLAG);

	/* Clearing SLEEPDEEP bit in SCR makes it Sleep mode */
	SCB_SCR &= ~SCB_SCR_SLEEPDEEP_FLAG;

	SCB_SCR |=  (1UL << 4); // all interrupts can resume
	SCB_SCR &= ~(1UL << 1); // do not sleep when returning to thread mode

	/* Enter sleep mode */
	__WFI();
}


/* Enter MCU Deep Sleep mode */
void Power_DeepSleep()
{
	/* Select Sleep/Deep Sleep mode */
	PCON &= ~(PMU_PCON_PM1_FLAG | PMU_PCON_PM0_FLAG);

	/* Setting SLEEPDEEP bit in SCR makes it Deep Sleep mode */
	SCB_SCR |= SCB_SCR_SLEEPDEEP_FLAG;

	SCB_SCR |=  (1UL << 4); // all interrupts can resume
	SCB_SCR &= ~(1UL << 1); // do not sleep when returning to thread mode

	/* Enter sleep mode */
	__WFI();
}

/* Enter MCU Power down mode */
void Power_PowerDown()
{
	/* Select power down mode */
	PCON = (PCON & ~PMU_PCON_PM1_FLAG) | PMU_PCON_PM0_FLAG;

	/* Setting SLEEPDEEP bit in SCR makes it power down mode */
	SCB_SCR |= SCB_SCR_SLEEPDEEP_FLAG;

	/* Enter sleep mode */
	__WFI();
}

/* Enter MCU Deep Power down mode */
void Power_DeepPowerDown()
{
	/* Select deep power down mode */
	PCON |= PMU_PCON_PM1_FLAG | PMU_PCON_PM0_FLAG;

	/* Setting SLEEPDEEP bit in SCR makes it deep power down mode */
	SCB_SCR |= SCB_SCR_SLEEPDEEP_FLAG;

	/* Enter sleep mode */
	__WFI();
}

/* Put some of the peripheral in sleep mode */
void PMU_Sleep(PMU_MCUPOWER_T SleepMode)
{
	if (SleepMode == PMU_MCU_DEEP_SLEEP) {
		Power_DeepSleep();
	}
	else if (SleepMode == PMU_MCU_POWER_DOWN) {
		Power_PowerDown();
	}
	else if (SleepMode == PMU_MCU_DEEP_PWRDOWN) {
		Power_DeepPowerDown();
	}
	else {	/* PMU_MCU_SLEEP */
		Power_Sleep();
	}
}


