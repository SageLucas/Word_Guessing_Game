#include "state_machine.h"
#include "magical_game_functions.h"
#include "HD44780.h"
#include "Debounce.h"

unsigned char remaining_tries;
unsigned char play_again_choice = 0;

void execute_state_machine(State_machine* state_machine)
{
	switch (state_machine->current_state)
	{
		case (IDLE_STATE):
		{
			display_data(); //displaying all data on the screen
			if (button_pressed(&state_machine->button_array[NAV_LETTER_LEFT]))
 			{
				navigate_letters_left();
 				state_machine->current_state = NAVIGATE_LETTER_STATE;
 			}
 			if (button_pressed(&state_machine->button_array[NAV_LETTER_RIGHT]))
 			{
				navigate_letters_right();
 				state_machine->current_state = NAVIGATE_LETTER_STATE;
 			}
			if (button_pressed(&state_machine->button_array[RESET_WORD]))
			{
				state_machine->current_state = RESET_WORD_STATE;
			}
			if (button_pressed(&state_machine->button_array[SELECT_LETTER]))
			{
				state_machine->current_state = CHECK_LETTER_STATE;
			}
			break;
		}
		case (NAVIGATE_LETTER_STATE):
		{
			display_data(); //displaying all data on the screen
			if (button_pressed(&state_machine->button_array[NAV_LETTER_LEFT]))
			{
				navigate_letters_left();
			}
			if (button_pressed(&state_machine->button_array[NAV_LETTER_RIGHT]))
			{
				navigate_letters_right();
			}
			if (button_pressed(&state_machine->button_array[SELECT_LETTER]))
			{
				state_machine->current_state = CHECK_LETTER_STATE;
			}
			if (button_pressed(&state_machine->button_array[RESET_WORD]))
			{
				state_machine->current_state = RESET_WORD_STATE;
			}
			if (remaining_tries == 0)
			{
				LCD_Clear();
				state_machine->current_state = LOSE_STATE;
			}
			break;
		}
		case (CHECK_LETTER_STATE):
		{
			select_letter();
			
			if (check_word()) //checks if all letters in word have been found
			{
				LCD_Clear();
				state_machine->current_state = WIN_STATE;
			}
			else
			{
				state_machine->current_state = NAVIGATE_LETTER_STATE;
			}
			break;
		}
		case (RESET_WORD_STATE):
		{
			LCD_Clear();
			select_random_word();
			state_machine->current_state = IDLE_STATE;
			break;
		}
		case (WIN_STATE):
		{
			win_state();
			if (button_pressed(&state_machine->button_array[NAV_LETTER_LEFT])) // PLAY AGAIN: Y / N
			{
				play_again_choice = YES; // 0 = YES
			}
			if (button_pressed(&state_machine->button_array[NAV_LETTER_RIGHT]))
			{
				play_again_choice = NO; // 1 = NO
			}
			if (button_pressed(&state_machine->button_array[SELECT_LETTER]))
			{
				if (play_again_choice == YES)
				{
					LCD_Clear();
					play_again_choice = YES; //resetting choice to YES
					state_machine->current_state = RESET_WORD_STATE;
				}
				else
				{
					LCD_Clear();
					play_again_choice = YES; //resetting choice to YES
					state_machine->current_state = STOP_STATE;
				}
			}
			break;
		}
		case (LOSE_STATE):
		{
			lose_state();
			if (button_pressed(&state_machine->button_array[NAV_LETTER_LEFT])) // PLAY AGAIN: Y / N
			{
				play_again_choice = YES; // 0 = YES
			}
			if (button_pressed(&state_machine->button_array[NAV_LETTER_RIGHT]))
			{
				play_again_choice = NO; // 1 = NO
			}
			if (button_pressed(&state_machine->button_array[SELECT_LETTER]))
			{
				if (play_again_choice == YES)
				{
					LCD_Clear();
					play_again_choice = YES; //resetting choice to YES
					state_machine->current_state = RESET_WORD_STATE;
				}
				else
				{
					LCD_Clear();
					play_again_choice = YES; //resetting choice to YES
					state_machine->current_state = STOP_STATE;
				}
			}
// 			if (button_pressed(&state_machine->button_array[SELECT_LETTER]) && play_again_choice == 0 ) // ???
// 			{
// 				LCD_Clear();
// 				state_machine->current_state = RESET_WORD_STATE;
// 			}
// 			if (button_pressed(&state_machine->button_array[SELECT_LETTER]) && play_again_choice == 1) // OK
// 			{
// 				LCD_Clear();
// 				state_machine->current_state = STOP_STATE;
// 			}
			break;
		}
		case (STOP_STATE):
		{
			stop_state();
			if (button_pressed(&state_machine->button_array[RESET_WORD]))
			{
				LCD_Clear();
				state_machine->current_state = RESET_WORD_STATE;
			}
			break;
		}
		default:break;
	}
}