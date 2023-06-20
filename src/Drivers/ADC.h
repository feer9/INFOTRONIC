#ifndef DRIVERS_ADC_H_
#define DRIVERS_ADC_H_

#include "chip.h"
#include "KitInfo2_BaseBoard.h"
#include "GPIO.h"
#include "Aplicacion/Aplicacion.h"


void ADC_init(void);
void ADC_start();
void ADC_stop();

bool     ADC_hasChanged (void);
void     ADC_clearChange(void);
uint32_t ADC_getVal     (void);
bool     ADC_isOn       (void);

#endif /* DRIVERS_ADC_H_ */
