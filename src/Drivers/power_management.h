#ifndef DRIVERS_POWER_MANAGEMENT_H_
#define DRIVERS_POWER_MANAGEMENT_H_

typedef enum CHIP_PMU_MCUPOWER {
	PMU_MCU_SLEEP = 0,		/*!< Sleep mode */
	PMU_MCU_DEEP_SLEEP,		/*!< Deep Sleep mode */
	PMU_MCU_POWER_DOWN,		/*!< Power down mode */
	PMU_MCU_DEEP_PWRDOWN	/*!< Deep power down mode */
} PMU_MCUPOWER_T;

void Power_Sleep();
void Power_DeepSleep();
void Power_PowerDown();
void Power_DeepPowerDown();
void PMU_Sleep(PMU_MCUPOWER_T SleepMode);

#define __ASM __asm

__attribute__( ( always_inline ) ) static inline void __WFI(void) {
  __ASM volatile ("wfi");
}

#endif /* DRIVERS_POWER_MANAGEMENT_H_ */
