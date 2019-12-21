#ifndef ACTION_H
#define ACTION_H
#include "turn.h"

struct action
{
	char *name;
	char dice[DICE_SLOTS];
	char num_dice;
	int points;
};
int check_for_single(struct turn *trn, struct action *act, int value);
int cb_1(struct turn *trn, struct action *act);
int cb_5(struct turn *trn, struct action *act);
extern int (*roll_callbacks[])(struct turn*, struct action*);
extern int num_roll_callbacks;
int check_roll(struct turn *trn, struct action actions[]);
void apply_action(struct turn *trn, struct action *act);

#endif