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
	ADC->ADCR |= (99 << CR_CLKDIV);

	set_dir(ADC5, ENTRADA);
	setPINSEL(ADC5, PINSEL_FUNC3);
	setPINMODE(ADC5, PINMODE_NONE);

	// seteo AD0.5 como fuente de entrada analogica
	ADC->ADCR |= (0x01 << 5);
	// seteo el modo burst, para que haga lecturas continuas
//	ADC->ADCR |= (0x01 << CR_BURST);


	ADC->ADINTEN = (0x01 << 5);
	// Habilito interrupcion en el NVIC
//	ISER0 = (0x01 << NVIC_ADC);

	ADC_stop(); // todavia no lo wa usar
}

void ADC_stop()
{
//	setPINSEL(ADC5, PINSEL_GPIO);
	ADC->ADCR _RESET_BIT(CR_BURST);
	ADC->ADCR _RESET_BIT(CR_PDN);
	PCONP _RESET_BIT(PCONP_ADC);
	ICER0 = _BIT(NVIC_ADC);
}

void ADC_start()
{
	PCONP _SET_BIT(PCONP_ADC);
	ADC->ADCR _SET_BIT(CR_PDN);
	ADC->ADCR _SET_BIT(CR_BURST);
//	setPINSEL(ADC5, PINSEL_FUNC3);
	ISER0 = (0x01 << NVIC_ADC);
}

void ADC_IRQHandler()
{
	uint32_t dr5 = ADC->ADDR5;
	static uint16_t prev = 0;

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
