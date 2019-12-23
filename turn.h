#ifndef TURN_H
#define TURN_H
#include "game.h"
struct turn
{
	struct game *game;
struct player *player;
	char dice[DICE_SLOTS];
	char num_remaining_dice;
	int score;
	char just_rolled;
};
void reroll_dice(struct turn *trn);
void print_dice(struct turn *trn);
char action_can_roll_dice(struct turn *trn);
char execute_turn(struct turn *trn);
#endif