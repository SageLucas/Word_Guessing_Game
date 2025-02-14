#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Debounce.h"

#define TRUE (1)
#define FALSE (0)

#define YES (0)
#define NO (1)

#define NAV_LETTER_LEFT (0)
#define NAV_LETTER_RIGHT (1)
#define SELECT_LETTER (2)
#define RESET_WORD (3)

typedef enum {
	IDLE_STATE,
	NAVIGATE_LETTER_STATE,
	CHECK_LETTER_STATE,
	RESET_WORD_STATE,
	WIN_STATE,
	LOSE_STATE,
	STOP_STATE
}States;

typedef struct {
	Button* button_array;
	unsigned char current_state;
}State_machine;

void execute_state_machine(State_machine* state_machine);

#endif /* STATE_MACHINE_H_ */