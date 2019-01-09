/*
 * KitInfo2_Baseboard.h
 *
 *  Created on: 17/04/2012
 *      Author: Mica y Chor
 */

#ifndef KITINFO2_BASEBOARD_H_
#define KITINFO2_BASEBOARD_H_

#include <GPIO.h>


#define CORE_CLK	100000000

// identificadores de los puertos
#define P0		0
#define P1		1
#define P2		2
#define P3		3
#define P4		4


// Identificación de los puertos de expansion:
#define		EXPANSION0	P2,7
#define		EXPANSION1	P1,29
#define		EXPANSION2	P4,28
#define		EXPANSION3	P1,23
#define		EXPANSION4	P1,20
#define		EXPANSION5	P0,19
#define		EXPANSION6	P3,26
#define		EXPANSION7	P1,25
#define		EXPANSION8	P1,22
#define		EXPANSION9	P1,19
#define		EXPANSION10	P0,20
#define		EXPANSION11	P3,25
#define		EXPANSION12	P1,27
#define		EXPANSION13	P1,24
#define		EXPANSION14	P1,21
#define		EXPANSION15	P1,18
#define		EXPANSION16	P1,31
#define		EXPANSION17	P0,24
#define		EXPANSION18	P0,25
#define		EXPANSION19	P0,17
#define		EXPANSION20	P1,31
#define		EXPANSION21	P0,22
#define		EXPANSION22	P0,15
#define		EXPANSION23	P0,16
#define		EXPANSION24	P2,8
#define		EXPANSION25	P2,12
#define		EXPANSION26	P1,31
#define		EXPANSION27	P1,31

// Teclado 5x1
#define 	SW1			1
#define 	SW2			2
#define 	SW3			3
#define 	SW4			4
#define 	SW5			5

#define		SW1_PORT	P2
#define		SW2_PORT	P0
#define		SW3_PORT	P0
#define		SW4_PORT	P2
#define		SW5_PORT	P1

#define		SW1_PIN		10
#define		SW2_PIN		18
#define		SW3_PIN		11
#define		SW4_PIN		13
#define		SW5_PIN		26

#define		KEY0		SW1_PORT,SW1_PIN
#define		KEY1		SW2_PORT,SW2_PIN
#define		KEY2		SW3_PORT,SW3_PIN
#define		KEY3		SW4_PORT,SW4_PIN
#define		KEY4		SW5_PORT,SW5_PIN
#define		KEY3_RC		SW4_PORT,SW4_PIN
#define		KEY4_RC		SW5_PORT,SW5_PIN

// Entradas digitales:
#define		ED0			P1,26
#define		ED1			P4,29
#define		ED2			P2,11

// Entradas analógicas / ADC
#define		EA0			P1,31
#define		EA1			P0,24
#define		EA2			P0,25

#define 	ADC5		P1,31	// es el pote (OJO! el plano indica ADC0)
#define 	ADC1		P0,24	// termistor
#define 	ADC2		P0,25	// entrada libre

// Leds / Salidas digitales / Relés
#define		LED0		P2,0
#define		LED1		P0,23
#define		LED2		P0,21
#define		LED3		P0,27
#define		LED4		P0,28

#define		SD0			P2,0
#define		SD1			P0,23
#define		SD2			P0,21
#define		SD3			P0,27
#define		SD4			P0,28

#define		RELAY0		P2,0
#define		RELAY1		P0,23
#define		RELAY2		P0,21
#define		RELAY3		P0,27
#define		RELAY4		P0,28

// Buzzer
#define		BUZZ		P0,28

// Salida analógica
#define		SA			P0,26

// Led RGB / Salidas PWM
#define		RGB_R		P2,3
#define		RGB_G		P2,2
#define		RGB_B		P2,1

#define		PWM0		P2,3
#define		PWM1		P2,2
#define		PWM2		P2,1
#define		PWM3		P1,28

// Led LPCXPresso
#define		LEDXpresso	P0,22
#define		LEDLPC_R	P0,22
#define		LEDLPC_G	P3,25
#define		LEDLPC_B	P3,26

#define		LEDLPC_ON	0
#define		LEDLPC_OFF	1

// LCD:
/*	RS = 0: IR write as an internal operation (display clear, etc.)
	RS = 1: Write data to DDRAM or CGRAM (DR to DDRAM or CGRAM)

	RS: register selector			BF: busy flag		E: chip enable signal
	IR: instruction register		DR: data register
	DDRAM: display data ram			CGRAM: character generator ram           */
#define		LCD_D4		P0,5
#define		LCD_D5		P0,10
#define		LCD_D6		P2,4
#define		LCD_D7		P2,5

#define		LCD_RS		P2,6
#define		LCD_E		P0,4

// Comunicacion serie
#define		CTS1		P0,17
#define		RTS1		P0,22
#define		Rx1			P0,16
#define		Tx1			P0,15

// USB a serie
#define		Tx0			P0,2
#define		Rx0			P0,3

// USB Device
#define		VBUS		P1,30

// USB Host / USB Device
#define		USB_Dn		P0,29
#define		USB_Dp		P0,30

// Ethernet
/*  a configurar
 * P1,0          *
 * P1,1          *
 * P1,4          *
 * P1,8  a P1,10 *
 * P1,14 a P1,17 */
#define		ETH_RXN
#define		ETH_RXP
#define		ETH_TXN
#define		ETH_TXP

// Memorias
#define		SCL			P0,1
#define		SDA			P0,0
#define		MISO		P0,8
#define		MOSI		P0,9
#define		SCK			P0,7
#define		SSEL		P0,6

// Defines generales:
#define		OFF			0
#define		ON			1

#define		OK			0
#define		ERROR		1

#define		BUZZER_ON	0
#define		BUZZER_OFF	1

#define		LEDLPC_ON	0
#define		LEDLPC_OFF	1


#endif /* KIT_INFO2_H_ */
