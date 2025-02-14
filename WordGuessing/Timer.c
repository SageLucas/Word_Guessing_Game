#include "Timer.h"

void init_timer0_OVF(void)
{
	TCNT0 = 6; // registrul de numarare // char are 256 numarari 0 --> 255
	// deci ca sa numar 250 de cicluri pornesc mereu de la 6 --> 255
	TCCR0 |= ((0 << CS02) | (1 << CS01) | (1 << CS00)); // pagina 164 prescaler 164
	TCCR0 &= (~((1 << WGM01) | (1 << WGM00))); // Normal Mode tab 21.2 pagina 73
	TIMSK |= (1 << TOIE0); // pagina 178
	// 64 prescaler * 62.5ns = 4000ns = 4us
	// 1000 / 4 = 250 numarari ca sa obtin o microsecunda
}

void init_timer0_CMP(void)
{
	OCR0 = 250 - 1;
	TCCR0 |= ((0 << CS02) | (1 << CS01) | (1 << CS00)); // pg175 prescaler
	TCCR0 |= ((1 << WGM01) | (0 << WGM00)); // pg173 CTC mode
	TIMSK |= (1 << OCIE0); // pg178
}

ISR(TIMER0_OVF_vect)
{
// cod care va fi executat la intreruperea de overflow pentru TIMER0
	TCNT0 = 6;
	flag_1ms = 1;
}

ISR(TIMER0_COMP_vect)
{
	// cod care va fi executat la intreruperea la comparatie pentru TIMER0
	flag_1ms = 1;
}