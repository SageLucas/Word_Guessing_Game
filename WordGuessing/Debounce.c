#include "Debounce.h"

void init_button(Button* button, volatile unsigned char* direction_register, volatile unsigned char* port, volatile unsigned char* pinX, unsigned char pin_pos, unsigned char mode)
{
	button->pinX = pinX;
	button->pin_pos = pin_pos;
	
	*direction_register &= (~( 1 << pin_pos)); // input = 0, output = 1
	*port |= ( 1 << pin_pos); // pull up
	button->counter = 0;
	button->mode = mode;
}

unsigned char button_pressed(Button* button)
{
	//return !((*button->pinX >> button->pin_pos) & 1); // checks if button is pressed
	if ((button->mode != CHECK_SINGLE_MODE) && (button->mode != CHECK_CONT_MODE))
	{
		return 0;
	}
	
	if (!((*button->pinX >> button->pin_pos) & 1)) // if (button pressed)
	{
		if (button->counter < MAX_BTN_COUNT)
		{
			(button->counter)++;
		}
	}
	else
	{
		button->counter = 0;
	}
	
	if (button->counter == MAX_BTN_COUNT)
	{
		if (button->mode == CHECK_SINGLE_MODE)
		{
			button->counter = MAX_BTN_COUNT + 1;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}