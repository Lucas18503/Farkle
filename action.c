#include<stdio.h>
#include "action.h"
#include "turn.h"
#include "utils.h"
int check_for_single(struct turn *trn, struct action *act, int value)
{
	int where=-1;
	struct dice_group dg=find_dice_by_value(value,trn);
	if(dg.length)
		where=dg.dice_positions[0];
	if(where<0)
		return false;
	act->name="Thing";
	act->num_dice=1;
	act->points = point_multiplier(value);
	act->dice[0]=where;
	return true;
}
int cb_1(struct turn *trn, struct action *act)
{
	return check_for_single(trn,act,1);
}
int cb_5(struct turn *trn, struct action *act)
{
	return check_for_single(trn,act,5);
}
int cb_multi(struct turn *trn, struct action *act)
{
	struct dice_group *groups = group_all_dice(trn);
	int group=-1;
	for(int i=0; i<DICE_SLOTS; i++)
	{
		if(groups[i].length>=3)
		{
			group=i;
			break;
		}
	}
	if(group<0)
		return false;

	act->name="Thing";
	for(int i=0; i<groups[group].length; i++) act->dice[i] = groups[group].dice_positions[i];
	act->num_dice=groups[group].length;
	int points=point_multiplier(groups[group].value)*10;
	for(int i=4; i<=groups[group].length; i++, points*=2);
	act->points=points;
	return true;
}
int (*roll_callbacks[])(struct turn*, struct action*) = {&cb_multi,&cb_1,&cb_5};
int num_roll_callbacks = 3;

int check_roll(struct turn *trn, struct action actions[])
{
	int ret=0;
	for(int i=0; i<num_roll_callbacks; i++)
	{
		struct action *act=&actions[ret];
		if((*roll_callbacks[i])(trn,act))
		{
			ret++;
		}
	}
	return ret;
}
void apply_action(struct turn *trn, struct action *act)
{
	for(int i=(act->num_dice-1); i>=0; i--)
	{
		trn->dice[act->dice[i]]=trn->dice[(trn->num_remaining_dice-1)];
		trn->num_remaining_dice--;
	}
	trn->score+=act->points;
	return;
}
