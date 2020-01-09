#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
int point_multiplier(int x)
{
	if(x == 1) return 100;
	return x*10;
}
void flush_buffer()
{
	while((getchar())!='\n');
}
void pause()
{
	printf("Press enter to continue.\n");
flush_buffer();
}
int random_number(int lower, int upper)
{
	return (rand() % (upper-lower+1))+lower;
}
int roll_die()
{
	return random_number(1,6);
}
void print_dice_array(char dice[], int num_dice)
{
	for(int i=0; i<num_dice; i++)
		printf("%d  ",dice[i]);
}