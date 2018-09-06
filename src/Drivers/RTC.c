#include <Aplicacion.h>

#define _RTC_SET_TIME 0

extern uint8_t displayClockStatus;

void initRTC()
{
	PCONP _SET_BIT(9);				// power control periferic rtc
	LPC_RTC->RTC_AUXEN _SET_BIT(4);		// the RTC Oscillator Fail detect interrupt is enabled
//	LPC_RTC->RTC_AUXEN = 0;

	if(LPC_RTC->RTC_AUX & _BIT(4))		// RTC Oscillator Fail detect flag.
	{
//		LPC_RTC->CCR _RESET_BIT(0);		// The time counters are disabled so that they may be initialized.
//		LPC_RTC->CCR _SET_BIT(1);		// the elements in the internal oscillator divider are reset
//		LPC_RTC->CCR _SET_BIT(4);		// The calibration counter is disabled and reset to zero
		LPC_RTC->CCR = 0b10010;
		LPC_RTC->CALIBRATION = 0x0; 	//!< parece que adelanta ~1seg / dia

#if _RTC_SET_TIME
		LPC_RTC->YEAR  = 2018;
		LPC_RTC->MONTH = 9;
		LPC_RTC->DOY   = 246;
		LPC_RTC->DOM   = 3;
		LPC_RTC->DOW   = 1;
		LPC_RTC->HOUR  = 5;
		LPC_RTC->MIN   = 43;
		LPC_RTC->SEC   = 0;

		LPC_RTC->GPREG0 = 20180903;
		LPC_RTC->GPREG1 = 54300;
		LPC_RTC->GPREG2 = 0;
		LPC_RTC->GPREG3 = 0;
		LPC_RTC->GPREG4 = 0;
#endif
		LPC_RTC->RTC_AUX _SET_BIT(4);	// clear flag
//		LPC_RTC->CCR _RESET_BIT(1);		// remain reset until CCR[1] is changed to zero -> lo pongo a 0
//		LPC_RTC->CCR _SET_BIT(0);		// The time counters are enabled
		LPC_RTC->CCR = 0b10001;
	}

	LPC_RTC->CIIR = 0x01;		// interrupcion cada: bit0->seg bit1->min bit2->hora...
	LPC_RTC->AMR = 0xFF;		// when 1, the * value is not compared for the alarm
	ICPR0 _SET_BIT(17);				// limpio interrupcion
	ISER0 _SET_BIT(17);				// habilito interrupcion en el NVIC
}

void RTC_IRQHandler(void)
{
	// RTCCIF (counter increment interrupt)
	if(LPC_RTC->ILR & _BIT(0))
	{
		LPC_RTC->ILR _SET_BIT(0);
		if(displayClockStatus == ON)
			LCD_updateClock();
	}

	// RTCALF (alarma)
	if(LPC_RTC->ILR & _BIT(1))
	{
		LPC_RTC->ILR _SET_BIT(1);
	}

}

void RTC_setTime (uint16_t Y, uint16_t M, uint16_t D, \
		uint8_t h, uint8_t m, uint8_t s)
{

}
