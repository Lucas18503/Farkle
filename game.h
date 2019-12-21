#ifndef GAME_H
#define GAME_H
#define DICE_SLOTS 6
#define DEBUG 1

#define true 1
#define false 0
#include "turn.h"
struct player
{
char *name;
int score;
char winmark;
};
struct dice_group
{
	int value; // The number all the dice in this group share in common.
	char dice_positions[DICE_SLOTS]; // The list of positions in the turn's dice array.
	int length; // The length of the dice_position array.
	
};
struct turn;
struct dice_group find_dice_by_value(int value, struct turn *trn);
void loop(struct player players[], int num_players);
#endif