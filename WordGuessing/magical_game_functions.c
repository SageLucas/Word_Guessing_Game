#include "magical_game_functions.h"
#include "state_machine.h"
#include "ADC.h"
#include "HD44780.h"
#include "Debounce.h"

char* words_list[WORDS_DATABASE_SIZE] = {"KNOWLEDGE", "SCIENTIFIC", "TRANSMUTATION", "SAMURAI", "SYNERGIST", "OVERLORD", "DEMIURGE"};
	
/* char* alphabet = {}; // char* only works for string?
Warning        initialization makes pointer from integer without a cast [-Wint-conversion]
Warning        excess elements in scalar initializer */
char alphabet[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
char* word;
char* aux_word = "WASD"; //initialisation for first comparison in select_random_word()
char game_word[16];
char play_again[2] = { 'Y', 'N'}; //YES - NO
unsigned char play_again_choice;
char current_letter = 'A';
unsigned char letter_position = 0;
unsigned char remaining_tries = REMAINING_TRIES;

void select_random_word(void)
{
	//static char* word = words[(rand() % 7)]; - initializer element is not constant?
	word = words_list[(rand() % WORDS_DATABASE_SIZE)]; //(rand() % 7) - returns random integer from 0 to 6
	while (word == aux_word)
	{
		word = words_list[(rand() % WORDS_DATABASE_SIZE)];
	}
	aux_word = word; //saving selected word in aux in order to not get the same word when B3 (reset) button is pressed
	remaining_tries = REMAINING_TRIES; //resetting tries after selecting word
	for (unsigned char i = 0; i <= strlen(word); i++)
	{
		if ( (i == 0) || (i == (strlen(word) - 1)) ) //show first and last letter
		{
			game_word[i] = word[i];
		}
		else if ( (word[0] == word[i]) || (word[strlen(word)-1] == word[i]) ) //show duplicates of first and last letters
		{
			game_word[i] = word[i];
		}
		else
		{
			game_word[i] = '_'; //else underline (acting as blank space)
		}
	}
	game_word[strlen(word)] = '\0';
	letter_position = 0;
	current_letter = alphabet[letter_position];
	//return (char*) word; //expected char*, but argument is of type char -> LCD_WriteText(select_random_word()); (char*) cast doesn't work?
}

void display_data(void)
{
	//Line 0 display
	LCD_GoTo(0,0);
	LCD_WriteText(game_word);
	if (strlen(game_word) < 10) //pos 15 to write 1 character, pos 14 for 2 characters
	{
		LCD_GoTo(15,0);
		LCD_WriteDec(strlen(game_word));
	}
	else
	{
		LCD_GoTo(14,0);
		LCD_WriteDec(strlen(game_word));
	}
	
	//Line 1 display
	static unsigned short counter_ms = 0; //counter_ms used for blinking the selected (middle) letter: Z A B ('A' blinking)
	LCD_GoTo(15,1);
	LCD_WriteDec(remaining_tries);
	LCD_GoTo(0,1);

	if (letter_position == 25)
	{
		LCD_WriteData(alphabet[letter_position - 1]);
		LCD_WriteText(" ");
		if (counter_ms < 499)
		{
			LCD_WriteData(current_letter);
		}
		if (counter_ms > 499)
		{
			LCD_WriteText(" ");
		}
		if (counter_ms > 999)
		{
			counter_ms = 0;
		}
		counter_ms++;
		LCD_WriteText(" ");
		LCD_WriteData(alphabet[0]);
	}
	else if (letter_position == 0)
	{
		LCD_WriteData(alphabet[25]);
		LCD_WriteText(" ");
		if (counter_ms < 499)
		{
			LCD_WriteData(current_letter);
		}
		if (counter_ms > 499)
		{
			LCD_WriteText(" ");
		}
		if (counter_ms > 999)
		{
			counter_ms = 0;
		}
		counter_ms++;
		LCD_WriteText(" ");
		LCD_WriteData(alphabet[letter_position + 1]);
	}
	else
	{
		LCD_WriteData(alphabet[letter_position - 1]);
		LCD_WriteText(" ");
		if (counter_ms < 499)
		{
			LCD_WriteData(current_letter);
		}
		if (counter_ms > 499)
		{
			LCD_WriteText(" ");
		}
		if (counter_ms > 999)
		{
			counter_ms = 0;
		}
		counter_ms++;
		LCD_WriteText(" ");
		LCD_WriteData(alphabet[letter_position + 1]);
	}
}

void navigate_letters_right(void)
{
	if (letter_position < 25)
	{
		letter_position++;
	}
	else
	{
		letter_position = 0;
	}
	current_letter = alphabet[letter_position];
}

void navigate_letters_left(void)
{
	if (letter_position > 0)
	{
		letter_position--;
	}
	else
	{
		letter_position = 25;
	}
	current_letter = alphabet[letter_position];
}

void select_letter(void)
{
	unsigned char flag_found = FALSE;
	for (unsigned char i = 0; i < (strlen(game_word) - 1); i++)
	{
		if (current_letter == word[i])
		{
			game_word[i] = current_letter;
			flag_found = TRUE;
		}
	}
	if (flag_found == FALSE)
	{
		remaining_tries--;
	}
}

unsigned char check_word(void)
{
	unsigned char flag_found_word = TRUE;
	for (unsigned char i = 0; i < strlen(word) - 1; i++) //test with strlen(game_word)
	{
		if (game_word[i] != word[i])
		{
			flag_found_word = FALSE;
		}
	}
	return flag_found_word;
}

void win_state(void)
{
	LCD_GoTo(0,0);
	LCD_WriteText("You won!");
	LCD_GoTo(0,1);
	LCD_WriteText("Play again? ");
	LCD_GoTo(12,1);
	LCD_WriteText("Y/N");
	
	static unsigned short counter_ms_win = 0; //counter_ms used for blinking the selected letter Y/N
	static unsigned char aux_choice = YES; //default value of choice in "PLAY AGAIN?" screen is YES
	
	if ( (play_again_choice == YES && aux_choice == NO) || (play_again_choice == NO && aux_choice == YES) ) //reset blinking state when changing option Y / N
	{
		counter_ms_win = 0;
		aux_choice = play_again_choice; //aux_choice takes the value of current play_again_choice
	}
	if (play_again_choice == 0)
	{
		LCD_GoTo(12,1); //blinking Y
		if (counter_ms_win < 499)
		{
			LCD_WriteData(play_again[play_again_choice]);
		}
		if (counter_ms_win > 499)
		{
			LCD_WriteText(" ");
		}
		if (counter_ms_win > 999)
		{
			counter_ms_win = 0;
		}
		counter_ms_win++;
	}
	else
	{
		LCD_GoTo(14,1); //blinking N
		if (counter_ms_win < 499)
		{
			LCD_WriteData(play_again[play_again_choice]);
		}
		if (counter_ms_win > 499)
		{
			LCD_WriteText(" ");
		}
		if (counter_ms_win > 999)
		{
			counter_ms_win = 0;
		}
		counter_ms_win++;
	}
	
}

void lose_state(void)
{
	LCD_GoTo(0,0);
	LCD_WriteText("You lost!");
	LCD_GoTo(0,1);
	LCD_WriteText("Play again? ");
	LCD_GoTo(12,1);
	LCD_WriteText("Y/N");
	
	static unsigned short counter_ms_lose = 0; //counter_ms used for blinking the selected letter Y/N
	static unsigned char aux_choice = YES; //default value of choice in "PLAY AGAIN?" screen is YES
	
	if ( (play_again_choice == YES && aux_choice == NO) || (play_again_choice == NO && aux_choice == YES) ) //reset blinking state when changing option Y / N
	{
		counter_ms_lose = 0;
		aux_choice = play_again_choice; //aux_choice takes the value of current play_again_choice
	}
	
	if (play_again_choice == 0)
	{
		LCD_GoTo(12,1); //blinking Y
		if (counter_ms_lose < 499)
		{
			LCD_WriteData(play_again[play_again_choice]);
		}
		if (counter_ms_lose > 499)
		{
			LCD_WriteText(" ");
		}
		if (counter_ms_lose > 999)
		{
			counter_ms_lose = 0;
		}
		counter_ms_lose++;
	}
	else
	{
		LCD_GoTo(14,1); //blinking N
		if (counter_ms_lose < 499)
		{
			LCD_WriteData(play_again[play_again_choice]);
		}
		if (counter_ms_lose > 499)
		{
			LCD_WriteText(" ");
		}
		if (counter_ms_lose > 999)
		{
			counter_ms_lose = 0;
		}
		counter_ms_lose++;
	}
}

void stop_state(void)
{
	LCD_GoTo(0,0);
	LCD_WriteText("TY FOR PLAYING!");
	LCD_GoTo(0,1);
	LCD_WriteText("RESTART - B3");
}