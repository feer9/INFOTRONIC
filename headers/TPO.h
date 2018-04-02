#ifndef TPO_H_
#define TPO_H_

#include <FW_Entradas.h>
#include <FW_GPIO.h>
#include <KitInfo2_BaseBoard.h>
#include <RegsLPC1769.h>
#include <Timer.h>

void InicializarKit( void );
void Teclado( void );

void ToggleLED(uint8_t);
void SetLED(uint8_t);
void ledUP   (void);
void ledDOWN (void);
void pressedKey(uint8_t);
void releasedKey(uint8_t);


void setearTimers(void);


#endif /* TPO_H_ */
