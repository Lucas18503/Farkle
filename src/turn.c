#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include "game.h"
#include "turn.h"
#include "combination.h"
#include "utils.h"

/*
Rerolls the remaining dice in the turn.
*/

void reroll_dice(struct turn *trn)
{
	if(trn->num_remaining_dice == 0)
		trn->num_remaining_dice=DICE_SLOTS;
	for(int i=0; i<trn->num_remaining_dice; i++)
	{
		trn->dice[i]=roll_die();
	}
}

/*
Prints the list of dice values to the screen.
*/

void print_dice(struct turn *trn)
{
	print_dice_array(trn->dice,trn->num_remaining_dice);
}

/*
Checks if the dice can be rolled from the turn interface.
*/

char action_can_roll_dice(struct turn *trn)
{
	if(trn->just_rolled)
		return false;
		return true;
}

/*
Checks if points can be bankd from the turn interface.
*/

char action_can_bank(struct turn *trn)
{
	if((trn->player->score<=0 && trn->score<trn->game->bank_score) || trn->score<=0)
		return false;
	return true;
}

#ifdef DEBUG
/*
Runs a single iteration of the dice configuration tool for debugging.
*/

int run_configure_dice(struct turn *trn)
{
	system("cls");
	printf("Dice roll configuration\n\n");
	printf("Dice:\n");
	for(int i=0; i<trn->num_remaining_dice; i++)
	{
		printf("%d: %d\n",(i+1),trn->dice[i]);
	}
	printf("+/-: Increese/decreese number of dice\n");
	printf("R: reset to maximum number of dice \n");
	printf("D: done\n");
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
		flush_buffer();
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
			printf("There can only be %d dice.\n",DICE_SLOTS);
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

/*
Continuously runs hte dice configuration tool.
*/

void configure_dice(struct turn *trn)
{
	for(;;)
	{
		char res=run_configure_dice(trn);
		if(!res)
			break;
	}
}
#endif
/*
Runs a single iteration of the turn interface.
*/

char execute_turn(struct turn *trn)
{
	system("cls");
	printf("It is %s's turn.\n",trn->player->name);
	if(trn->score>0)
		printf("Points saved: %d\n",trn->score);
	if(trn->num_remaining_dice>0)
	{
		printf("Remaining dice (%d): ",trn->num_remaining_dice);
		print_dice(trn);
		printf("\n");
	}
	for(int i=0; i<80; i++) printf("-");
	printf("\n");
	//Construct menu.
	struct combination combinations[num_roll_callbacks];
	int num_combinations = check_roll(trn,combinations);
	if(!num_combinations && trn->just_rolled)
	{
		printf("%s farkled with %d points.\n",trn->player->name,trn->score);
		pause();
		return false;
	}
	for(int i=0; i<num_combinations; i++)
	{
		printf("%d: %s for %d points\n",i+1,combinations[i].name,combinations[i].points);
	}
	char can_roll=action_can_roll_dice(trn);
	char can_bank=action_can_bank(trn);

	if(can_roll)
		printf("r: roll dice.\n");
	printf("S: check all player scores\n");
	printf("V: Save game\n");
	if(can_bank)
		printf("B: bank score and end turn\n");
	#ifdef DEBUG
	printf("Debug:\n");
	printf("!: instant turn end.\n");
	printf("@: Modify dice.\n");
	printf("#: Run game configuration.\n");

	#endif
	//Check for input.
	printf("Enter the letter or number of your selection.\n");
	char selection;
	scanf("%c",&selection);
	flush_buffer();
	if(isdigit(selection))
	{
		int ind = selection - '0';
		if(ind <= num_combinations)
		{
			apply_combination(trn,&combinations[(ind-1)]);
			trn->just_rolled=false;
		}
		else
			printf("Invalid input\n");
		return true;
	}
	else if(selection == 'r' && can_roll)
	{
		reroll_dice(trn);
		trn->just_rolled=true;
		return true;
	}
	else if(selection=='s')
	{
		show_scores(trn->game);
		pause();
		return true;
	}
	else if(selection == 'v')
	{
		char loc[128];
		printf("What name would you like the save to have? If you use the same name for multiple saves, the first will be overwritten with no worning.\n");
		scanf("%s",loc);
		flush_buffer();
		save_game(trn->game,loc);
		printf("Success!\n");
		pause();
	}
	else if(selection=='b' && can_bank)
	{
		trn->player->score+=trn->score;
		printf("%s banks %d points.\n",trn->player->name,trn->score);
		pause();
		return false;
	}
	#ifdef DEBUG
	else if(selection == '!')
		return false;
	else if(selection == '@')
	{
		configure_dice(trn);
		return true;
	}
	else if(selection == '#')
	{
		configure_game(trn->game);
		return true;
	}

	#endif
	else
	{
		printf("Invalid input.\n");
		return true;
	}
}
