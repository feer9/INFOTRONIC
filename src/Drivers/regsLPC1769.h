#ifndef REGSLPC1769_H_
#define REGSLPC1769_H_


#include <lpc_types.h>



//!< ////////////////Registros PINSEL//////////////////////////////
//!< 0x4002C000UL : Direccion de inicio de los registros PINSEL
#define		PINSEL		( ( __RW uint32_t  * ) 0x4002C000UL )

#define		PINSEL0		PINSEL[0]	//!< PINSEL0--->P0[15:0] (0x4002C000)
#define		PINSEL1		PINSEL[1]	//!< PINSEL1--->P0[31:16](0x4002C004)
#define		PINSEL2		PINSEL[2]	//!< PINSEL2--->P1[15:0] (0x4002C008)
#define		PINSEL3		PINSEL[3]	//!< PINSEL3--->P1[31:16](0x4002C00C)
#define		PINSEL4		PINSEL[4]	//!< PINSEL4--->P2[15:0] (0x4002C010)
#define		PINSEL5		PINSEL[5]	//!< PINSEL5--->P2[31:16] NOT USED
#define		PINSEL6		PINSEL[6]	//!< PINSEL6--->P3[15:0]  NOT USED
#define		PINSEL7		PINSEL[7]	//!< PINSEL7--->P3[31:16](0x4002C01C)
#define		PINSEL8		PINSEL[8]	//!< PINSEL8--->P4[15:0]  NOT USED
#define		PINSEL9		PINSEL[9]	//!< PINSEL9--->P4[31:16](0x4002C024)
#define		PINSEL10	PINSEL[10]	//!< PINSEL10-->P2[6:2]  (0x4002C028) (Trace function)


//!< //////////////////Registros PINMODE ///////////////////////////
//!< 0x4002C040UL : Direccion de inicio de los registros de modo de los pines del GPIO
#define		PINMODE	( ( __RW uint32_t  * ) 0x4002C040UL )

#define		PINMODE0	PINMODE[0]		//!< 0x4002C040
#define		PINMODE1	PINMODE[1]		//!< 0x4002C044
#define		PINMODE2	PINMODE[2]		//!< 0x4002C048
#define		PINMODE3	PINMODE[3]		//!< 0x4002C04C
#define		PINMODE4	PINMODE[4]		//!< 0x4002C050
#define		PINMODE5	PINMODE[5]		//!< 0x4002C054
#define		PINMODE6	PINMODE[6]		//!< 0x4002C058
#define		PINMODE7	PINMODE[7]		//!< 0x4002C05C
#define		PINMODE9	PINMODE[9]		//!< 0x4002C064


//!< ///////////////// REGISTROS PINMODE_OD ///////////////////////////
//!< 0x4002C068UL : Direccion de inicio de los registros de control del modo OPEN DRAIN
#define		PINMODE_OD		( ( __RW uint32_t  * ) 0x4002C068UL )

#define		PINMODE_OD0		PINMODE_OD[0]
#define		PINMODE_OD1		PINMODE_OD[1]
#define		PINMODE_OD2		PINMODE_OD[2]
#define		PINMODE_OD3		PINMODE_OD[3]
#define		PINMODE_OD4		PINMODE_OD[4]

// 0x4002C07CUL : I2C Pin Configuration register :
#define		I2CPADCFG		( * ( ( __RW uint32_t  * ) 0x4002C07CUL ) )


//!< ////////////////// REGISTROS GPIOs //////////////////////////////
//!< 0x2009C000UL : Direccion de inicio de los registros de GPIOs
#define	GPIOs		( ( __RW uint32_t  * ) 0x2009C000UL )

#define GPIO_BASE		0x2009C000UL
#define FIODIR_BASE		0x2009C000UL
#define FIOMASK_BASE	0x2009C010UL
#define FIOPIN_BASE		0x2009C014UL
#define FIOSET_BASE		0x2009C018UL
#define FIOCLR_BASE		0x2009C01CUL

/*	*						*
	*************************
	*		FIODIR			*	0x2009C000
	*************************
	*		RESERVED		*	0x2009C004
	*************************
	*		RESERVED		*	0x2009C008
	*************************
	*		RESERVED		*	0x2009C00C
	*************************
	*		FIOMASK			*	0x2009C010
	*************************
	*		FIOPIN			*	0x2009C014
	*************************
	*		FIOSET			*	0x2009C018
	*************************
	*		FIOCLR			*	0x2009C01C
	*************************
	*						*
	*						*
*/

// estructura de GPIOs
typedef struct
{
	__RW uint32_t DIR;
	__R  uint32_t reserved[3];
	__RW uint32_t MASK;
	__RW uint32_t PIN;
	__RW uint32_t SET;
	__RW uint32_t CLR;
} GPIO_t;

#define		FIO		( ( GPIO_t * ) GPIO_BASE )

#define		FIO0	( ( GPIO_t * ) 0x2009C000UL )
#define		FIO1	( ( GPIO_t * ) 0x2009C020UL )
#define		FIO2	( ( GPIO_t * ) 0x2009C040UL )
#define		FIO3	( ( GPIO_t * ) 0x2009C060UL )
#define		FIO4	( ( GPIO_t * ) 0x2009C080UL )




#define 	FIODIR		( ( __RW uint32_t * ) FIODIR_BASE  )
#define		FIO0DIR		FIO[0]->DIR		//!< 0x2009C000
#define		FIO1DIR		FIO[1]->DIR		//!< 0x2009C020
#define		FIO2DIR		FIO[2]->DIR		//!< 0x2009C040
#define		FIO3DIR		FIO[3]->DIR		//!< 0x2009C060
#define		FIO4DIR		FIO[4]->DIR		//!< 0x2009C080

#define 	FIOMASK		( ( __RW uint32_t * ) FIOMASK_BASE )
#define		FIO0MASK	FIO[0]->MASK	//!< 0x2009C010
#define		FIO1MASK	FIO[1]->MASK	//!< 0x2009C030
#define		FIO2MASK	FIO[2]->MASK	//!< 0x2009C050
#define		FIO3MASK	FIO[3]->MASK	//!< 0x2009C070
#define		FIO4MASK	FIO[4]->MASK	//!< 0x2009C090

#define 	FIOPIN		( ( __RW uint32_t * ) FIOPIN_BASE  )
#define		FIO0PIN		FIO[0]->PIN		//!< 0x2009C014
#define		FIO1PIN		FIO[1]->PIN		//!< 0x2009C034
#define		FIO2PIN		FIO[2]->PIN		//!< 0x2009C054
#define		FIO3PIN		FIO[3]->PIN		//!< 0x2009C074
#define		FIO4PIN		FIO[4]->PIN		//!< 0x2009C094

#define 	FIOSET		( ( __RW uint32_t * ) FIOSET_BASE  )
#define		FIO0SET		FIO[0]->SET		//!< 0x2009C018
#define		FIO1SET		FIO[1]->SET		//!< 0x2009C038
#define		FIO2SET		FIO[2]->SET		//!< 0x2009C058
#define		FIO3SET		FIO[3]->SET		//!< 0x2009C078
#define		FIO4SET		FIO[4]->SET		//!< 0x2009C098

#define 	FIOCLR		( ( __RW uint32_t * ) FIOCLR_BASE  )
#define		FIO0CLR		FIO[0]->CLR		//!< 0x2009C01C
#define		FIO1CLR		FIO[1]->CLR		//!< 0x2009C03C
#define		FIO2CLR		FIO[2]->CLR		//!< 0x2009C05C
#define		FIO3CLR		FIO[3]->CLR		//!< 0x2009C07C
#define		FIO4CLR		FIO[4]->CLR		//!< 0x2009C09C

//-----------------------------------------------------------------------------
// NVIC
//-----------------------------------------------------------------------------

#define		ISER 		( ( __RW uint32_t  * ) 0xE000E100UL )
#define 	ISER0		ISER[0]
#define 	ISER1		ISER[1]

#define		ICER 		( ( __RW uint32_t  * ) 0xE000E180UL )
#define		ICER0		ICER[0]
#define		ICER1		ICER[1]

#define		ISPR 		( ( __RW uint32_t  * ) 0xE000E200UL )
#define		ISPR0		ISPR[0]
#define		ISPR1		ISPR[1]

#define		ICPR 		( ( __RW uint32_t  * ) 0xE000E280UL )
#define		ICPR0		ICPR[0]
#define		ICPR1		ICPR[1]

#define		IABR 		( ( __RW uint32_t  * ) 0xE000E300UL )
#define		IABR0		IABR[0]
#define		IABR1		IABR[1]

#define		IPR 		( ( __RW uint32_t  * ) 0xE000E400UL )
#define		IPR0		IPR[0]
#define		IPR1		IPR[1]
#define		IPR2		IPR[2]
#define		IPR3		IPR[3]
#define		IPR4		IPR[4]
#define		IPR5		IPR[5]
#define		IPR6		IPR[6]
#define		IPR7		IPR[7]
#define		IPR8		IPR[8]

#define		STIR_ 		( ( __RW uint32_t  * ) 0xE000EF00UL )
#define		STIR 		STIR_[0]


//	Bits relativos a ISER0, ICER0, ISPR0, ICPR0, IABR0
//	los registros IPR y STIR son aparte
#define		NVIC_WDT		(1UL << 0)
#define		NVIC_TIMER0		(1UL << 1)
#define		NVIC_TIMER1		(1UL << 2)
#define		NVIC_TIMER2		(1UL << 3)
#define		NVIC_TIMER3		(1UL << 4)
#define		NVIC_UART0		(1UL << 5)
#define		NVIC_UART1		(1UL << 6)
#define		NVIC_UART2		(1UL << 7)
#define		NVIC_UART3		(1UL << 8)
#define		NVIC_PWM		(1UL << 9)
#define		NVIC_I2C0		(1UL << 10)
#define		NVIC_I2C1		(1UL << 11)
#define		NVIC_I2C2		(1UL << 12)
#define		NVIC_SPI		(1UL << 13)
#define		NVIC_SSP0		(1UL << 14)
#define		NVIC_SSP1		(1UL << 15)
#define		NVIC_PLL0		(1UL << 16)
#define		NVIC_RTC		(1UL << 17)
#define		NVIC_EINT0		(1UL << 18)
#define		NVIC_EINT1		(1UL << 19)
#define		NVIC_EINT2		(1UL << 20)
#define		NVIC_EINT3		(1UL << 21)
#define		NVIC_ADC		(1UL << 22)
#define		NVIC_BOD		(1UL << 23)
#define		NVIC_USB		(1UL << 24)
#define		NVIC_CAN		(1UL << 25)
#define		NVIC_DMA		(1UL << 26)
#define		NVIC_I2S		(1UL << 27)
#define		NVIC_ENET		(1UL << 28)
#define		NVIC_RIT		(1UL << 29)
#define		NVIC_MCPWM		(1UL << 30)
#define		NVIC_QEI		(1UL << 31)
#define		NVIC_PLL1		(1UL << 32)
#define		NVIC_USBACT		(1UL << 33)
#define		NVIC_CANACT		(1UL << 34)


typedef enum {
	/* -------------------------  Cortex-M3 Processor Exceptions Numbers  ----------------------------- */
	Reset_IRQn                    = -15,		/*!< 1 Reset Vector, invoked on Power up and warm reset */
	NonMaskableInt_IRQn           = -14,		/*!< 2 Non maskable Interrupt, cannot be stopped or preempted */
	HardFault_IRQn                = -13,		/*!< 3 Hard Fault, all classes of Fault */
	MemoryManagement_IRQn         = -12,		/*!< 4 Memory Management, MPU mismatch, including Access Violation and No Match */
	BusFault_IRQn                 = -11,		/*!< 5 Bus Fault, Pre-Fetch-, Memory Access Fault, other address/memory related Fault */
	UsageFault_IRQn               = -10,		/*!< 6 Usage Fault, i.e. Undef Instruction, Illegal State Transition  */
	SVCall_IRQn                   = -5,			/*!< 11 System Service Call via SVC instruction   */
	DebugMonitor_IRQn             = -4,			/*!< 12 CDebug Monitor   */
	PendSV_IRQn                   = -2,			/*!< 14 Pendable request for system service */
	SysTick_IRQn                  = -1,			/*!< 15 System Tick Interrupt */

	/* ---------------------------  LPC17xx Specific Interrupt Numbers  ------------------------------- */
	WDT_IRQn                      = 0,			/*!< Watchdog Timer Interrupt                         */
	TIMER0_IRQn                   = 1,			/*!< Timer0 Interrupt                                 */
	TIMER1_IRQn                   = 2,			/*!< Timer1 Interrupt                                 */
	TIMER2_IRQn                   = 3,			/*!< Timer2 Interrupt                                 */
	TIMER3_IRQn                   = 4,			/*!< Timer3 Interrupt                                 */
	UART0_IRQn                    = 5,			/*!< UART0 Interrupt                                  */
	UART_IRQn                     = UART0_IRQn,	/*!< Alias for UART0 Interrupt                        */
	UART1_IRQn                    = 6,			/*!< UART1 Interrupt                                  */
	UART2_IRQn                    = 7,			/*!< UART2 Interrupt                                  */
	UART3_IRQn                    = 8,			/*!< UART3 Interrupt                                  */
	PWM1_IRQn                     = 9,			/*!< PWM1 Interrupt                                   */
	I2C0_IRQn                     = 10,			/*!< I2C0 Interrupt                                   */
	I2C_IRQn                      = I2C0_IRQn,	/*!< Alias for I2C0 Interrupt                         */
	I2C1_IRQn                     = 11,			/*!< I2C1 Interrupt                                   */
	I2C2_IRQn                     = 12,			/*!< I2C2 Interrupt                                   */
	SPI_IRQn                      = 13,			/*!< SPI Interrupt                                    */
	SSP0_IRQn                     = 14,			/*!< SSP0 Interrupt                                   */
	SSP_IRQn                      = SSP0_IRQn,	/*!< Alias for SSP0 Interrupt                         */
	SSP1_IRQn                     = 15,			/*!< SSP1 Interrupt                                   */
	PLL0_IRQn                     = 16,			/*!< PLL0 Lock (Main PLL) Interrupt                   */
	RTC_IRQn                      = 17,			/*!< Real Time Clock and event recorder Interrupt     */
	EINT0_IRQn                    = 18,			/*!< External Interrupt 0 Interrupt                   */
	EINT1_IRQn                    = 19,			/*!< External Interrupt 1 Interrupt                   */
	EINT2_IRQn                    = 20,			/*!< External Interrupt 2 Interrupt                   */
	EINT3_IRQn                    = 21,			/*!< External Interrupt 3 Interrupt                   */
	ADC_IRQn                      = 22,			/*!< A/D Converter Interrupt                          */
	BOD_IRQn                      = 23,			/*!< Brown-Out Detect Interrupt                       */
	USB_IRQn                      = 24,			/*!< USB Interrupt                                    */
	CAN_IRQn                      = 25,			/*!< CAN Interrupt                                    */
	DMA_IRQn                      = 26,			/*!< General Purpose DMA Interrupt                    */
	I2S_IRQn                      = 27,			/*!< I2S Interrupt                                    */
	ETHERNET_IRQn                 = 28,			/*!< Ethernet Interrupt                               */
	RITIMER_IRQn                  = 29,			/*!< Repetitive Interrupt Interrupt                   */
	MCPWM_IRQn                    = 30,			/*!< Motor Control PWM Interrupt                      */
	QEI_IRQn                      = 31,			/*!< Quadrature Encoder Interface Interrupt           */
	PLL1_IRQn                     = 32,			/*!< PLL1 Lock (USB PLL) Interrupt                    */
	USBActivity_IRQn              = 33,			/*!< USB Activity interrupt                           */
	CANActivity_IRQn              = 34,			/*!< CAN Activity interrupt                           */
} LPC175X_6X_IRQn_Type;

typedef LPC175X_6X_IRQn_Type IRQn_Type;

//-----------------------------------------------------------------------------
// Interrupciones Externas
//-----------------------------------------------------------------------------
#define		EXTINT_ 	( ( __RW uint32_t  * ) 0x400FC140UL )
#define		EXTINT		EXTINT_[0]

#define		EXTMODE_ 	( ( __RW uint32_t  * ) 0x400FC148UL )
#define		EXTMODE		EXTMODE_[0]

#define		EXTPOLAR_ 	( ( __RW uint32_t  * ) 0x400FC14CUL )
#define 	EXTPOLAR	EXTPOLAR_[0]

#define 	EINT0  	0
#define 	EINT1  	1
#define 	EINT2  	2
#define 	EINT3  	3


//-----------------------------------------------------------------------------
// Interrupciones Externas GPIO
//-----------------------------------------------------------------------------
#define		IOIntStatus_ 	( ( __RW uint32_t  * ) 0x40028080UL )
#define		IOIntStatus		IOIntStatus_[0]

#define		IOInt_P0		0x01
#define		IOInt_P2		0x04

// P0
#define		IO0IntStatR_ 	( ( __RW uint32_t  * ) 0x40028084UL )
#define		IO0IntStatR		IO0IntStatR_[0]

#define		IO0IntStatF_ 	( ( __RW uint32_t  * ) 0x40028088UL )
#define		IO0IntStatF		IO0IntStatF_[0]

#define		IO0IntClr_ 		( ( __RW uint32_t  * ) 0x4002808CUL )
#define		IO0IntClr		IO0IntClr_[0]

#define		IO0IntEnR_ 		( ( __RW uint32_t  * ) 0x40028090UL )
#define		IO0IntEnR		IO0IntEnR_[0]

#define		IO0IntEnF_ 		( ( __RW uint32_t  * ) 0x40028094UL )
#define		IO0IntEnF		IO0IntEnF_[0]

// P2
#define		IO2IntStatR_ 	( ( __RW uint32_t  * ) 0x400280A4UL )
#define		IO2IntStatR		IO2IntStatR_[0]

#define		IO2IntStatF_ 	( ( __RW uint32_t  * ) 0x400280A8UL )
#define		IO2IntStatF		IO2IntStatF_[0]

#define		IO2IntClr_ 		( ( __RW uint32_t  * ) 0x400280ACUL )
#define		IO2IntClr		IO2IntClr_[0]

#define		IO2IntEnR_ 		( ( __RW uint32_t  * ) 0x400280B0UL )
#define		IO2IntEnR		IO2IntEnR_[0]

#define		IO2IntEnF_ 		( ( __RW uint32_t  * ) 0x400280B4UL )
#define		IO2IntEnF		IO2IntEnF_[0]


//!< ///////////////////   PCON   //////////////////////////
//!<  Power Control Register. (PCON - 0x400F C0C0)
#define		PCON		( * ( ( __RW uint32_t * ) 0x400FC0C0UL ) )

// system control block - system control register
#define		SCB_SCR		( * ( ( __RW uint32_t * ) 0xE000ED10UL ) )

//!< ///////////////////   PCONP   //////////////////////////
//!<  Power Control for Peripherals register (PCONP - 0x400F C0C4) [pag. 62 user manual LPC1769]
//!< 0x400FC0C4UL : Direccion de inicio del registro de habilitación de dispositivos:
#define		PCONP 		( * ( ( __RW uint32_t * ) 0x400FC0C4UL ) )

//!< ///////////////////   PCLKSEL   //////////////////////////
//!< Peripheral Clock Selection registers 0 and 1 (PCLKSEL0 -0x400F C1A8 and PCLKSEL1 - 0x400F C1AC) [pag. 56 user manual]
//!< 0x400FC1A8UL : Direccion de inicio de los registros de seleccion de los CLKs de los dispositivos:
#define		DIR_PCLKSEL		0x400FC1A8UL
//!< Registros PCLKSEL
#define		PCLKSEL				( ( __RW uint32_t  * ) DIR_PCLKSEL )
#define		PCLKSEL0			( ( __RW uint32_t  * ) DIR_PCLKSEL )[0]
#define		PCLKSEL1			( ( __RW uint32_t  * ) DIR_PCLKSEL )[1]



//!< /////////////		SYSTICK		///////////////////////////
//!< Tipo de dato específico para manejar el SYSTICK
typedef struct {
	union {
		__RW uint32_t CTRL;
		struct {
			__RW unsigned ENABLE:1;
			__RW unsigned TICKINT:1;
			__RW unsigned CLKSOURCE:1;
			__R  unsigned RESERVED0:13;
			__RW unsigned COUNTFLAG:1;
			__R  unsigned RESERVED1:15;
		} ;
	} ;
	__RW uint32_t RELOAD;
	__RW uint32_t CURR;
	__RW uint32_t CALIB;
} SYSTICK_T;

//!< 0xE000E010UL: Registro de control del SysTick:
#define 	SysTick		( (SYSTICK_T *) 0xE000E010UL )

#define		SYSTICK_COUNTFLAG	16


//!< /////////////		TIMERs		///////////////////////////
typedef struct
{
	__RW uint32_t IR;			/** IR   - INTERRUPT REGISTER        */
	__RW uint32_t TCR;			/** TCR  - TIMER CONTROL REGISTER    */
	__RW uint32_t TC;			/** TC   - TIMER COUNTER REGISTER    */
	__RW uint32_t PR;			/** PR   - PRESCALE REGISTER         */
	__RW uint32_t PC;			/** PC   - PRESCALE COUNTER REGISTER */
	__RW uint32_t MCR;			/** MCR  - MATCH CONTROL REGISTER    */
	__RW uint32_t MR0;			/** MR   - MATCH REGISTERS           */
	__RW uint32_t MR1;
	__RW uint32_t MR2;
	__RW uint32_t MR3;
	__RW uint32_t CCR;			/** CCR  - CAPTURE CONTROL REGISTER  */
	__R  uint32_t CR0;			/** CR   - CAPTURE REGISTERS         */
	__R  uint32_t CR1;
	__R  uint32_t dummy1[2];
	__RW uint32_t EMR;			/** EMR  - EXTERNAL MATCH REGISTER   */
	__R  uint32_t dummy2[12];
	__RW uint32_t CTCR;			/** CTCR - COUNT CONTROL REGISTER    */
} TIMER_t;

// Direcciones iniciales de cada Timer
#define		T0				( ( TIMER_t *) 0x40004000UL )
#define		T1				( ( TIMER_t *) 0x40008000UL )
#define		T2				( ( TIMER_t *) 0x40090000UL )
#define		T3				( ( TIMER_t *) 0x40094000UL )

//!< /////////////		FIN TIMERs		///////////////////////////



//!< /////////////		UARTs		///////////////////////////
//0x40010000UL : Registro de recepcion de la UART0:
#define		UART0_DIR		( ( __RW uint32_t  * ) 0x4000C000UL )

#define		U0RBR		UART0_DIR[0]	// DLAB = 0
#define		U0THR		UART0_DIR[0]	// DLAB = 0
#define		U0DLL		UART0_DIR[0]	// DLAB = 1
#define		U0DLM		UART0_DIR[1]	// DLAB = 1
#define		U0IER		UART0_DIR[1]	// DLAB = 0
#define		U0IIR		UART0_DIR[2]
#define		U0FCR		UART0_DIR[2]
#define		U0LCR		UART0_DIR[3]
#define		U0LSR		UART0_DIR[5]
#define		U0SCR		UART0_DIR[7]
#define		U0ACR		UART0_DIR[8]
#define		U0ICR		UART0_DIR[9]
#define		U0FDR		UART0_DIR[10]
#define		U0TER		UART0_DIR[12]

//0x40010000UL : Registro de recepcion de la UART1:
#define		UART1_DIR		( ( __RW uint32_t  * ) 0x40010000UL )

#define		U1RBR		UART1_DIR[0]
#define		U1THR		UART1_DIR[0]
#define		U1DLL		UART1_DIR[0]
#define		U1IER		UART1_DIR[1]
#define		U1DLM		UART1_DIR[1]
#define		U1IIR		UART1_DIR[2]
#define		U1LCR		UART1_DIR[3]
#define		U1LSR		UART1_DIR[5]

//!< /////////////		FIN UARTs		///////////////////////////



//!< /////////////		I2C		///////////////////////////

typedef struct {				/* I2C0 Structure         */
	__RW uint32_t CONSET;		/*!< I2C Control Set Register. When a one is written to a bit of this register, the corresponding bit in the I2C control register is set. Writing a zero has no effect on the corresponding bit in the I2C control register. */
	__R  uint32_t STAT;			/*!< I2C Status Register. During I2C operation, this register provides detailed status codes that allow software to determine the next action needed. */
	__RW uint32_t DAT;			/*!< I2C Data Register. During master or slave transmit mode, data to be transmitted is written to this register. During master or slave receive mode, data that has been received may be read from this register. */
	__RW uint32_t ADR0;			/*!< I2C Slave Address Register 0. Contains the 7-bit slave address for operation of the I2C interface in slave mode, and is not used in master mode. The least significant bit determines whether a slave responds to the General Call address. */
	__RW uint32_t SCLH;			/*!< SCH Duty Cycle Register High Half Word. Determines the high time of the I2C clock. */
	__RW uint32_t SCLL;			/*!< SCL Duty Cycle Register Low Half Word. Determines the low time of the I2C clock. SCLL and SCLH together determine the clock frequency generated by an I2C master and certain times used in slave mode. */
	__W  uint32_t CONCLR;		/*!< I2C Control Clear Register. When a one is written to a bit of this register, the corresponding bit in the I2C control register is cleared. Writing a zero has no effect on the corresponding bit in the I2C control register. */
	__RW uint32_t MMCTRL;		/*!< Monitor mode control register. */
	__RW uint32_t ADR1;			/*!< I2C Slave Address Register. Contains the 7-bit slave address for operation of the I2C interface in slave mode, and is not used in master mode. The least significant bit determines whether a slave responds to the General Call address. */
	__RW uint32_t ADR2;			/*!< I2C Slave Address Register. Contains the 7-bit slave address for operation of the I2C interface in slave mode, and is not used in master mode. The least significant bit determines whether a slave responds to the General Call address. */
	__RW uint32_t ADR3;			/*!< I2C Slave Address Register. Contains the 7-bit slave address for operation of the I2C interface in slave mode, and is not used in master mode. The least significant bit determines whether a slave responds to the General Call address. */
	__R  uint32_t DATA_BUFFER;	/*!< Data buffer register. The contents of the 8 MSBs of the DAT shift register will be transferred to the DATA_BUFFER automatically after every nine bits (8 bits of data plus ACK or NACK) has been received on the bus. */
	__RW uint32_t MASK[4];		/*!< I2C Slave address mask register */
} LPC_I2C_T;

// I2C Pin Configuration registers :
#define LPC_I2C0_BASE             0x4001C000UL
#define LPC_I2C1_BASE             0x4005C000UL
#define LPC_I2C2_BASE             0x400A0000UL

#define LPC_I2C0		( ( LPC_I2C_T * ) LPC_I2C0_BASE )
#define LPC_I2C1		( ( LPC_I2C_T * ) LPC_I2C1_BASE )
#define LPC_I2C2		( ( LPC_I2C_T * ) LPC_I2C2_BASE )

//!< /////////////		FIN I2C		///////////////////////////



//!< ////////////		RTC		///////////////////

typedef struct {
	__RW uint32_t ILR;
	__R uint32_t dummy;
	__RW uint32_t CCR;
	__RW uint32_t CIIR;
	__RW uint32_t AMR;

	__R  uint32_t CTIME0;
	__R  uint32_t CTIME1;
	__R  uint32_t CTIME2;

	__RW uint32_t SEC;
	__RW uint32_t MIN;
	__RW uint32_t HOUR;
	__RW uint32_t DOM;
	__RW uint32_t DOW;
	__RW uint32_t DOY;
	__RW uint32_t MONTH;
	__RW uint32_t YEAR;

	__RW uint32_t CALIBRATION;

	__RW uint32_t GPREG0;
	__RW uint32_t GPREG1;
	__RW uint32_t GPREG2;
	__RW uint32_t GPREG3;
	__RW uint32_t GPREG4;

	__RW uint32_t RTC_AUXEN;
	__RW uint32_t RTC_AUX;
	__RW uint32_t ALSEC;
	__RW uint32_t ALMIN;
	__RW uint32_t ALHOUR;
	__RW uint32_t ALDOM;
	__RW uint32_t ALDOW;
	__RW uint32_t ALDOY;
	__RW uint32_t ALMON;
	__RW uint32_t ALYEAR;
} LPC_RTC_t;

#define LPC_RTC_BASE	0x40024000UL
#define	LPC_RTC			( (LPC_RTC_t *) LPC_RTC_BASE )

//!< //////////////// END RTC ///////////////////


//!< ////////////////  USB  /////////////////////

typedef struct {
	__R  uint32_t DevIntSt;		// 0x5000 C200
	__RW uint32_t DevIntEn;		// 0x5000 C204
	__W  uint32_t DevIntClr;	// 0x5000 C208
	__W  uint32_t DevIntSet;	// 0x5000 C20C

	__W  uint32_t CmdCode;		// 0x5000 C210
	__R  uint32_t CmdData;		// 0x5000 C214

	__R  uint32_t RxData;		// 0x5000 C218
	__W  uint32_t TxData;		// 0x5000 C21C
	__R  uint32_t RxPLen;		// 0x5000 C220
	__W  uint32_t TxPLen;		// 0x5000 C224
	__RW uint32_t Ctrl;			// 0x5000 C228

	__W  uint32_t DevIntPri;	// 0x5000 C22C

	__R  uint32_t EpIntSt;		// 0x5000 C230
	__RW uint32_t EpIntEn;		// 0x5000 C234
	__W  uint32_t EpIntClr;		// 0x5000 C238
	__W  uint32_t EpIntSet;		// 0x5000 C23C
	__W  uint32_t EpIntPri;		// 0x5000 C240

	__RW uint32_t ReEp;			// 0x5000 C244
	__W  uint32_t EpIn;			// 0x5000 C248
	__RW uint32_t MaxPSize;		// 0x5000 C24C

	__R  uint32_t DMARSt;		// 0x5000 C250
	__W  uint32_t DMARClr;		// 0x5000 C254
	__W  uint32_t DMARSet;		// 0x5000 C258

	__R  uint32_t dummy1[9];	// C25C ~ C27C

	__RW uint32_t UDCAH;		// 0x5000 C280
	__R  uint32_t EpDMASt;		// 0x5000 C284
	__W  uint32_t EpDMAEn;		// 0x5000 C288
	__W  uint32_t EpDMADis;		// 0x5000 C28C

	__R  uint32_t DMAIntSt;		// 0x5000 C290
	__RW uint32_t DMAIntEn;		// 0x5000 C294

	__R  uint32_t dummy2[2];	// C298 y C29C

	__R  uint32_t EoTIntSt;		// 0x5000 C2A0
	__W  uint32_t EOTIntClr;	// 0x5000 C2A4
	__W  uint32_t EoTIntSet;	// 0x5000 C2A8
	__R  uint32_t NDDRIntSt;	// 0x5000 C2AC
	__W  uint32_t NDDRIntClr;	// 0x5000 C2B0
	__W  uint32_t NDDRIntSet;	// 0x5000 C2B4
	__R  uint32_t SysErrIntSt;	// 0x5000 C2B8
	__W  uint32_t SysErrIntClr;	// 0x5000 C2BC
	__W  uint32_t SysErrIntSet;	// 0x5000 C2C0

} USB_t;

#define		LPC_USB_BASE	0x5000C200UL
#define		USB				( (USB_t *) LPC_USB_BASE )

#define		USBIntSt_		( (__RW uint32_t*) 0x400FC1C0UL )
#define		USBIntSt		USBIntSt_[0]

#define		USBClkCtrl_		( (__RW uint32_t*) 0x5000CFF4UL )
#define		USBClkCtrl		USBClkCtrl_[0]

#define		USBClkSt_		( (__R  uint32_t*) 0x5000CFF8UL )
#define		USBClkSt		USBClkSt_[0]

//!< ////////////////  END USB  /////////////////////


//!< ////////////////  ADC  /////////////////////

typedef struct
{
	__RW uint32_t ADCR;

	__RW uint32_t ADGDR;

	__R  uint32_t dummy;

	__RW uint32_t ADINTEN;

	__R  uint32_t ADDR0;
	__R  uint32_t ADDR1;
	__R  uint32_t ADDR2;
	__R  uint32_t ADDR3;
	__R  uint32_t ADDR4;
	__R  uint32_t ADDR5;
	__R  uint32_t ADDR6;
	__R  uint32_t ADDR7;

	__R  uint32_t ADSTAT;

	__RW uint32_t ADTRM;
} LPC_ADC_t;

#define		ADC			( ( LPC_ADC_t * ) 0x40034000UL )

//!< ////////////////  END ADC  /////////////////////


//!< ////////////////  RITimer  /////////////////////

typedef struct {				/*!< RITIMER Structure      */
	__RW uint32_t  COMPVAL;		/*!< Compare register       */
	__RW uint32_t  MASK;		/*!< Mask register. This register holds the 32-bit mask value. A 1 written to any bit will force a compare on the corresponding bit of the counter and compare register. */
	__RW uint32_t  CTRL;		/*!< Control register.      */
	__RW uint32_t  COUNTER;		/*!< 32-bit counter         */
} LPC_RITIMER_T;

#define LPC_RITIMER_BASE 0x400B0000
#define LPC_RITIMER      ((LPC_RITIMER_T *) LPC_RITIMER_BASE)

/**	Set by H/W when the counter value equals the masked compare value */
#define RIT_CTRL_INT    (1UL << 0)
/** Set timer enable clear to 0 when the counter value equals the masked compare value  */
#define RIT_CTRL_ENCLR  (1UL << 1)
/** Set timer enable on debug */
#define RIT_CTRL_ENBR   (1UL << 2)
/** Set timer enable */
#define RIT_CTRL_TEN    (1UL << 3)

//!< ////////////////  END RITimer  /////////////////////

#endif /* REGS_H_ */
