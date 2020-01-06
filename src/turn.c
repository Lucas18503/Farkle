#include<stdio.h>
#include<ctype.h>
#include "game.h"
#include "turn.h"
#include "action.h"
#include "utils.h"
void reroll_dice(struct turn *trn)
{
	if(trn->num_remaining_dice == 0)
		trn->num_remaining_dice=DICE_SLOTS;
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
int run_configure_dice(struct turn *trn)
{
	printf("Dice:\n");
	for(int i=0; i<trn->num_remaining_dice; i++)
	{
		printf("%d: %d\n",(i+1),trn->dice[i]);
	}
	printf("+/-: Increese/decreese number of dice\n");
	printf("R: reset to maximum number of dice \n");
	printf("D: done");
	printf("Enter the letter or number of your selection.\n");
	char selection;
	scanf("%c",&selection);
	flush_buffer();
	if(isdigit(selection))
	{
		int ind = selection - '0';
		if(ind <= trn->num_remaining_dice)
		{
		printf("Enter the new dice value.\n");
		int value;
		scanf("%i",&value);
		if(value >= 1 && value <= 6)
		{
			trn->dice[(ind-1)]=value;
			flush_buffer();
		}
		else
		{
			printf("Invalid input\n");
		}
		}
		else
			printf("Invalid input\n");
		return true;
	}
	else if(selection == '+')
	{
		if(trn->num_remaining_dice < DICE_SLOTS)
		{
			trn->num_remaining_dice++;
		}
		else
			printf("There can only be %d dice.",DICE_SLOTS);
	}
	else if(selection == '-')
	{
		if(trn->num_remaining_dice > 0)
		{
			trn->num_remaining_dice--;
		}
		else
			printf("There can be no fewer dice.\n");
	}
	else if(selection == 'r')
	{
		trn->num_remaining_dice=DICE_SLOTS;
		return true;
	}
	else if(selection == 'd')
	{
		return false;
	}
	else
	{
		printf("Invalid input\n");
		return true;
	}
}
void configure_dice(struct turn *trn)
{
	for(;;)
	{
		char res=run_configure_dice(trn);
		if(!res)
			break;
	}
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
	{
		printf("Debug:\n");
		printf("!: instant turn end.\n");
		printf("@: Modify dice.\n");
	}
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
	else if(selection == '@' && DEBUG)
	{
		configure_dice(trn);
	}
	else
	{
		printf("Invalid input.\n");
		return true;
	}
}
