#include<stdio.h>
#include<string.h>
#include "bl_number_to_words.h"
#include "combination.h"
#include "turn.h"
#include "utils.h"

/*
Checks for the presence of a single die in a roll.
The combination argument cb can be considered an out value, it will only be written to.
*/

int check_for_single(struct turn *trn, struct combination *cb, int value)
{
	int where=-1;
	struct dice_group dg=find_dice_by_value(value,trn);
	if(dg.length)
		where=dg.dice_positions[0];
	if(where<0)
		return false;
	char name[500];
	bl_number_to_words(value,name,500);
	strcpy(cb->name, name);

	cb->num_dice=1;
	cb->points = point_multiplier(value);
	cb->dice[0]=where;
	return true;
}

/*
Checks for a certain number of groupings (required_num_groups) of a certain length (required_length).
Given roll of (3,4,3,4,3,4): check_for_similar_groups(trn, cb, 3, 2) => (3,3,3), (4,4,4)
*/

int check_for_similar_groups(struct turn *trn, struct combination *cb, int required_length, int required_num_groups)
{
	struct dice_group *groups = group_all_dice(trn);
	int group_indexes[DICE_SLOTS];
	int num_groups=0;
	for(int i=0; i<DICE_SLOTS; i++)
	{
		if(groups[i].length==required_length)
		{
			group_indexes[num_groups]=i;
		num_groups++;
		}
	}
	if(num_groups!=required_num_groups)
		return false;
	cb->num_dice=0;
	for(int i=0; i<num_groups; i++)
	{
		struct dice_group *curr_group=&groups[group_indexes[i]];
		for(int j=0; j<curr_group->length; j++)
		{
			cb->dice[cb->num_dice]=curr_group->dice_positions[j];
			cb->num_dice++;
		}
	}
	return true;
}
int cb_1(struct turn *trn, struct combination *cb)
{
	return check_for_single(trn,cb,1);
}
int cb_5(struct turn *trn, struct combination *cb)
{
	return check_for_single(trn,cb,5);
}
int cb_multi(struct turn *trn, struct combination *cb)
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
	char first[500], second[500];
	bl_number_to_words(groups[group].length,first,500);
	bl_number_to_words(groups[group].value,second,500);
sprintf(cb->name,"%s %ss",first,second);
	for(int i=0; i<groups[group].length; i++) cb->dice[i] = groups[group].dice_positions[i];
	cb->num_dice=groups[group].length;
	int points=point_multiplier(groups[group].value)*10;
	for(int i=4; i<=groups[group].length; i++, points*=2);
	cb->points=points;
	return true;
}
int cb_three_pair(struct turn *trn, struct combination *cb)
{
	int res=check_for_similar_groups(trn,cb,2,3);
	if(res==false) return false;
	cb->points=1500;
	strcpy(cb->name,"Three pair");
return true;
}
int cb_two_triplets(struct turn *trn, struct combination *cb)
{
	int res=check_for_similar_groups(trn,cb,3,2);
	if(res==false) return false;
	cb->points=2500;
	strcpy(cb->name,"Two triplets");
return true;
}
int cb_straight(struct turn *trn, struct combination *cb)
{
	int res=check_for_similar_groups(trn,cb,1,6);
	if(res==false) return false;
	cb->points=1500;
	strcpy(cb->name,"Straight");
return true;
}

int (*roll_callbacks[])(struct turn*, struct combination*) = {&cb_straight,&cb_three_pair,&cb_two_triplets,&cb_multi,&cb_1,&cb_5};
int num_roll_callbacks = 6;

/*
Returns an array of possible combinations for a certain roll.
The combination argument combinations can be considered an out value, and an array.
*/

int check_roll(struct turn *trn, struct combination combinations[])
{
	int ret=0;
	for(int i=0; i<num_roll_callbacks; i++)
	{
		struct combination *cb=&combinations[ret];
		if((*roll_callbacks[i])(trn,cb))
		{
			ret++;
		}
	}
	return ret;
}

/*
Applies a combination to a turn. Applies dice changes and score changes.
*/

void apply_combination(struct turn *trn, struct combination *cb)
{
	for(int i=(cb->num_dice-1); i>=0; i--)
	{
		trn->dice[cb->dice[i]]=trn->dice[(trn->num_remaining_dice-1)];
		trn->num_remaining_dice--;
	}
	trn->score+=cb->points;
	return;
}
