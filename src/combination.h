#ifndef COMBINATION_H
#define COMBINATION_H
#include "turn.h"

struct combination
{
	char name[500];
	char dice[DICE_SLOTS];
	char num_dice;
	int points;
};
int check_for_single(struct turn *trn, struct combination *cb, int value);
int cb_1(struct turn *trn, struct combination *cb);
int cb_5(struct turn *trn, struct combination *cb);
int cb_multi(struct turn *trn, struct combination *cb);
int check_for_similar_groups(struct turn *trn, struct combination *cb, int required_length, int required_num_groups);
int cb_three_pair(struct turn *trn, struct combination *cb);
int cb_two_triplets(struct turn *trn, struct combination *cb);

extern int (*roll_callbacks[])(struct turn*, struct combination*);
extern int num_roll_callbacks;
int check_roll(struct turn *trn, struct combination combinations[]);
void apply_combination(struct turn *trn, struct combination *cb);

#endif