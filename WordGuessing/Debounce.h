#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_
#include <avr/io.h>
#include <avr/interrupt.h>

#define CHECK_SINGLE_MODE 1
#define CHECK_CONT_MODE 2
#define MAX_BTN_COUNT 10
#define BTN_NAV_UP 0
#define BTN_NAV_DOWN 1
#define BTN_SELECT 2
#define BTN_RESET 3

typedef struct 
{
	volatile unsigned char* pinX; // pin register
	unsigned char pin_pos;
	unsigned char counter;
	unsigned char mode;
}Button;

void init_button(Button* button, volatile unsigned char* direction_register, volatile unsigned char* port, volatile unsigned char* pinX, unsigned char pin_pos, unsigned char mode);
unsigned char button_pressed(Button* button);

#endif /* DEBOUNCE_H_ */