#include <string.h>
#include "KitInfo2_BaseBoard.h"
#include "Drivers/GPIO.h"
#include "Drivers/i2c.h"
#include "Drivers/delay.h"
#include "Drivers/u8g2/u8x8.h"
#include "Drivers/oled_ssd1306.h"


uint8_t u8x8_byte_i2c_lpc1769(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  static uint16_t cmdLen = 0;
  static uint8_t cmd[32] = {0};
  static bool sending_cmd = false, sending_data = false;
  static const uint8_t  cmd_word = 0x00;
  static const uint8_t data_word = 0x40;

  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
    	if(arg_int == 1 && !sending_cmd && !sending_data) {
    		if( *(uint8_t*)arg_ptr == 0x000) {
    			sending_cmd = true;
    		}
    		else if(*(uint8_t*)arg_ptr == 0x040) {
    			sending_data = true;
    		}
    	}
    	else if(sending_cmd && arg_int == 1) {
    		cmd[cmdLen++] = *(uint8_t*)arg_ptr;
    	}
    	else if(sending_data) {
    		I2C_MasterSendCmdData(I2C, u8x8_GetI2CAddress(u8x8),
    								&data_word,1,
    								(uint8_t*)arg_ptr,arg_int,
									NULL, 0);
    		sending_data = false;
    	}
    	else {
    		while(1); // catch unexpected msg
    	}
      break;

    case U8X8_MSG_BYTE_INIT:
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
    	cmdLen = 0;
    	sending_data = sending_cmd = false;
    	u8x8->i2c_started = 1;
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
    	if(sending_cmd) {
    		sending_cmd = false;
    		I2C_MasterSendCmdData(I2C, u8x8_GetI2CAddress(u8x8),
    								&cmd_word,1,
									cmd,cmdLen,
									NULL, I2C_OPT_COPY_TXDATA);
    	}
    	u8x8->i2c_started = 0;
      break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8x8_gpio_and_delay_lpc1769(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	switch(msg)
	{
	case U8X8_MSG_GPIO_AND_DELAY_INIT:
		// set up lpc1769 hardware i2c
		I2C_Config(I2C, 400000, I2C_EventHandler);

		// configure action buttons
		gpio_configPin(KEY_SELECT, PINMODE_PULLUP, PINSEL_GPIO);
		gpio_pinDir_input(KEY_SELECT);
/*
		gpio_configPin(KEY_PREV, PINMODE_PULLUP, PINSEL_GPIO);
		gpio_pinDir_input(KEY_PREV);

		gpio_configPin(KEY_NEXT, PINMODE_PULLUP, PINSEL_GPIO);
		gpio_pinDir_input(KEY_NEXT);

		gpio_configPin(KEY_HOME, PINMODE_PULLUP, PINSEL_GPIO);
		gpio_pinDir_input(KEY_HOME);
*/
		break;


    case U8X8_MSG_DELAY_NANO:
      /* not required for SW I2C */
      break;

    case U8X8_MSG_DELAY_10MICRO:
      /* not used at the moment */
      break;

    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
      break;

    case U8X8_MSG_DELAY_MILLI:
   //   delay_ms(arg_int);
      break;
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
   //   delay_us(arg_int<=2?5:1);
      break;

    case U8X8_MSG_GPIO_I2C_CLOCK:
     /* not used, HW i2c handles it */
     break;
    case U8X8_MSG_GPIO_I2C_DATA:
        /* not used, HW i2c handles it */
        break;
      break;

    case U8X8_MSG_GPIO_MENU_SELECT:
      u8x8_SetGPIOResult(u8x8, gpio_readPin(KEY_SELECT));
      break;
/*    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, gpio_readPin(KEY_NEXT));
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, gpio_readPin(KEY_PREV));
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, gpio_readPin(KEY_HOME));
      break;
*/
    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}

uint8_t u8x8_gpio_and_delay_sw_lpc1769(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:

      /* only support for software I2C*/
    	gpio_configPin(I2C_SCL, PINMODE_PULLUP, PINSEL_GPIO);
		gpio_pinDir_input(I2C_SCL);

		gpio_configPin(I2C_SDA, PINMODE_PULLUP, PINSEL_GPIO);
		gpio_pinDir_input(I2C_SCL);

      break;
    case U8X8_MSG_DELAY_NANO:
      /* not required for SW I2C */
      break;

    case U8X8_MSG_DELAY_10MICRO:
      /* not used at the moment */
      break;

    case U8X8_MSG_DELAY_100NANO:
      /* not used at the moment */
      break;

    case U8X8_MSG_DELAY_MILLI:
      delay_ms(arg_int);
      break;
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
      delay_us(arg_int<=2?5:1);
      break;

    case U8X8_MSG_GPIO_I2C_CLOCK:
      if ( arg_int == 0 )
      {
    	  gpio_pinDir_output(I2C_SCL);
    	  gpio_clearPin(I2C_SCL);
      }
      else
      {
//        gpio_setPin(I2C_SCL);
    	  gpio_pinDir_input(I2C_SCL);
      }
      break;
    case U8X8_MSG_GPIO_I2C_DATA:
      if ( arg_int == 0 )
      {
    	  gpio_pinDir_output(I2C_SDA);
    	  gpio_clearPin(I2C_SDA);
      }
      else
      {
//        gpio_setPin(I2C_SDA);
    	  gpio_pinDir_input(I2C_SDA);
      }
      break;

    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}
