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

typedef enum {FALSE = 0, TRUE = !FALSE} Bool;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, IntStatus, SetState;
typedef enum {ERROR = 0, SUCCESS = !ERROR} Status;


/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
#define _BIT(n) (1 << (n))

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

/* NULL pointer */
#ifndef NULL
#define NULL ((void *) 0)
#endif

/* Number of elements in an array */
#define NELEMENTS(array)  (sizeof(array) / sizeof(array[0]))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))



typedef struct {
	__RW uint32_t PIN0:2;
	__RW uint32_t PIN1:2;
	__RW uint32_t PIN2:2;
	__RW uint32_t PIN3:2;
	__RW uint32_t PIN4:2;
	__RW uint32_t PIN5:2;
	__RW uint32_t PIN6:2;
	__RW uint32_t PIN7:2;
	__RW uint32_t PIN8:2;
	__RW uint32_t PIN9:2;
	__RW uint32_t PIN10:2;
	__RW uint32_t PIN11:2;
	__RW uint32_t PIN12:2;
	__RW uint32_t PIN13:2;
	__RW uint32_t PIN14:2;
	__RW uint32_t PIN15:2;
	__RW uint32_t PIN16:2;
	__RW uint32_t PIN17:2;
	__RW uint32_t PIN18:2;
	__RW uint32_t PIN19:2;
	__RW uint32_t PIN20:2;
	__RW uint32_t PIN21:2;
	__RW uint32_t PIN22:2;
	__RW uint32_t PIN23:2;
	__RW uint32_t PIN24:2;
	__RW uint32_t PIN25:2;
	__RW uint32_t PIN26:2;
	__RW uint32_t PIN27:2;
	__RW uint32_t PIN28:2;
	__RW uint32_t PIN29:2;
	__RW uint32_t PIN30:2;
	__RW uint32_t PIN31:2;
}PINs_2B_t;
typedef struct {
	PINs_2B_t P0;
	PINs_2B_t P1;
	PINs_2B_t P2;
	PINs_2B_t P3;
	PINs_2B_t P4;
}PORTs_2B_t;


//!< ////////////////Registros PINSEL//////////////////////////////
//!< 0x4002C000UL : Direccion de inicio de los registros PINSEL
#define		PINSEL		( (    PORTs_2B_t  * ) 0x4002C000UL )
#define		PINSEL_		( ( __RW uint32_t  * ) 0x4002C000UL )

#define		PINSEL0		PINSEL_[0]	//!< PINSEL0--->P0[15:0] (0x4002C000)
#define		PINSEL1		PINSEL_[1]	//!< PINSEL1--->P0[31:16](0x4002C004)
#define		PINSEL2		PINSEL_[2]	//!< PINSEL2--->P1[15:0] (0x4002C008)
#define		PINSEL3		PINSEL_[3]	//!< PINSEL3--->P1[31:16](0x4002C00C)
#define		PINSEL4		PINSEL_[4]	//!< PINSEL4--->P2[15:0] (0x4002C010)
#define		PINSEL5		PINSEL_[5]	//!< PINSEL5--->P2[31:16] NOT USED
#define		PINSEL6		PINSEL_[6]	//!< PINSEL6--->P3[15:0] NOT USED
#define		PINSEL7		PINSEL_[7]	//!< PINSEL7--->P3[31:16] 0x4002C01C)
#define		PINSEL8		PINSEL_[8]	//!< PINSEL8--->P4[15:0]  NOT USED
#define		PINSEL9		PINSEL_[9]	//!< PINSEL9--->P4[31:16](0x4002C024)

//!< ----------- Estados de PINSEL:
#define		PINSEL_GPIO			0
#define		PINSEL_FUNC1		1
#define		PINSEL_FUNC2		2
#define		PINSEL_FUNC3		3

//!< //////////////////Registros PINMODE ///////////////////////////
//!< 0x4002C040UL : Direccion de inicio de los registros de modo de los pines del GPIO
#define		PINMODE		( (    PORTs_2B_t  * ) 0x4002C040UL )
#define		PINMODE_	( ( __RW uint32_t  * ) 0x4002C040UL )

#define		PINMODE0	PINMODE_[0]		//!< 0x4002C040
#define		PINMODE1	PINMODE_[1]		//!< 0x4002C044
#define		PINMODE2	PINMODE_[2]		//!< 0x4002C048
#define		PINMODE3	PINMODE_[3]		//!< 0x4002C04C
#define		PINMODE4	PINMODE_[4]		//!< 0x4002C050
#define		PINMODE5	PINMODE_[5]		//!< 0x4002C054
#define		PINMODE6	PINMODE_[6]		//!< 0x4002C058
#define		PINMODE7	PINMODE_[7]		//!< 0x4002C05C
#define		PINMODE9	PINMODE_[9]		//!< 0x4002C064



//!< ----------- Estados de PINMODE
//!< 00	Pull Up resistor enable (reset value)		01	repeated mode enable
//!< 11	Pull Down resistor enable					10	ni Pull Up ni Pull DOwn
#define		PINMODE_PULLUP 		0
#define		PINMODE_REPEAT 		1
#define		PINMODE_NONE 		2
#define		PINMODE_PULLDOWN 	3

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


typedef struct{
	union {
		__RW uint32_t DIR;
		struct {
			__RW uint32_t DIR0:1;
			__RW uint32_t DIR1:1;
			__RW uint32_t DIR2:1;
			__RW uint32_t DIR3:1;
			__RW uint32_t DIR4:1;
			__RW uint32_t DIR5:1;
			__RW uint32_t DIR6:1;
			__RW uint32_t DIR7:1;
			__RW uint32_t DIR8:1;
			__RW uint32_t DIR9:1;
			__RW uint32_t DIR10:1;
			__RW uint32_t DIR11:1;
			__RW uint32_t DIR12:1;
			__RW uint32_t DIR13:1;
			__RW uint32_t DIR14:1;
			__RW uint32_t DIR15:1;
			__RW uint32_t DIR16:1;
			__RW uint32_t DIR17:1;
			__RW uint32_t DIR18:1;
			__RW uint32_t DIR19:1;
			__RW uint32_t DIR20:1;
			__RW uint32_t DIR21:1;
			__RW uint32_t DIR22:1;
			__RW uint32_t DIR23:1;
			__RW uint32_t DIR24:1;
			__RW uint32_t DIR25:1;
			__RW uint32_t DIR26:1;
			__RW uint32_t DIR27:1;
			__RW uint32_t DIR28:1;
			__RW uint32_t DIR29:1;
			__RW uint32_t DIR30:1;
			__RW uint32_t DIR31:1;
		};
	};

	__R uint32_t reserved[3];

	union {
		__RW uint32_t MASK;
		struct {
			__RW uint32_t MASK0:1;
			__RW uint32_t MASK1:1;
			__RW uint32_t MASK2:1;
			__RW uint32_t MASK3:1;
			__RW uint32_t MASK4:1;
			__RW uint32_t MASK5:1;
			__RW uint32_t MASK6:1;
			__RW uint32_t MASK7:1;
			__RW uint32_t MASK8:1;
			__RW uint32_t MASK9:1;
			__RW uint32_t MASK10:1;
			__RW uint32_t MASK11:1;
			__RW uint32_t MASK12:1;
			__RW uint32_t MASK13:1;
			__RW uint32_t MASK14:1;
			__RW uint32_t MASK15:1;
			__RW uint32_t MASK16:1;
			__RW uint32_t MASK17:1;
			__RW uint32_t MASK18:1;
			__RW uint32_t MASK19:1;
			__RW uint32_t MASK20:1;
			__RW uint32_t MASK21:1;
			__RW uint32_t MASK22:1;
			__RW uint32_t MASK23:1;
			__RW uint32_t MASK24:1;
			__RW uint32_t MASK25:1;
			__RW uint32_t MASK26:1;
			__RW uint32_t MASK27:1;
			__RW uint32_t MASK28:1;
			__RW uint32_t MASK29:1;
			__RW uint32_t MASK30:1;
			__RW uint32_t MASK31:1;
		};
	};

	union {
		__RW uint32_t PIN;
		struct {
			__RW uint32_t PIN0:1;
			__RW uint32_t PIN1:1;
			__RW uint32_t PIN2:1;
			__RW uint32_t PIN3:1;
			__RW uint32_t PIN4:1;
			__RW uint32_t PIN5:1;
			__RW uint32_t PIN6:1;
			__RW uint32_t PIN7:1;
			__RW uint32_t PIN8:1;
			__RW uint32_t PIN9:1;
			__RW uint32_t PIN10:1;
			__RW uint32_t PIN11:1;
			__RW uint32_t PIN12:1;
			__RW uint32_t PIN13:1;
			__RW uint32_t PIN14:1;
			__RW uint32_t PIN15:1;
			__RW uint32_t PIN16:1;
			__RW uint32_t PIN17:1;
			__RW uint32_t PIN18:1;
			__RW uint32_t PIN19:1;
			__RW uint32_t PIN20:1;
			__RW uint32_t PIN21:1;
			__RW uint32_t PIN22:1;
			__RW uint32_t PIN23:1;
			__RW uint32_t PIN24:1;
			__RW uint32_t PIN25:1;
			__RW uint32_t PIN26:1;
			__RW uint32_t PIN27:1;
			__RW uint32_t PIN28:1;
			__RW uint32_t PIN29:1;
			__RW uint32_t PIN30:1;
			__RW uint32_t PIN31:1;
		};
	};

	union {
		__RW uint32_t SET;
		struct {
			__RW uint32_t SET0:1;
			__RW uint32_t SET1:1;
			__RW uint32_t SET2:1;
			__RW uint32_t SET3:1;
			__RW uint32_t SET4:1;
			__RW uint32_t SET5:1;
			__RW uint32_t SET6:1;
			__RW uint32_t SET7:1;
			__RW uint32_t SET8:1;
			__RW uint32_t SET9:1;
			__RW uint32_t SET10:1;
			__RW uint32_t SET11:1;
			__RW uint32_t SET12:1;
			__RW uint32_t SET13:1;
			__RW uint32_t SET14:1;
			__RW uint32_t SET15:1;
			__RW uint32_t SET16:1;
			__RW uint32_t SET17:1;
			__RW uint32_t SET18:1;
			__RW uint32_t SET19:1;
			__RW uint32_t SET20:1;
			__RW uint32_t SET21:1;
			__RW uint32_t SET22:1;
			__RW uint32_t SET23:1;
			__RW uint32_t SET24:1;
			__RW uint32_t SET25:1;
			__RW uint32_t SET26:1;
			__RW uint32_t SET27:1;
			__RW uint32_t SET28:1;
			__RW uint32_t SET29:1;
			__RW uint32_t SET30:1;
			__RW uint32_t SET31:1;
		};
	};

	union {
		__RW uint32_t CLR;
		struct {
			__RW uint32_t CLR0:1;
			__RW uint32_t CLR1:1;
			__RW uint32_t CLR2:1;
			__RW uint32_t CLR3:1;
			__RW uint32_t CLR4:1;
			__RW uint32_t CLR5:1;
			__RW uint32_t CLR6:1;
			__RW uint32_t CLR7:1;
			__RW uint32_t CLR8:1;
			__RW uint32_t CLR9:1;
			__RW uint32_t CLR10:1;
			__RW uint32_t CLR11:1;
			__RW uint32_t CLR12:1;
			__RW uint32_t CLR13:1;
			__RW uint32_t CLR14:1;
			__RW uint32_t CLR15:1;
			__RW uint32_t CLR16:1;
			__RW uint32_t CLR17:1;
			__RW uint32_t CLR18:1;
			__RW uint32_t CLR19:1;
			__RW uint32_t CLR20:1;
			__RW uint32_t CLR21:1;
			__RW uint32_t CLR22:1;
			__RW uint32_t CLR23:1;
			__RW uint32_t CLR24:1;
			__RW uint32_t CLR25:1;
			__RW uint32_t CLR26:1;
			__RW uint32_t CLR27:1;
			__RW uint32_t CLR28:1;
			__RW uint32_t CLR29:1;
			__RW uint32_t CLR30:1;
			__RW uint32_t CLR31:1;
		};
	};
}FIO_t ;

#define		FIO0	( ( FIO_t * ) 0x2009C000UL )
#define		FIO1	( ( FIO_t * ) 0x2009C020UL )
#define		FIO2	( ( FIO_t * ) 0x2009C040UL )
#define		FIO3	( ( FIO_t * ) 0x2009C060UL )
#define		FIO4	( ( FIO_t * ) 0x2009C080UL )


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
#define 	PConP			( ( PCONP_t  * ) 0x400FC0C4UL )
#define		PCONP 		( * ( ( __RW uint32_t * ) 0x400FC0C4UL ) )


typedef struct {
	__R  uint32_t reserved1:1;
	__RW uint32_t TIM0:1;
	__RW uint32_t TIM1:1;
	__RW uint32_t UART0:1;
	__RW uint32_t UART1:1;
	__R  uint32_t reserved2:1;
	__RW uint32_t PWM1:1;
	__RW uint32_t I2C0:1;
	__RW uint32_t SPI:1;
	__RW uint32_t RTC:1;
	__RW uint32_t SSP1:1;
	__R  uint32_t reserved3:1;
	__RW uint32_t ADC:1;
	__RW uint32_t CAN1:1;
	__RW uint32_t CAN2:1;
	__RW uint32_t GPIO:1;
	__RW uint32_t RIT:1;
	__RW uint32_t MCPWM:1;
	__RW uint32_t QEI:1;
	__RW uint32_t I2C1:1;
	__R  uint32_t reserved4:1;
	__RW uint32_t SSP0:1;
	__RW uint32_t TIM2:1;
	__RW uint32_t TIM3:1;
	__RW uint32_t UART2:1;
	__RW uint32_t UART3:1;
	__RW uint32_t I2C2:1;
	__RW uint32_t I2S:1;
	__R  uint32_t reserved5:1;
	__RW uint32_t GPDMA:1;
	__RW uint32_t ENET:1;
	__RW uint32_t USB:1;
}PCONP_t;


//!< ///////////////////   PCLKSEL   //////////////////////////
//!< Peripheral Clock Selection registers 0 and 1 (PCLKSEL0 -0x400F C1A8 and PCLKSEL1 - 0x400F C1AC) [pag. 56 user manual]
//!< 0x400FC1A8UL : Direccion de inicio de los registros de seleccion de los CLKs de los dispositivos:
#define		PCLKSEL			( (     PCLKSEL_t  * ) 0x400FC1A8UL )
#define		PCLKSEL_		( ( __RW uint32_t  * ) 0x400FC1A8UL )
//!< Registros PCLKSEL
#define		PCLKSEL0		PCLKSEL_[0]
#define		PCLKSEL1		PCLKSEL_[1]

typedef struct {
		__RW uint32_t WDT:2;
		__RW uint32_t TIMER0:2;
		__RW uint32_t TIMER1:2;
		__RW uint32_t UART0:2;
		__RW uint32_t UART1:2;
		__R  uint32_t reserved1:2;
		__RW uint32_t PWM1:2;
		__RW uint32_t I2C0:2;
		__RW uint32_t SPI:2;
		__R  uint32_t reserved2:2;
		__RW uint32_t SSP1:2;
		__RW uint32_t DAC:2;
		__RW uint32_t ADC:2;
		__RW uint32_t CAN1:2;
		__RW uint32_t ACF:2;
		__RW uint32_t QEI:2;
		__RW uint32_t GPIOINT:2;
		__RW uint32_t PCB:2;
		__RW uint32_t I2C1:2;
		__RW uint32_t SSP0:2;
		__RW uint32_t TIMER2:2;
		__RW uint32_t TIMER3:2;
		__RW uint32_t UART2:2;
		__RW uint32_t UART3:2;
		__RW uint32_t I2C2:2;
		__RW uint32_t I2S:2;
		__R  uint32_t reserved3:2;
		__RW uint32_t RIT:2;
		__RW uint32_t SYSCON:2;
		__RW uint32_t MC:2;
} PCLKSEL_t;


//!< /////////////		SYSTICK		///////////////////////////
//!< Tipo de dato específico para manejar el SYSTICK
typedef struct
{
	union{
		__RW uint32_t _STCTRL;
		struct{
			__RW uint32_t _ENABLE:1;
			__RW uint32_t _TICKINT:1;
			__RW uint32_t _CLKSOURCE:1;
			__R  uint32_t _RESERVED0:13;
			__RW uint32_t _COUNTFLAG:1;
			__R  uint32_t _RESERVED1:15;
		}bits;
	};
	__RW uint32_t _STRELOAD;
	__RW uint32_t _STCURR;
	__R uint32_t  _STCALIB;
}systick_t;

//!< 0xE000E010UL: Registro de control del SysTick:
#define 	DIR_SYSTICK		( (systick_t *) 0xE000E010UL )

#define		STCTRL		DIR_SYSTICK->_STCTRL
	#define	ENABLE			DIR_SYSTICK->bits._ENABLE
	#define	TICKINT			DIR_SYSTICK->bits._TICKINT
	#define	CLKSOURCE		DIR_SYSTICK->bits._CLKSOURCE
	#define	COUNTFLAG		DIR_SYSTICK->bits._COUNTFLAG
#define		STRELOAD	DIR_SYSTICK->_STRELOAD
#define		STCURR		DIR_SYSTICK->_STCURR
#define		STCALIB		DIR_SYSTICK->_STCALIB

#define 	N 		1	//Si N=1, Systick interrumpe cada 10ms.


//!< /////////////		TIMERs		///////////////////////////
typedef struct
{
	union {							/** IR - INTERRUPT REGISTER */
		__RW uint32_t IR;
		struct{
			__RW uint32_t MR0:1;
			__RW uint32_t MR1:1;
			__RW uint32_t MR2:1;
			__RW uint32_t MR3:1;
			__RW uint32_t CR0:1;
			__RW uint32_t CR1:1;
			__R  uint32_t reserved:26;
		} IR_;
	};
	union{							/** TCR - TIMER CONTROL REGISTER */
		__RW uint32_t TCR;
		struct{
			__RW uint32_t CE:1;
			__RW uint32_t CR:1;
			__R uint32_t reserved:30;
		} TCR_;
	};
	__RW uint32_t TC;				/** TC - TIMER COUNTER REGISTER */
	__RW uint32_t PR;				/** PR - PRESCALE REGISTER */
	__RW uint32_t PC;				/** PC - PRESCALE COUNTER REGISTER */
	union{							/** MCR - MATCH CONTROL REGISTER */
		uint32_t MCR;
		struct{
			__RW uint32_t MR0I:1;	/** interrupt enable/disable */
			__RW uint32_t MR0R:1;	/** reset timer on match */
			__RW uint32_t MR0S:1;	/** stop timer on match */
			__RW uint32_t MR1I:1;
			__RW uint32_t MR1R:1;
			__RW uint32_t MR1S:1;
			__RW uint32_t MR2I:1;
			__RW uint32_t MR2R:1;
			__RW uint32_t MR2S:1;
			__RW uint32_t MR3I:1;
			__RW uint32_t MR3R:1;
			__RW uint32_t MR3S:1;
			__R  uint32_t reserved:20;
		} MCR_;
	};
	__RW uint32_t MR0;				/** MR - MATCH REGISTERS */
	__RW uint32_t MR1;
	__RW uint32_t MR2;
	__RW uint32_t MR3;
	union{							/** CCR - CAPTURE CONTROL REGISTER */
		__RW uint32_t CCR;
		struct{
			__RW uint32_t CAP0RE:1;
			__RW uint32_t CAP0FE:1;
			__RW uint32_t CAP0I:1;
			__RW uint32_t CAP1RE:1;
			__RW uint32_t CAP1FE:1;
			__RW uint32_t CAP1I:1;
			__R  uint32_t reserved:26;
		} CCR_;
	};
		__RW uint32_t CR0;				/** CR - CAPTURE REGISTERS */
	__RW uint32_t CR1;
	__R uint32_t dummy1[2];
	union{							/** EMR - EXTERNAL MATCH REGISTER */
		__RW uint32_t EMR;
		struct{
			__RW uint32_t EM0:1;
			__RW uint32_t EM1:1;
			__RW uint32_t EM2:1;
			__RW uint32_t EM3:1;
			__RW uint32_t EMC0:2;
			__RW uint32_t EMC1:2;
			__RW uint32_t EMC2:2;
			__RW uint32_t EMC3:2;
			__R  uint32_t reserved:20;
		} EMR_;
	};
		__R uint32_t dummy2[12];
	union{							/** CTCR - COUNT CONTROL REGISTER */
		uint32_t CTCR;
		struct{
			__RW uint32_t TimerCounterMode:2;
			__RW uint32_t CountInputSelect:2;
			__R  uint32_t reserved:28;
		} CTCR_;
	};

} TIMER_t;

// Direcciones iniciales de cada Timer
#define		T0				( ( TIMER_t *) 0x40004000UL )
#define		T1				( ( TIMER_t *) 0x40008000UL )
#define		T2				( ( TIMER_t *) 0x40090000UL )
#define		T3				( ( TIMER_t *) 0x40094000UL )
//!< /////////////		FIN TIMERs		///////////////////////////



//!< /////////////		UARTs		///////////////////////////
//0x40010000UL : Registro de recepcion de la UART0:
#define		DIR_UART0		( ( __RW uint32_t  * ) 0x4000C000UL )

#define		U0RBR		DIR_UART0[0]
#define		U0THR		DIR_UART0[0]
#define		U0DLL		DIR_UART0[0]
#define		U0IER		DIR_UART0[1]
#define		U0DLM		DIR_UART0[1]
#define		U0IIR		DIR_UART0[2]
#define		U0LCR		DIR_UART0[3]
#define		U0LSR		DIR_UART0[5]

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


//!< ////////////		RTC		///////////////////

typedef struct {
		union {
		__RW uint32_t ILR;
		struct {
			__RW uint32_t CIF:1;
			__RW uint32_t ALF:1;
			__R  uint32_t reserved:30;
		} ILR_;
	};
		__R uint32_t dummy;
		union {
		__RW uint32_t CCR;
		struct {
			__RW uint32_t CLKEN:1;
			__RW uint32_t CTCRST:1;
			__RW uint32_t internal_test:2;
			__RW uint32_t CCALEN:1;
			__R  uint32_t reserved:27;
		} CCR_;
	};
	union {
			__RW uint32_t CIIR;
		struct {
			__RW uint32_t IMSEC:1;
			__RW uint32_t IMMIN:1;
			__RW uint32_t IMHOUR:1;
			__RW uint32_t IMDOM:1;
			__RW uint32_t IMDOW:1;
			__RW uint32_t IMDOY:1;
			__RW uint32_t IMMONTH:1;
			__RW uint32_t IMYEAR:1;
			__R  uint32_t reserved:24;
		} CIIR_;
	};
	union {
			__RW uint32_t AMR;
		struct {
			__RW uint32_t AMRSEC:1;
			__RW uint32_t AMRMIN:1;
			__RW uint32_t AMRHOUR:1;
			__RW uint32_t AMRDOM:1;
			__RW uint32_t AMRDOW:1;
			__RW uint32_t AMRDOY:1;
			__RW uint32_t AMRMONTH:1;
			__RW uint32_t AMRYEAR:1;
			__R  uint32_t reserved:24;
		} AMR_;
	};
	union {
		struct{
		__RW uint32_t CTIME0;
		__RW uint32_t CTIME1;
		__RW uint32_t CTIME2;
		};
		struct{
			__R uint32_t Seconds:6;
			__R uint32_t reserved1:2;
			__R uint32_t Minutes:6;
			__R uint32_t reserved2:2;
			__R uint32_t Hours:5;
			__R uint32_t reserved3:3;
			__R uint32_t DayOfWeek:3;
			__R uint32_t reserved4:5;

			__R uint32_t DayOfMonth:5;
			__R uint32_t reserved5:3;
			__R uint32_t Month:4;
			__R uint32_t reserved6:4;
			__R uint32_t Year:12;
			__R uint32_t reserved7:4;

			__R uint32_t DayOfYear:12;
			__R uint32_t reserved8:20;
		} CTIME_;
	};

	__RW uint32_t SEC;
	__RW uint32_t MIN;
	__RW uint32_t HOUR;
	__RW uint32_t DOM;
	__RW uint32_t DOW;
	__RW uint32_t DOY;
	__RW uint32_t MONTH;
	__RW uint32_t YEAR;

	__RW uint32_t CALIBRATION; // faltan los campos de esto

	__RW uint32_t GPREG0;
	__RW uint32_t GPREG1;
	__RW uint32_t GPREG2;
	__RW uint32_t GPREG3;
	__RW uint32_t GPREG4;

	union {
		__RW uint32_t RTC_AUXEN;
		struct{
			__R  uint32_t reserved1:4;
			__RW uint32_t OSCFEN:1;
			__R  uint32_t reserved2:27;
		}RTC_AUXEN_;
	};

	union {
		__RW uint32_t RTC_AUX;
		struct{
			__R  uint32_t reserved1:4;
			__RW uint32_t OSCF:1;
			__R  uint32_t reserved2:27;
		}RTC_AUX_;
	};

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



#endif /* REGS_H_ */
