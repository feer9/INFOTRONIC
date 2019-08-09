#include <string.h>
#include "../Drivers/KitInfo2_BaseBoard.h"
#include "../Drivers/GPIO.h"
#include "../Drivers/i2c.h"
#include "../Drivers/delay.h"
#include "../Drivers/u8g2/u8x8.h"
#include "../Drivers/oled_ssd1306.h"


uint8_t u8x8_byte_i2c_lpc1769(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  static uint8_t len;
  static uint8_t buffer[32];

  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      memcpy((void*)(buffer + len), arg_ptr, (size_t) arg_int);
      len += arg_int;
      break;

    case U8X8_MSG_BYTE_INIT:
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
    	len = 0;
    	u8x8->i2c_started = 1;
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        I2C_MasterSend(I2C, u8x8_GetI2CAddress(u8x8), buffer, len);
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
		configurePin(KEY_SELECT, PINMODE_PULLUP, PINSEL_GPIO);
		set_dir_input(KEY_SELECT);
/*
		configurePin(KEY_PREV, PINMODE_PULLUP, PINSEL_GPIO);
		set_dir_input(KEY_PREV);

		configurePin(KEY_NEXT, PINMODE_PULLUP, PINSEL_GPIO);
		set_dir_input(KEY_NEXT);

		configurePin(KEY_HOME, PINMODE_PULLUP, PINSEL_GPIO);
		set_dir_input(KEY_HOME);
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
      u8x8_SetGPIOResult(u8x8, read(KEY_SELECT));
      break;
/*    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, read(KEY_NEXT));
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, read(KEY_PREV));
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, read(KEY_HOME));
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
		setPINSEL(I2C_SCL, PINSEL_GPIO);
		setPINMODE(I2C_SCL, PINMODE_PULLUP);
		set_dir_input(I2C_SCL);

		setPINSEL(I2C_SDA, PINSEL_GPIO);
		setPINMODE(I2C_SDA, PINMODE_PULLUP);
		set_dir_input(I2C_SCL);

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
      delay_us(arg_int*1000UL);
      break;
    case U8X8_MSG_DELAY_I2C:
      /* arg_int is 1 or 4: 100KHz (5us) or 400KHz (1.25us) */
      delay_us(arg_int<=2?5:1);
      break;

    case U8X8_MSG_GPIO_I2C_CLOCK:
      if ( arg_int == 0 )
      {
    	  set_dir_output(I2C_SCL);
    	  clear_pin(I2C_SCL);
      }
      else
      {
//	Chip_GPIO_SetPinOutHigh(LPC_GPIO, I2C_SCL);//
    	  set_dir_input(I2C_SCL);
      }
      break;
    case U8X8_MSG_GPIO_I2C_DATA:
      if ( arg_int == 0 )
      {
    	  set_dir_output(I2C_SDA);
    	  clear_pin(I2C_SDA);
      }
      else
      {
//	Chip_GPIO_SetPinOutHigh(LPC_GPIO, I2C_SDA);//
    	  set_dir_input(I2C_SDA);
      }
      break;

    default:
      u8x8_SetGPIOResult(u8x8, 1);
      break;
  }
  return 1;
}
