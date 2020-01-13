#ifndef GAME_H
#define GAME_H
#define DICE_SLOTS 6
#define DEBUG 1
#define MAX_PLAYERS 8

#define V_HOT_DICE_ROLL 1
#define V_THREE_FARKLES 2
#define V_TOXIC_TWOS 4
#define V_HIGH_STAKES 8
#define V_WELLFAIR 16

#define true 1
#define false 0
#include "turn.h"
struct player
{
	char name[100];
	int score;
	char winmark;
};
struct dice_group
{
	int value; // The number all the dice in this group share in common.
	char dice_positions[DICE_SLOTS]; // The list of positions in the turn's dice array.
	int length; // The length of the dice_position array.
	
};
struct game
{
	struct player players[MAX_PLAYERS];
	int num_players;
	int winning_score;
	int bank_score;
	
};
struct turn;
struct dice_group find_dice_by_value(int value, struct turn *trn);
struct dice_group *group_all_dice(struct turn *trn);
int compare_scores(const void *p1, const void *p2); // These are typecast to players.
void show_scores(struct game *game);
int run_configure_game(struct game *game);
void configure_game(struct game *game);
void turn_loop(struct game *game);
void game_loop(struct game *game);
#endif