#ifndef TECLADO_H_
#define TECLADO_H_

#include <RegsLPC1769.h>

#define		T_DEBOUNCE		20
#define		ACEPTAReSTADO	((int) (0.7 * T_DEBOUNCE))	// arbitrario

// configuro si quiero usar el SW5, el cual no se puede manejar por interrupcion
#define _5_ENTRADAS 0
#if _5_ENTRADAS
	#define		N_ENTRADAS		5		// SW1, SW2, SW3, SW4 y SW5
#else
	#define		N_ENTRADAS		4		// SW1, SW2, SW3 y SW4
#endif /* _5_ENTRADAS */

void Debounce_Teclado(void);
uint8_t readSW(uint8_t);
void enableReleaseKeyInt(uint8_t);

void SetLEDActual	( void );
void SetLED			( int8_t );
void ledUP			( void );
void ledDOWN		( void );
void pressedKey		( uint8_t );
void releasedKey	( uint8_t );
void ledON			( void );
void ledOFF			( void );
void ToggleLEDActual( void );
void ToggleLED		( void );


#endif /* TECLADO_H_ */
