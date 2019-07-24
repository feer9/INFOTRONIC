/*
 * ssd1306.h
 *
 *  Created on: 23 jul. 2019
 *      Author: feer
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include "lpc_types.h"
#include "u8g2/u8g2.h"
#include "u8g2/u8x8.h"


#define		KEY_SELECT		2,10 /* boton del lpc */
//#define		KEY_NEXT
//#define		KEY_PREV
//#define		KEY_HOME


#define bitmap_linux_width 64
#define bitmap_linux_height 64
extern const unsigned char bitmap_linux[] U8X8_PROGMEM;

#define bitmap_mushroom_width 64
#define bitmap_mushroom_height 64
extern const unsigned char bitmap_mushroom[] U8X8_PROGMEM;

uint8_t u8x8_gpio_and_delay_sw_lpc1769(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_gpio_and_delay_lpc1769(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_i2c_lpc1769(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void sm_ssd1306(u8g2_t *u8g2);
void ssd1306_init(u8g2_t *u8g2);

#endif /* SSD1306_H_ */
