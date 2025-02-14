#include <avr/io.h>
#include <avr/interrupt.h>
#include "HD44780.h" //LCD functions
#include "Debounce.h"
#include "Timer.h"
#include "ADC.h"
#include "state_machine.h"
#include "magical_game_functions.h"

volatile uint8_t flag_1ms = 0;

int main(void)
{
	Button button_array[4]; //B0 - LEFT, B1 - RIGHT, B2 - SELECT, B3 - RESET GAME
	State_machine state_machine;
	
	//initialising timer, sei() enable interrupts (cli() disable interrupts)
	init_timer0_CMP();
	sei();
	
	//initialising ADC
	init_ADC();
	unsigned char channel = PINA0; //ADC0 is set on PIN A0 = 0
	srand(ADC_read(channel)); //setting ADC channel as the seed for picking random numbers
	
	//initialising buttons
	for (unsigned char i = 0; i <= 3; i++)
	{
		init_button(&button_array[i], &DDRB, &PORTB, &PINB, i, CHECK_SINGLE_MODE);
	}
	state_machine.button_array = button_array;
	
	//initialising starting state
	state_machine.current_state = IDLE_STATE;
	
	//initialising LCD
	LCD_Initialize();
	LCD_Clear();
	select_random_word(); //selecting starting word
	
	while (1)
	{
		if (flag_1ms) // flag_1ms == 1 when TIMER0 == OCR0
		{
			flag_1ms = 0;
			execute_state_machine(&state_machine);
		}
	}
}