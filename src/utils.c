#include<stdio.h>
#include<stdlib.h>
#include "utils.h"

/*
Returns a multiplier for a dice number. This multiplier is used for calculating points for three of a kind.
*/

int point_multiplier(int x)
{
	if(x == 1) return 100;
	return x*10;
}

/*
Flushes hte input buffer.
*/

void flush_buffer()
{
	while((getchar())!='\n');
}

/*
Pauses until enter is pressed.
*/

void pause()
{
	printf("Press enter to continue.\n");
flush_buffer();
}

/*
Generates a (pseudo) random number.
*/

int random_number(int lower, int upper)
{
	return (rand() % (upper-lower+1))+lower;
}

/*
Rolls a dice (random(1,6)).
*/
int roll_die()
{
	return random_number(1,6);
}

/*
Prints a list of numbers. Used in this context for dice.
*/

void print_dice_array(char dice[], int num_dice)
{
	for(int i=0; i<num_dice; i++)
		printf("%d  ",dice[i]);
}