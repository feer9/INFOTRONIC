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
#include "vcom/cdc_vcom.h"
#include "LCD.h"
#include "Aplicacion/Aplicacion.h"
#include "Timer.h"
#include <string.h>

int EEPROM_read(uint16_t address);
int EEPROM_write(uint16_t address, int cant);
uint8_t *EEPROM_getBuff(void);

#endif /* DRIVERS_24LC256_H_ */
