#ifndef REGSLPC1769_H_
#define REGSLPC1769_H_


#define		__R		volatile const
#define		__W		volatile
#define		__RW	volatile

typedef 	signed int		int32_t;
typedef 	signed short	int16_t;
typedef 	signed char		int8_t;
typedef 	unsigned int 	uint32_t;
typedef 	unsigned short 	uint16_t;
typedef 	unsigned char 	uint8_t;

typedef enum {FALSE = 0, TRUE = !FALSE} bool;
typedef enum {RESET = 0, SET = !RESET} flagStatus, intStatus, setState;
typedef enum {SUCCESS = 0, ERROR = !SUCCESS} status;


/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
#define _BIT(n) (1UL << (n))

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */
#define _SBF(f, v) ((v) << (f))

/* _BITMASK constructs a symbol with 'field_width' least significant
 * bits set.
 * e.g., _BITMASK(5) constructs '0x1F', _BITMASK(16) == 0xFFFF
 * The symbol is intended to be used to limit the bit field width
 * thusly:
 * <a_register> = (any_expression) & _BITMASK(x), where 0 < x <= 32.
 * If "any_expression" results in a value that is larger than can be
 * contained in 'x' bits, the bits above 'x - 1' are masked off.  When
 * used with the _SBF example above, the example would be written:
 * a_reg = ((_SBF(5,7) | _SBF(12,0xF)) & _BITMASK(16))
 * This ensures that the value written to a_reg is no wider than
 * 16 bits, and makes the code easier to read and understand.
 */
#define _BITMASK(field_width) ( _BIT(field_width) - 1)

#define _RESET_BIT(n) &= ~ _BIT(n)
#define _SET_BIT(n)   |=   _BIT(n)
#define clearBIT(n)   &= ~ _BIT(n)
#define setBIT(n)     |=   _BIT(n)


/* Number of elements in an array */
#define NELEMENTS(array)  (sizeof(array) / sizeof(array[0]))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))


//!< ////////////////Registros PINSEL//////////////////////////////
//!< 0x4002C000UL : Direccion de inicio de los registros PINSEL
#define		PINSEL		( ( __RW uint32_t  * ) 0x4002C000UL )

#define		PINSEL0		PINSEL[0]	//!< PINSEL0--->P0[15:0] (0x4002C000)
#define		PINSEL1		PINSEL[1]	//!< PINSEL1--->P0[31:16](0x4002C004)
#define		PINSEL2		PINSEL[2]	//!< PINSEL2--->P1[15:0] (0x4002C008)
#define		PINSEL3		PINSEL[3]	//!< PINSEL3--->P1[31:16](0x4002C00C)
#define		PINSEL4		PINSEL[4]	//!< PINSEL4--->P2[15:0] (0x4002C010)
#define		PINSEL5		PINSEL[5]	//!< PINSEL5--->P2[31:16] NOT USED
#define		PINSEL6		PINSEL[6]	//!< PINSEL6--->P3[15:0] NOT USED
#define		PINSEL7		PINSEL[7]	//!< PINSEL7--->P3[31:16] 0x4002C01C)
#define		PINSEL8		PINSEL[8]	//!< PINSEL8--->P4[15:0]  NOT USED
#define		PINSEL9		PINSEL[9]	//!< PINSEL9--->P4[31:16](0x4002C024)

//!< ----------- Estados de PINSEL:
#define		PINSEL_GPIO			0
#define		PINSEL_FUNC1		1
#define		PINSEL_FUNC2		2
#define		PINSEL_FUNC3		3

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



//!< ----------- Estados de PINMODE
//!< 00	Pull Up resistor enable (reset value)		01	repeated mode enable
//!< 11	Pull Down resistor enable					10	ni Pull Up ni Pull DOwn
#define		PINMODE_PULLUP 		0x00
#define		PINMODE_REPEAT 		0x01
#define		PINMODE_NONE 		0x02
#define		PINMODE_PULLDOWN 	0x03

//!< ///////////////// REGISTROS PINMODE_OD ///////////////////////////
//!< 0x4002C068UL : Direccion de inicio de los registros de control del modo OPEN DRAIN
#define		PINMODE_OD		( ( __RW uint32_t  * ) 0x4002C068UL )

#define		PINMODE_OD0		PINMODE_OD[0]
#define		PINMODE_OD1		PINMODE_OD[1]
#define		PINMODE_OD2		PINMODE_OD[2]
#define		PINMODE_OD3		PINMODE_OD[3]
#define		PINMODE_OD4		PINMODE_OD[4]

//!< ////////////////// REGISTROS GPIOs //////////////////////////////
//!< 0x2009C000UL : Direccion de inicio de los registros de GPIOs
#define	GPIOs		( ( __RW uint32_t  * ) 0x2009C000UL )

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

#define 	FIODIR		( ( __RW uint32_t * ) 0x2009C000UL )
#define		FIO0DIR		GPIOs[0]	//!< 0x2009C000
#define		FIO1DIR		GPIOs[8]	//!< 0x2009C020
#define		FIO2DIR		GPIOs[16]	//!< 0x2009C040
#define		FIO3DIR		GPIOs[24]	//!< 0x2009C060
#define		FIO4DIR		GPIOs[32]	//!< 0x2009C080

#define 	FIOMASK		( ( __RW uint32_t * ) 0x2009C010UL )
#define		FIO0MASK	GPIOs[4]	//!< 0x2009C010
#define		FIO1MASK	GPIOs[12]	//!< 0x2009C030
#define		FIO2MASK	GPIOs[20]	//!< 0x2009C050
#define		FIO3MASK	GPIOs[28]	//!< 0x2009C070
#define		FIO4MASK	GPIOs[36]	//!< 0x2009C090

#define 	FIOPIN		( ( __RW uint32_t * ) 0x2009C014UL )
#define		FIO0PIN		GPIOs[5]	//!< 0x2009C014
#define		FIO1PIN		GPIOs[13]	//!< 0x2009C034
#define		FIO2PIN		GPIOs[21]	//!< 0x2009C054
#define		FIO3PIN		GPIOs[29]	//!< 0x2009C074
#define		FIO4PIN		GPIOs[37]	//!< 0x2009C094

#define 	FIOSET		( ( __RW uint32_t * ) 0x2009C018UL )
#define		FIO0SET		GPIOs[6]	//!< 0x2009C018
#define		FIO1SET		GPIOs[14]	//!< 0x2009C038
#define		FIO2SET		GPIOs[22]	//!< 0x2009C058
#define		FIO3SET		GPIOs[30]	//!< 0x2009C078
#define		FIO4SET		GPIOs[38]	//!< 0x2009C098

#define 	FIOCLR		( ( __RW uint32_t * ) 0x2009C01CUL )
#define		FIO0CLR		GPIOs[7]	//!< 0x2009C01C
#define		FIO1CLR		GPIOs[15]	//!< 0x2009C03C
#define		FIO2CLR		GPIOs[23]	//!< 0x2009C05C
#define		FIO3CLR		GPIOs[31]	//!< 0x2009C07C
#define		FIO4CLR		GPIOs[39]	//!< 0x2009C09C

// dirección en FIODIR
#define 	ENTRADA			0
#define 	SALIDA			1


typedef struct
{
	__RW uint32_t DIR;
	__R uint32_t reserved[3];
	__RW uint32_t MASK;
	__RW uint32_t PIN;
	__RW uint32_t SET;
	__RW uint32_t CLR;
} GPIO_t;

#define		FIO		( ( GPIO_t * ) 0x2009C000UL )

#define		FIO0	( ( GPIO_t * ) 0x2009C000UL )
#define		FIO1	( ( GPIO_t * ) 0x2009C020UL )
#define		FIO2	( ( GPIO_t * ) 0x2009C040UL )
#define		FIO3	( ( GPIO_t * ) 0x2009C060UL )
#define		FIO4	( ( GPIO_t * ) 0x2009C080UL )


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
#define		NVIC_WDT		0
#define		NVIC_TIMER0		1
#define		NVIC_TIMER1		2
#define		NVIC_TIMER2		3
#define		NVIC_TIMER3		4
#define		NVIC_UART0		5
#define		NVIC_UART1		6
#define		NVIC_UART2		7
#define		NVIC_UART3		8
#define		NVIC_PWM		9
#define		NVIC_I2C0		10
#define		NVIC_I2C1		11
#define		NVIC_I2C2		12
#define		NVIC_SPI		13
#define		NVIC_SSP0		14
#define		NVIC_SSP1		15
#define		NVIC_PLL0		16
#define		NVIC_RTC		17
#define		NVIC_EINT0		18
#define		NVIC_EINT1		19
#define		NVIC_EINT2		20
#define		NVIC_EINT3		21
#define		NVIC_ADC		22
#define		NVIC_BOD		23
#define		NVIC_USB		24
#define		NVIC_CAN		25
#define		NVIC_DMA		26
#define		NVIC_I2S		27
#define		NVIC_ENET		28
#define		NVIC_RIT		29
#define		NVIC_MCPWM		30
#define		NVIC_QEI		31
#define		NVIC_PLL1		32
#define		NVIC_USBACT		33
#define		NVIC_CANACT		34


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


//!< ///////////////////   PCONP   //////////////////////////
//!<  Power Control for Peripherals register (PCONP - 0x400F C0C4) [pag. 62 user manual LPC1769]
//!< 0x400FC0C4UL : Direccion de inicio del registro de habilitación de dispositivos:
#define		PCONP 		( * ( ( __RW uint32_t * ) 0x400FC0C4UL ) )


#define 	PCONP_TIMER0	1
#define 	PCONP_TIMER1	2
#define 	PCONP_UART0		3
#define 	PCONP_UART1		4

#define 	PCONP_PWM1		6
#define 	PCONP_I2C0		7
#define 	PCONP_SPI		8
#define 	PCONP_RTC		9
#define 	PCONP_SSP1		10

#define 	PCONP_ADC		12
#define 	PCONP_CAN1		13
#define 	PCONP_CAN2		14
#define 	PCONP_GPIO		15
#define 	PCONP_RIT		16
#define 	PCONP_MCPWM		17
#define 	PCONP_QEI		18
#define 	PCONP_I2C1		19

#define 	PCONP_SSP0		21
#define 	PCONP_TIM2		22
#define 	PCONP_TIM3		23
#define 	PCONP_UART2		24
#define 	PCONP_UART3		25
#define 	PCONP_I2C2		26
#define 	PCONP_I2S		27

#define 	PCONP_GPDMA		29
#define 	PCONP_ENET		30
#define 	PCONP_USB		31


//!< ///////////////////   PCLKSEL   //////////////////////////
//!< Peripheral Clock Selection registers 0 and 1 (PCLKSEL0 -0x400F C1A8 and PCLKSEL1 - 0x400F C1AC) [pag. 56 user manual]
//!< 0x400FC1A8UL : Direccion de inicio de los registros de seleccion de los CLKs de los dispositivos:
#define		PCLKSEL		( ( __RW uint32_t  * ) 0x400FC1A8UL )
//!< Registros PCLKSEL
#define		PCLKSEL0		PCLKSEL[0]
#define		PCLKSEL1		PCLKSEL[1]


#define		PCLK_CCLK_4		0UL
#define		PCLK_CCLK		1UL
#define		PCLK_CCLK_2		2UL
#define		PCLK_CCLK_8		3UL


#define		PCLKSEL_WDT		0
#define		PCLKSEL_TIMER0	2
#define		PCLKSEL_TIMER1	4
#define		PCLKSEL_UART0	6
#define		PCLKSEL_UART1	8

#define		PCLKSEL_PWM1	12
#define		PCLKSEL_I2C0	14
#define		PCLKSEL_SPI		16

#define		PCLKSEL_SSP1	20
#define		PCLKSEL_DAC		22
#define		PCLKSEL_ADC		24
#define		PCLKSEL_CAN1	26
#define		PCLKSEL_CAN2	28
#define		PCLKSEL_ACF		30
#define		PCLKSEL_QEI		32
#define		PCLKSEL_GPIOINT	34
#define		PCLKSEL_PCB		36
#define		PCLKSEL_I2C1	38

#define		PCLKSEL_SSP0	42
#define		PCLKSEL_TIMER2	44
#define		PCLKSEL_TIMER3	46
#define		PCLKSEL_UART2	48
#define		PCLKSEL_UART3	50
#define		PCLKSEL_I2C2	52
#define		PCLKSEL_I2S		54

#define		PCLKSEL_RIT		58
#define		PCLKSEL_SYSCON	60
#define		PCLKSEL_MC		62


//!< /////////////		SYSTICK		///////////////////////////
//!< Tipo de dato específico para manejar el SYSTICK
typedef struct {
	union {
		__RW uint32_t STCTRL;
		struct {
			__RW uint32_t ENABLE:1;
			__RW uint32_t TICKINT:1;
			__RW uint32_t CLKSOURCE:1;
			__R  uint32_t RESERVED0:13;
			__RW uint32_t COUNTFLAG:1;
			__R  uint32_t RESERVED1:15;
		} ;
	} ;
	__RW uint32_t STRELOAD;
	__RW uint32_t STCURR;
	__R uint32_t  STCALIB;
} systick_t;

//!< 0xE000E010UL: Registro de control del SysTick:
#define 	SYSTICK		( (systick_t *) 0xE000E010UL )

#define		SYSTICK_COUNTFLAG	16
/*
#define		STCTRL		DIR_SYSTICK->_STCTRL
	#define	ENABLE			DIR_SYSTICK->_ENABLE
	#define	TICKINT			DIR_SYSTICK->_TICKINT
	#define	CLKSOURCE		DIR_SYSTICK->_CLKSOURCE
	#define	COUNTFLAG		DIR_SYSTICK->_COUNTFLAG
#define		STRELOAD	DIR_SYSTICK->_STRELOAD
#define		STCURR		DIR_SYSTICK->_STCURR
#define		STCALIB		DIR_SYSTICK->_STCALIB
*/

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
	__RW uint32_t CR0;			/** CR   - CAPTURE REGISTERS         */
	__RW uint32_t CR1;
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

#define		MR0I	0
#define		MR0R	1
#define		MR0S	2
#define		MR1I	3
#define		MR1R	4
#define		MR1S	5
#define		MR2I	6
#define		MR2R	7
#define		MR2S	8
#define		MR3I	9
#define		MR3R	10
#define		MR3S	11

//!< /////////////		FIN TIMERs		///////////////////////////



//!< /////////////		UARTs		///////////////////////////
//0x40010000UL : Registro de recepcion de la UART0:
#define		DIR_UART0		( ( __RW uint32_t  * ) 0x4000C000UL )

#define		U0RBR		DIR_UART0[0]	// DLAB = 0
#define		U0THR		DIR_UART0[0]	// DLAB = 0
#define		U0DLL		DIR_UART0[0]	// DLAB = 1
#define		U0DLM		DIR_UART0[1]	// DLAB = 1
#define		U0IER		DIR_UART0[1]	// DLAB = 0
#define		U0IIR		DIR_UART0[2]
#define		U0FCR		DIR_UART0[2]
#define		U0LCR		DIR_UART0[3]
#define		U0LSR		DIR_UART0[5]
#define		U0SCR		DIR_UART0[7]
#define		U0ACR		DIR_UART0[8]
#define		U0ICR		DIR_UART0[9]
#define		U0FDR		DIR_UART0[10]
#define		U0TER		DIR_UART0[12]

//0x40010000UL : Registro de recepcion de la UART1:
#define		DIR_UART1		( ( __RW uint32_t  * ) 0x40010000UL )

#define		U1RBR		DIR_UART1[0]
#define		U1THR		DIR_UART1[0]
#define		U1DLL		DIR_UART1[0]
#define		U1IER		DIR_UART1[1]
#define		U1DLM		DIR_UART1[1]
#define		U1IIR		DIR_UART1[2]
#define		U1LCR		DIR_UART1[3]
#define		U1LSR		DIR_UART1[5]


#define		IIR_RLS		0x03
#define		IIR_RDA		0x02
#define		IIR_CTI		0x06
#define		IIR_THRE	0x01


// RX TRIGGER LEVEL

// 1  character  or 0x01
#define		TRIGGER_LEVEL_0		0UL
// 4  characters or 0x04
#define		TRIGGER_LEVEL_1		1UL
// 8  characters or 0x08
#define		TRIGGER_LEVEL_2		2UL
// 14 characters or 0x0E
#define		TRIGGER_LEVEL_3		3UL

// trigger level bit position
#define		U0FCR_TRIGGER_LEVEL		6

//!< /////////////		FIN UARTs		///////////////////////////

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
} RTC_t;

#define	LPC_RTC		( (RTC_t *) 0x40024000UL )

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

#define		USB				( (USB_t *) 0x5000C200UL )

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

// Control Register setting bits
#define		CR_SEL		0
#define		CR_CLKDIV	8
#define		CR_BURST	16
#define		CR_PDN		21
#define		CR_START	24
#define		CR_EDGE		27


//!< ////////////////  END ADC  /////////////////////

#endif /* REGS_H_ */
