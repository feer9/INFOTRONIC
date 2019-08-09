/*
 * rit.h
 *
 *  Created on: 1 ago. 2019
 *      Author: Feer
 */

#ifndef DRIVERS_RIT_H_
#define DRIVERS_RIT_H_

void RIT_init(uint32_t us);

void RIT_enableInterrupts(void);
void RIT_disableInterrupts(void);

#endif /* DRIVERS_RIT_H_ */
