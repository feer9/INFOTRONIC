#ifndef TECLADO_H_
#define TECLADO_H_

#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"

#define		T_DEBOUNCE		20
#define		ACEPTAReSTADO	((int) (0.5 * T_DEBOUNCE))	// arbitrario

// configuro si quiero usar el SW5, el cual no se puede manejar por interrupcion
#define _5_TECLAS 0
#if _5_TECLAS
	#define		N_TECLAS		5		// SW1, SW2, SW3, SW4 y SW5
#else
	#define		N_TECLAS		4		// SW1, SW2, SW3 y SW4
#endif /* _5_TECLAS */


#define enableDebounce( n, flag )	((flag) |= (0x1 << ((n)-1)))
//#define acceptKeyChange(n, ant  )	(((ant>>n)& 0x1) ? pressedKey(n+1) : releasedKey(n+1) )

void debounceTeclado(void);


/*
void SetLEDActual	( void );
void SetLED			( int8_t );
void ledUP			( void );
void ledDOWN		( void );
void ledON			( void );
void ledOFF			( void );
void ToggleLEDActual( void );
void ToggleLED		( void );
*/
void SW1_handler(bool st);
void SW2_handler(bool st);
void SW3_handler(bool st);
void SW4_handler(bool st);
void defaultKeyHandler(bool st);

#endif /* TECLADO_H_ */
