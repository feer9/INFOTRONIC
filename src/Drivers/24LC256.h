/*
 * 24LC256.h
 *
 *  Created on: 3 ago. 2019
 *      Author: Feer
 */

#ifndef DRIVERS_24LC256_H_
#define DRIVERS_24LC256_H_

#include "chip.h"
#include "KitInfo2_BaseBoard.h"
#include "i2c.h"
#include "Timer.h"
#include <string.h>


status EEPROM_read(uint16_t address, uint8_t *buff, uint16_t sz);
status EEPROM_write(uint16_t address, const uint8_t *buff, uint16_t sz);
status EEPROM_setCallback(void (*cb) (status));

#endif /* DRIVERS_24LC256_H_ */
