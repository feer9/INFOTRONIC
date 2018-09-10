#include "RTC.h"
#include "LCD.h"


extern LCD_t LCD;

void RTC_init()
{
	PCONP _SET_BIT(9);				// power control periferic rtc
	LPC_RTC->RTC_AUXEN _SET_BIT(4);		// the RTC Oscillator Fail detect interrupt is enabled

	if(LPC_RTC->RTC_AUX & _BIT(4))		// RTC Oscillator Fail detect flag.
	{
//		CCR->CLKEN  = 0 : The time counters are disabled so that they may be initialized.
//		CCR->CTCRST = 1 : The elements in the internal oscillator divider are reset
//		CCR->CCALEN = 1 : The calibration counter is disabled and reset to zero
		LPC_RTC->CCR = 0x12;

		LPC_RTC->CALIBRATION = 86400 | (0x01 << 17); 	// adelantaba ~1seg / dia

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

		LPC_RTC->RTC_AUX _SET_BIT(4);	// clear flag

//		CCR->CLKEN  = 1: The time counters are enabled
//		CCR->CTCRST = 0: Remain reset until CCR[1] is changed to zero -> lo pongo a 0
//		CCR->CCALEN = 0 : The calibration counter is enabled and counting
		LPC_RTC->CCR = 0x01;
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
		if(LCD.isInClock && LCD.isOn)
			LCD_updateClock();
	}

	// RTCALF (alarma)
	if(LPC_RTC->ILR & _BIT(1))
	{
		LPC_RTC->ILR _SET_BIT(1);
	}

}

void RTC_setTime (rtc_t *rtc)
{
	LPC_RTC->SEC   = rtc->sec;
	LPC_RTC->MIN   = rtc->min;
	LPC_RTC->HOUR  = rtc->hour;
	LPC_RTC->DOM   = rtc->dom;
	LPC_RTC->DOW   = rtc->dow;
	LPC_RTC->DOY   = rtc->doy;
	LPC_RTC->MONTH = rtc->month;
	LPC_RTC->YEAR  = rtc->year;
}

void RTC_getTime(rtc_t *rtc)
{
	rtc->sec   = LPC_RTC->SEC;
	rtc->min   = LPC_RTC->MIN;
	rtc->hour  = LPC_RTC->HOUR;
	rtc->dom   = LPC_RTC->DOM;
	rtc->dow   = LPC_RTC->DOW;
	rtc->doy   = LPC_RTC->DOY;
	rtc->month = LPC_RTC->MONTH;
	rtc->year  = LPC_RTC->YEAR;
}
