#ifndef _CHIP_H
#define _CHIP_H

#include "lpc_types.h"
#include "regsLPC1769.h"

#ifndef CHIP_LPC175X_6X
#define CHIP_LPC175X_6X
#endif

//////////////Registros del CLOCK y de sistema/////////////////
//#define		LPC_SYSCTL_BASE		0x400FC000
//0x400FC1A0UL: Registro de control de sistema y registro de status:
#define		DIR_SCS				( (__RW uint32_t *) 0x400FC1A0UL)
//0x400FC104UL: Registro de configuracion del clock:
#define		DIR_CCLKCFG			( (__RW uint32_t *) 0x400FC104UL)
//0x400FC10CUL: Registro de seleccion del clock:
#define		DIR_CLKSRCSEL		( (__RW uint32_t *) 0x400FC10CUL)
//0x400FC1C8UL: Clock Output Config register:
#define		DIR_CLKOUTCFG		( (__RW uint32_t *) 0x400FC1C8UL)
//0x400FC000UL: Flash access configuration:
#define		DIR_FLASHCFG		( (__RW uint32_t *) 0x400FC000UL)

/////////////////Registros de los PLL///////////////////////////
#define		PLL0				( (__RW uint32_t *) 0x400FC080UL)
//0x400FC080UL: Registro de control del PLL0:
#define		DIR_PLL0CON			( (__RW uint32_t *) 0x400FC080UL)
//0x400FC084UL: Registro de configuracion del PLL0:
#define		DIR_PLL0CFG			( (__RW uint32_t *) 0x400FC084UL)
//0x400FC088UL: Registro de estado del PLL0:
#define		DIR_PLL0STAT		( (__RW uint32_t *) 0x400FC088UL)
//0x400FC08CUL: Registro de control del PLL0:
#define		DIR_PLL0FEED		( (__RW uint32_t *) 0x400FC08CUL)
//0x400FC0A0UL: Registro de control del PLL1:
#define		DIR_PLL1CON			( (__RW uint32_t *) 0x400FC0A0UL)
//0x400FC0A4UL: Registro de configuracion del PLL1:
#define		DIR_PLL1CFG			( (__RW uint32_t *) 0x400FC0A4UL)
//0x400FC0A8UL: Registro de estado del PLL1:
#define		DIR_PLL1STAT		( (__RW uint32_t *) 0x400FC0A8UL)
//0x400FC0ACUL: Registro de control del PLL1:
#define		DIR_PLL1FEED		( (__RW uint32_t *) 0x400FC0ACUL)

//Registro de status y configuracion del sistema:
#define		SCS			DIR_SCS[0]
#define 	FLASHCFG	DIR_FLASHCFG[0]

//Registros de control del CLOCK:
#define		CCLKCFG		DIR_CCLKCFG[0]
#define		CLKSRCSEL	DIR_CLKSRCSEL[0]
#define		CLKOUTCFG	DIR_CLKOUTCFG[0]

//PLL0:
#define		PLL0CON		DIR_PLL0CON[0]
#define		PLL0CFG		DIR_PLL0CFG[0]
#define		PLL0STAT	DIR_PLL0STAT[0]
#define		PLL0FEED	DIR_PLL0FEED[0]

//PLL1:
#define		PLL1CON		DIR_PLL1CON[0]
#define		PLL1CFG		DIR_PLL1CFG[0]
#define		PLL1STAT	DIR_PLL1STAT[0]
#define		PLL1FEED	DIR_PLL1FEED[0]

//Valores para configuracion del PLL:
#define CLOCK_SETUP_Value 		1
#define SCS_Value				0x00000020
#define CLKSRCSEL_Value         0x00000001
#define PLL0_SETUP_Value        1
#define PLL0CFG_Value           0x00050063
#define PLL1_SETUP_Value        1
#define PLL1CFG_Value           0x00000023
#define CCLKCFG_Value           0x00000003
#define USBCLKCFG_Value         0x00000000
#define PCLKSEL0_Value          0x00000000
#define PCLKSEL1_Value          0x00000000
#define PCONP_Value             0x042887DE
#define CLKOUTCFG_Value         0x00000000
#define FLASHCFG_Value			0x00004000


#define SYSCTL_OSCRANGE_15_25 (1 << 4)	/*!< SCS register - main oscillator range 15 to 25MHz */
#define SYSCTL_OSCEC          (1 << 5)	/*!< SCS register - main oscillator enable */
#define SYSCTL_OSCSTAT        (1 << 6)	/*!< SCS register - main oscillator is ready status */

#define SYSCTL_IRC_FREQ (4000000)

#define SYSCTL_PLL_ENABLE   (1 << 0)	/*!< PLL enable flag */
#define SYSCTL_PLL_CONNECT  (1 << 1)	/*!< PLL connect flag only applies to 175x/6x */


#define PLL0STS_ENABLED   (1 << 24)	/*!< PLL0 enable flag */
#define PLL0STS_CONNECTED (1 << 25)	/*!< PLL0 connect flag */
#define PLL0STS_LOCKED    (1 << 26)	/*!< PLL0 connect flag */
#define PLL1STS_ENABLED   (1 << 8)	/*!< PLL1 enable flag */
#define PLL1STS_CONNECTED (1 << 9)	/*!< PLL1 connect flag */
#define PLL1STS_LOCKED    (1 << 10)	/*!< PLL1 connect flag */

#define LPC_SYSCTL		((LPC_SYSCTL_T *) LPC_SYSCTL_BASE)

/* LPC17XX/40XX Clock and Power PLL register block structure */
typedef struct {
	__RW uint32_t PLLCON;	/*!< (R/W)  PLL Control Register */
	__RW uint32_t PLLCFG;	/*!< (R/W)  PLL Configuration Register */
	__R  uint32_t PLLSTAT;	/*!< (R/ )  PLL Status Register */
	__W  uint32_t PLLFEED;	/*!< ( /W)  PLL Feed Register */
	uint32_t RESERVED1[4];
} SYSCTL_PLL_REGS_T;

#define		PLL		((SYSCTL_PLL_REGS_T *) 0x400FC080UL)

/* Selectable PLLs */
typedef enum {
	SYSCTL_MAIN_PLL,		/*!< Main PLL (PLL0) */
	SYSCTL_USB_PLL,			/*!< USB PLL (PLL1) */
} SYSCTL_PLL_T;

typedef enum CCLKSRC {
	CCLKSRC_SYSCLK,			/*!< Select Sysclk as the input to the CPU clock divider. */
	CCLKSRC_MAINPLL			/*!< Select the output of the Main PLL as the input to the CPU clock divider. */
} CCLKSRC_T;


/* PLL source clocks */
typedef enum PLLCLKSRC {
	PLLCLKSRC_IRC,			/*!< PLL is sourced from the internal oscillator (IRC) */
	PLLCLKSRC_MAINOSC,		/*!< PLL is sourced from the main oscillator */
	PLLCLKSRC_RTC,			/*!< PLL is sourced from the RTC oscillator */
	PLLCLKSRC_RESERVED
} PLLCLKSRC_T;



//!< ///////////////////   PCLKSEL   //////////////////////////
/**
 * Peripheral clock divider rates used with the
 * PCLKSEL_T clock types (devices only)
 */
typedef enum {
	PCLKDIV_4,					/*!< Divider by 4 */
	PCLKDIV_1,					/*!< Divider by 1 */
	PCLKDIV_2,					/*!< Divider by 2 */
	PCLKDIV_8,					/*!< Divider by 8, not for use with CAN */
	PCLKDIV_6_CCAN = PCLKDIV_8	/*!< Divider by 6, CAN only */
} PCLKDIV_T;

/**
 * Peripheral clock selection for LPC175x/6x
 * This is a list of clocks that can be divided on the 175x/6x
 */
typedef enum {
	PCLKSEL_WDT,		/*!< Watchdog divider */
	PCLKSEL_TIMER0,		/*!< Timer 0 divider */
	PCLKSEL_TIMER1,		/*!< Timer 1 divider */
	PCLKSEL_UART0,		/*!< UART 0 divider */
	PCLKSEL_UART1,		/*!< UART 1 divider */
	PCLKSEL_RSVD5,
	PCLKSEL_PWM1,		/*!< PWM 1 divider */
	PCLKSEL_I2C0,		/*!< I2C 0 divider */
	PCLKSEL_SPI,		/*!< SPI divider */
	PCLKSEL_RSVD9,
	PCLKSEL_SSP1,		/*!< SSP 1 divider */
	PCLKSEL_DAC,		/*!< DAC divider */
	PCLKSEL_ADC,		/*!< ADC divider */
	PCLKSEL_CAN1,		/*!< CAN 1 divider */
	PCLKSEL_CAN2,		/*!< CAN 2 divider */
	PCLKSEL_ACF,		/*!< ACF divider */
	PCLKSEL_QEI,		/*!< QEI divider */
	PCLKSEL_GPIOINT,	/*!< GPIOINT divider */
	PCLKSEL_PCB,		/*!< PCB divider */
	PCLKSEL_I2C1,		/*!< I2C 1 divider */
	PCLKSEL_RSVD20,
	PCLKSEL_SSP0,		/*!< SSP 0 divider */
	PCLKSEL_TIMER2,		/*!< Timer 2 divider */
	PCLKSEL_TIMER3,		/*!< Timer 3 divider */
	PCLKSEL_UART2,		/*!< UART 2 divider */
	PCLKSEL_UART3,		/*!< UART 3 divider */
	PCLKSEL_I2C2,		/*!< I2C 2 divider */
	PCLKSEL_I2S,		/*!< I2S divider */
	PCLKSEL_RSVD28,
	PCLKSEL_RIT,		/*!< Repetitive timer divider */
	PCLKSEL_SYSCON,		/*!< SYSCON divider */
	PCLKSEL_MCPWM		/*!< Motor control PWM divider */
} PCLKSEL_T;



//!< ///////////////////   PCONP   //////////////////////////

#define 	PCONP_TIMER0	(1UL << 1)
#define 	PCONP_TIMER1	(1UL << 2)
#define 	PCONP_UART0		(1UL << 3)
#define 	PCONP_UART1		(1UL << 4)

#define 	PCONP_PWM1		(1UL << 6)
#define 	PCONP_I2C0		(1UL << 7)
#define 	PCONP_SPI		(1UL << 8)
#define 	PCONP_RTC		(1UL << 9)
#define 	PCONP_SSP1		(1UL << 10)

#define 	PCONP_ADC		(1UL << 12)
#define 	PCONP_CAN1		(1UL << 13)
#define 	PCONP_CAN2		(1UL << 14)
#define 	PCONP_GPIO		(1UL << 15)
#define 	PCONP_RIT		(1UL << 16)
#define 	PCONP_MCPWM		(1UL << 17)
#define 	PCONP_QEI		(1UL << 18)
#define 	PCONP_I2C1		(1UL << 19)

#define 	PCONP_SSP0		(1UL << 21)
#define 	PCONP_TIM2		(1UL << 22)
#define 	PCONP_TIM3		(1UL << 23)
#define 	PCONP_UART2		(1UL << 24)
#define 	PCONP_UART3		(1UL << 25)
#define 	PCONP_I2C2		(1UL << 26)
#define 	PCONP_I2S		(1UL << 27)

#define 	PCONP_GPDMA		(1UL << 29)
#define 	PCONP_ENET		(1UL << 30)
#define 	PCONP_USB		(1UL << 31)


/* FLASH Access time definitions */
typedef enum {
	FLASHTIM_20MHZ_CPU = 0,		/*!< Flash accesses use 1 CPU clocks. Use for up to 20 MHz CPU clock */
	FLASHTIM_40MHZ_CPU = 1,		/*!< Flash accesses use 2 CPU clocks. Use for up to 40 MHz CPU clock */
	FLASHTIM_60MHZ_CPU = 2,		/*!< Flash accesses use 3 CPU clocks. Use for up to 60 MHz CPU clock */
	FLASHTIM_80MHZ_CPU = 3,		/*!< Flash accesses use 4 CPU clocks. Use for up to 80 MHz CPU clock */
	FLASHTIM_100MHZ_CPU = 4,	/*!< Flash accesses use 5 CPU clocks. Use for up to 100 MHz CPU clock */
	FLASHTIM_120MHZ_CPU = 4,	/*!< Flash accesses use 5 CPU clocks. Use for up to 120 Mhz for LPC1759 and LPC1769 only.*/
	FLASHTIM_SAFE_SETTING = 5,	/*!< Flash accesses use 6 CPU clocks. Safe setting for any allowed conditions */
} FMC_FLASHTIM_T;


/** System oscillator rate
 * This value is defined externally to the chip layer and contains
 * the value in Hz for the external oscillator for the board. If using the
 * internal oscillator, this rate can be 0.
 */
extern const uint32_t OscRateIn;

/** RTC oscillator rate
 * This value is defined externally to the chip layer and contains
 * the value in Hz for the RTC oscillator for the board. This is
 * usually 32KHz (32768). If not using the RTC, this rate can be 0.
 */
extern const uint32_t RTCOscRateIn;

/* Current system clock rate, mainly used for sysTick */
extern uint32_t SystemCoreClock;



// Prototipos de función:
void      PLL_init  ( void );
void      power_init( void );
uint32_t  getPCLKDiv(PCLKSEL_T clk);
void      setPCLKDiv(PCLKSEL_T clk, PCLKDIV_T div);
CCLKSRC_T getCPUClockSource(void);
uint32_t  getSYSCLKRate(void);
uint32_t  getMainClockRate(void);
uint32_t  getMainPLLOutClockRate(void);


static inline uint32_t getCPUClockDiv(void)
{
	return (CCLKCFG & 0xFF) + 1;
}

static inline PLLCLKSRC_T getMainPLLSource(void)
{
	return (PLLCLKSRC_T) (CLKSRCSEL & 0x3);
}

static inline uint32_t getMainPLLInClockRate(void)
{
	return getSYSCLKRate();
}

static inline uint32_t getPLLStatus(SYSCTL_PLL_T PLLNum)
{
	return PLL[PLLNum].PLLSTAT;
}

static inline bool isMainPLLEnabled(void)
{
	return (bool) ((PLL0STAT & PLL0STS_ENABLED) != 0);
}

static inline bool isMainPLLConnected(void)
{
	return (bool) ((PLL0STAT & PLL0STS_CONNECTED) != 0);
}

static inline bool isMainPLLLocked(void)
{
	return (bool) ((PLL0STAT & PLL0STS_LOCKED) != 0);
}

static inline bool isUSBPLLEnabled(void)
{
	return (bool) ((PLL1STAT & PLL1STS_ENABLED) != 0);
}

static inline bool isUSBPLLConnected(void)
{
	return (bool) ((PLL1STAT & PLL1STS_CONNECTED) != 0);
}

static inline bool isUSBPLLLocked(void)
{
	return (bool) ((PLL1STAT & PLL1STS_LOCKED) != 0);
}

static inline uint32_t getSystemClockRate(void)
{
	return getMainClockRate() / getCPUClockDiv();
}

static inline void SystemCoreClockUpdate(void)
{
	SystemCoreClock = getSystemClockRate();
}

static inline void setCPUClockDiv(uint32_t div)
{
	CCLKCFG = div;
}

static inline void setMainPLLSource(PLLCLKSRC_T src)
{
	CLKSRCSEL = src;
}

static inline bool isCrystalEnabled(void)
{
	return (SCS & SYSCTL_OSCSTAT) != 0;
}

static inline void enableCrystal(void)
{
	SCS |= SYSCTL_OSCEC;
}

static inline void setFLASHAccess(FMC_FLASHTIM_T clks)
{
	uint32_t tmp = FLASHCFG & 0xFFF;

	/* Don't alter lower bits */
	FLASHCFG = tmp | (clks << 12);
}

/**
 * @brief	Enables or connects a PLL
 * @param	PLLNum:	PLL number
 * @param	flags:	SYSCTL_PLL_ENABLE or SYSCTL_PLL_CONNECT
 * @return	Nothing
 * @note	This will also perform a PLL feed sequence. Connect only applies to the
 * LPC175x/6x devices.
 */
void enablePLL(SYSCTL_PLL_T PLLNum, uint32_t flags);
void disablePLL(SYSCTL_PLL_T PLLNum, uint32_t flags);
/**
 * @brief	Feeds a PLL
 * @param	PLLNum:	PLL number
 * @return	Nothing
 */
static inline void feedPLL(SYSCTL_PLL_T PLLNum)
{
	PLL[PLLNum].PLLFEED = 0xAA;
	PLL[PLLNum].PLLFEED = 0x55;
}


void setupPLL(SYSCTL_PLL_T PLLNum, uint32_t msel, uint32_t psel);

void USB_Init(void);

void USBD_Init(uint32_t port);

void setupIrcClocking(void);
void setupXtalClocking(void);
void setupClocking(void);

void chip_init(void);


static inline void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* enable interrupt */
}

static inline void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* disable interrupt */
}

static inline void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* disable interrupt */
}

static inline void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  ISPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* disable interrupt */
}

static inline void SysTick_enableInterrupts(void)
{
	SysTick->TICKINT = 1;
}

static inline void SysTick_disableInterrupts(void)
{
	SysTick->TICKINT = 0;
}


#endif /* _CHIP_H */
