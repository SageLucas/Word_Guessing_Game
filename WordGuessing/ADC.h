#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>
#include <avr/interrupt.h>

void init_ADC(void);
unsigned short int ADC_read(unsigned char channel);

#endif /* ADC_H_ */