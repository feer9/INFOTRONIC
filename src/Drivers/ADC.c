#include "ADC.h"


// Control Register setting bits
#define		CR_SEL		0
#define		CR_CLKDIV	8
#define		CR_BURST	16
#define		CR_PDN		21
#define		CR_START	24
#define		CR_EDGE		27

struct ADC {
	__RW uint32_t AD5_val;
	__RW bool change;
	bool status;
} ;

static struct ADC adc = {
		.status = false,
		.AD5_val = 0,
		.change = true,
};

inline bool     ADC_hasChanged (void) { return adc.change;  }
inline void     ADC_clearChange(void) { adc.change = false; }
inline uint32_t ADC_getVal     (void) { return adc.AD5_val; }
inline bool     ADC_isOn       (void) { return adc.status;  }

void ADC_init()
{
	PCONP |= PCONP_ADC;
	// PDN = 1 => The A/D Converter is operational
	LPC_ADC->ADCR = _BIT(CR_PDN);

	// seteo la fuente de clock, core clock / 8 = 96/8 = 12MHz
	setPCLKDiv(PCLKSEL_ADC, PCLKDIV_8);

	// seteo el divisor de clock (tiene q quedar <= 13MHZ)
	// CLKDIV = 1 + este valor
	LPC_ADC->ADCR = (0UL << CR_CLKDIV); // (12MHz / 0+1) = 12MHz (< 13MHz)

	gpio_pinDir_input(ADC5);
	gpio_configPin(ADC5, PINMODE_NONE, PINSEL_FUNC3);

	// seteo AD0.5 como fuente de entrada analogica
	LPC_ADC->ADCR |= (1UL << 5);

	// Habilito interrupcion para AD0.5
	LPC_ADC->ADINTEN = (1UL << 5);

	// Lo dejo apagado porque no lo estoy usando
	ADC_stop();
}

void ADC_stop()
{
	gpio_pinsel(ADC5, PINSEL_GPIO);
	LPC_ADC->ADCR _RESET_BIT(CR_BURST);	// Conversions are software controlled
	LPC_ADC->ADCR _RESET_BIT(CR_PDN);	// The A/D converter is in power-down mode
	PCONP &= ~PCONP_ADC;
	ICER0 = NVIC_ADC;
	adc.status = false;
}

void ADC_start()
{
	PCONP |= PCONP_ADC;
	LPC_ADC->ADCR _SET_BIT(CR_PDN);		// The A/D converter is operational
	LPC_ADC->ADCR _SET_BIT(CR_BURST);	// The A/D converter does repeated conversions at up to 200 kHz
	gpio_pinsel(ADC5, PINSEL_FUNC3);
	ISER0 = NVIC_ADC;
	adc.status = true;
}

void ADC_IRQHandler()
{
	static uint32_t prev = 0;
	uint32_t dr5 = LPC_ADC->ADDR5;

	if(dr5 >> 31) // DONE
	{
//		if((dr5 >> 30) & 0x01) {} // overrun
		dr5 = (dr5 >> 4) & 0xFFF; // result
		if (dr5 != prev)
		{
			prev = dr5;
			adc.AD5_val = dr5;
			adc.change = true;
		}
	}
}
