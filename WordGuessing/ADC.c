#include "ADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void init_ADC(void)
{
	ADMUX = 0;
	ADCSRA = 0;
	
	ADMUX |= ((0 << REFS1) | (1 << REFS0)); // pg272 ADC voltage reference select
	ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) |(1 << ADPS0)); // pg276 Prescaler select
	ADCSRA |= (1 << ADEN); // pg275 Enable ADC mode
}

unsigned short int ADC_read(unsigned char channel)
{
	ADMUX &= 0xF8; // clear channel
	ADMUX |= channel;
	ADCSRA |= (1 << ADSC); // pg260 start conversion
	while(ADCSRA & (1 << ADSC)) // stay here until the conversion is done
	{
		
	}
	return ADC; // pg269
}