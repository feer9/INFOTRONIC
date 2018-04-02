#ifndef FW_ENTRADAS_H_
#define FW_ENTRADAS_H_

#include <RegsLPC1769.h>

#define		T_DEBOUNCE		20
#define		ACEPTAReSTADO	16		// arbitrario

// configuro si quiero usar el SW5, el cual no se puede manejar por interrupcion
#define _5_ENTRADAS 1
#if _5_ENTRADAS
	#define		N_ENTRADAS		5		// SW1, SW2, SW3, SW4 y SW5
#else
	#define		N_ENTRADAS		4		// SW1, SW2, SW3 y SW4
#endif /* _5_ENTRADAS */

void Debounce_Teclado(void);
uint8_t readSW(uint8_t);

#endif /* FW_ENTRADAS_H_ */
