#include<stdio.h>
#include<ctype.h>
#include "game.h"
#include "turn.h"
#include "action.h"
#include "utils.h"
void reroll_dice(struct turn *trn)
{
	if(trn->num_remaining_dice == 0)
		trn->num_remaining_dice=6;
	for(int i=0; i<trn->num_remaining_dice; i++)
	{
		trn->dice[i]=roll_die();
	}
}
void print_dice(struct turn *trn)
{
	print_dice_array(trn->dice,trn->num_remaining_dice);
}
char action_can_roll_dice(struct turn *trn)
{
	if(trn->just_rolled)
		return false;
		return true;
}

char execute_turn(struct turn *trn)
{
	//Construct menu.
	printf("\n");
	struct action actions[num_roll_callbacks];
	int num_actions = check_roll(trn,actions);
	if(!num_actions && trn->just_rolled)
	{
		printf("%s farkled with %d points.\n",trn->player->name,trn->score);
		return false;
	}
	for(int i=0; i<num_actions; i++)
	{
		printf("%d: %s for %d points\n",i+1,actions[i].name,actions[i].points);
	}
	char can_roll=action_can_roll_dice(trn);
	if(can_roll)
		printf("r: roll dice.\n");
	printf("C: check remaining dice.\n");
	if(DEBUG)
		printf("!: instant turn end.\n");
	//Check for input.
	printf("Enter the letter or number of your selection.\n");
	char selection;
	scanf("%c",&selection);
	flush_buffer();
	if(isdigit(selection))
	{
		int ind = selection - '0';
		if(ind <= num_actions)
		{
			apply_action(trn,&actions[(ind-1)]);
			printf("Applied %d",ind);
			trn->just_rolled=false;
		}
		else
			printf("Invalid input\n");
		return 1;
	}
	else if(selection == 'r' && can_roll)
	{
		reroll_dice(trn);
		print_dice(trn);
		trn->just_rolled=true;
	}
	else if(selection == 'c')
	{
	print_dice(trn);
		return true;
	}
	else if(selection == '!' && DEBUG)
		return false;
	else
	{
		printf("Invalid input.\n");
		return true;
	}
}
