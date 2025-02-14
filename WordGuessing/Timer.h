#ifndef TIMER_H_
#define TIMER_H_
#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint8_t flag_1ms;
void init_timer0_OVF(void);
void init_timer0_CMP(void);

#endif /* TIMER_H_ */