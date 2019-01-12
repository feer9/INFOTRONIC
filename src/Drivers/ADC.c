#include "ADC.h"

ADC_t adc = {
		.AD5_val = 0,
		.change = FALSE,
		.timerId = -1
};

void ADC_init()
{
	PCONP _SET_BIT(PCONP_ADC);
	// PDN = 1 => The A/D Converter is operational
	ADC->ADCR = _BIT(CR_PDN);

	// seteo la fuente de clock, core clock / 8 = 12,5MHz
	PCLKSEL0 |= (PCLK_CCLK_8 << PCLKSEL_ADC);
	// seteo el divisor de clock (tiene q quedar <= 13MHZ)
	// CLKDIV = 1 + este valor
	ADC->ADCR |= (99UL << CR_CLKDIV);

	set_dir(ADC5, ENTRADA);
	setPINSEL(ADC5, PINSEL_FUNC3);
	setPINMODE(ADC5, PINMODE_NONE);

	// seteo AD0.5 como fuente de entrada analogica
	ADC->ADCR |= (1UL << 5);

	// Habilito interrupcion para AD0.5
	ADC->ADINTEN = (1UL << 5);

	// Lo dejo apagado porque no lo estoy usando
	ADC_stop();
}

void ADC_stop()
{
	setPINSEL(ADC5, PINSEL_GPIO);
	ADC->ADCR _RESET_BIT(CR_BURST);	// Conversions are software controlled
	ADC->ADCR _RESET_BIT(CR_PDN);	// The A/D converter is in power-down mode
	PCONP _RESET_BIT(PCONP_ADC);
	ICER0 = _BIT(NVIC_ADC);
}

void ADC_start()
{
	PCONP _SET_BIT(PCONP_ADC);
	ADC->ADCR _SET_BIT(CR_PDN);		// The A/D converter is operational
	ADC->ADCR _SET_BIT(CR_BURST);	// The A/D converter does repeated conversions at up to 200 kHz
	setPINSEL(ADC5, PINSEL_FUNC3);
	ISER0 = (1UL << NVIC_ADC);
}

void ADC_IRQHandler()
{
	static uint16_t prev = 0;
	uint32_t dr5 = ADC->ADDR5;

	if(dr5 >> 31) // DONE
	{
//		if((dr5 >> 30) & 0x01) {} // overrun
		dr5 = (dr5 >> 4) & 0xFFF; // result
		if (dr5 != prev)
		{
			prev = dr5;
			adc.AD5_val = dr5;
			adc.change = TRUE;
		}
	}
}
