/*
 * digital_outs.h
 *
 *  Created on: 30 jul. 2019
 *      Author: Feer
 */

#ifndef DRIVERS_DIGITAL_OUTPUTS_H_
#define DRIVERS_DIGITAL_OUTPUTS_H_

#include <stdint.h>
#include <stdbool.h>

void D_OUT_init( void );
void D_OUT_toggle(uint8_t n);
bool D_OUT_getStatus(uint8_t n);
void turnLedsLpcOff(void);
void flashLedLpc(uint8_t port, uint8_t pin);

#endif /* DRIVERS_DIGITAL_OUTPUTS_H_ */
