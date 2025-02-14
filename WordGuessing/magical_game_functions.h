#ifndef MAGICAL_GAME_FUNCTIONS_H_
#define MAGICAL_GAME_FUNCTIONS_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <time.h>
#include <string.h>
#include "state_machine.h"

#define WORDS_DATABASE_SIZE (7)
#define REMAINING_TRIES (6)

extern char* words_list[7];
extern char alphabet[26];
extern char* word;
extern char* aux_word;
extern char game_word[16];
extern char current_letter;
extern unsigned char letter_position;
extern unsigned char remaining_tries;
extern char play_again[2];
extern unsigned char play_again_choice;

void select_random_word(void);
void display_data(void);
void navigate_letters_right(void);
void navigate_letters_left(void);
void select_letter(void);
unsigned char check_word(void);
void win_state(void);
void lose_state(void);
void stop_state(void);

#endif /* MAGICAL_GAME_FUNCTIONS_H_ */