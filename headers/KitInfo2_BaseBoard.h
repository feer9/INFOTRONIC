/*
 * KitInfo2_Baseboard.h
 *
 *  Created on: 17/04/2012
 *      Author: Mica y Chor
 */

#ifndef KIT_INFO2BASEBOARD_H_
#define KIT_INFO2BASEBOARD_H_

	#include <GPIO.h>
//******agregue aquí debajo los includes propios de su aplicación.

	#define SW1 0x02
	#define SW2 0x04
	#define SW3 0x08
	#define SW4 0x10
	#define SW5 0x20

//******                ****************                  *******//

//Identificación de los puertos de expansion:
	#define EXPANSION0		PORT2,7
	#define EXPANSION1		PORT1,29
	#define EXPANSION2		PORT4,28
	#define EXPANSION3		PORT1,23
	#define EXPANSION4		PORT1,20
	#define EXPANSION5		PORT0,19
	#define EXPANSION6		PORT3,26
	#define EXPANSION7		PORT1,25
	#define EXPANSION8		PORT1,22
	#define EXPANSION9		PORT1,19
	#define EXPANSION10		PORT0,20
	#define EXPANSION11		PORT3,25
	#define EXPANSION12		PORT1,27
	#define EXPANSION13		PORT1,24
	#define EXPANSION14		PORT1,21
	#define EXPANSION15		PORT1,18
	#define EXPANSION16		PORT1,31
	#define EXPANSION17		PORT0,24
	#define EXPANSION18		PORT0,25
	#define EXPANSION19		PORT0,17
	#define EXPANSION20		PORT1,31
	#define EXPANSION21		PORT0,22
	#define EXPANSION22		PORT0,15
	#define EXPANSION23		PORT0,16
	#define EXPANSION24		PORT2,8
	#define EXPANSION25		PORT2,12
	#define EXPANSION26		PORT1,31
	#define EXPANSION27		PORT1,31

	//Leds (Reles)
	#define		LED1		PORT2,0 		//GPIO2
	#define		LED2		PORT0,23		//GPIO0
	#define		LED3		PORT0,21		//GPIO0
	#define		LED4		PORT0,27		//GPIO0

	//Led RGB:
	#define		RGBB		PORT2,1			//GPIO2
	#define		RGBG		PORT2,2			//GPIO2
	#define		RGBR		PORT2,3			//GPIO2
	#define		BUZZ		PORT0,28		//GPIO0
	//Led LPCXPresso
	#define		LEDLPC_R	PORT0,22
	#define		LEDLPC_G	PORT3,25
	#define		LEDLPC_B	PORT3,26
	#define		LEDXpresso	LEDLPC_R

	//Teclas (teclado 5x1)
	#define		KEY0		PORT2,10		//SW1
	#define		KEY1		PORT0,18		//SW2
	#define		KEY2		PORT0,11		//SW3
	#define		KEY3_RC		PORT2,13		//SW4
	#define		KEY4_RC		PORT1,26		//SW5

	//Entradas digitales:
	#define		ED0		PORT1,26
	#define		ED1		PORT4,29
	#define		ED2		PORT2,11

	//GPIOS LCD:
	#define		LCD_D4		PORT0,5			//GPIO0
	#define		LCD_D5		PORT0,10		//GPIO0
	#define		LCD_D6		PORT2,4			//GPIO2
	#define		LCD_D7		PORT2,5			//GPIO2

	#define		LCD_RS		PORT2,6			//GPIO2
	#define		LCD_BF		PORT0,28		//GPIO1
	#define		LCD_E		PORT0,4			//GPIO0
/*
	RS = 0: IR write as an internal operation (display clear, etc.)
	RS = 1: Write data to DDRAM or CGRAM (DR to DDRAM or CGRAM)

	RS: register selector			BF: busy flag		E: chip enable signal
	IR: instruction register		DR: data register
	DDRAM: display data ram			CGRAM: character generator ram
*/

	//ADC
	#define 	ADC5		P1,31	//ADC5	//es el pote (OJO! el plano indica ADC0)
	#define 	ADC1		P0,24	//ADC1	//termistor
	#define 	ADC2		P0,25	//ADC2	//entrada libre


	//Defines generales:
	#define		OFF		0
	#define		ON		1

	#define		BUZZ_ON		0
	#define		BUZZ_OFF	1

	#define		LEDLPC_ON	0
	#define		LEDLPC_OFF	1

#endif /* KIT_INFO2_H_ */
