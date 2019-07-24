#ifndef DRIVERS_ADC_H_
#define DRIVERS_ADC_H_

#include "../Aplicacion/Aplicacion.h"
#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"
#include "clock.h"


// Control Register setting bits
#define		CR_SEL		0
#define		CR_CLKDIV	8
#define		CR_BURST	16
#define		CR_PDN		21
#define		CR_START	24
#define		CR_EDGE		27

typedef struct {
	uint32_t AD5_val;
	__RW bool change;
	__RW int8_t timerId;
} ADC_t;

void ADC_init(void);
void ADC_start();
void ADC_stop();

#endif /* DRIVERS_ADC_H_ */
