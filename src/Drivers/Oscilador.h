 /**
 * Oscilador.h
 *
 *  Created on: 17/04/2012 -   Author: Mica y Chor
 */

#ifndef _OSCILADOR_H
#define _OSCILADOR_H

	//////////////Registros del CLOCK y de sistema/////////////////
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

	//Prototipos de función:


#endif /* _OSCILADOR_H */
