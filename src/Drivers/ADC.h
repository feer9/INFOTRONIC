#ifndef DRIVERS_ADC_H_
#define DRIVERS_ADC_H_

#include "../Aplicacion/Aplicacion.h"
#include "regsLPC1769.h"
#include "KitInfo2_BaseBoard.h"

typedef struct {
	uint16_t AD5_val;
	__RW bool change;
	__RW int8_t timerId;
} ADC_t;

void ADC_init(void);
void ADC_start();
void ADC_stop();

#endif /* DRIVERS_ADC_H_ */
