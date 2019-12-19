#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define DICE_SLOTS 6
#define DEBUG 1

#define true 1
#define false 0

void flush_buffer()
{
	while((getchar())!='\n');
}
int random_number(int lower, int upper)
{
	return (rand() % (upper-lower+1))+lower;
}
int roll_die()
{
	return random_number(1,6);
}
struct player
{
char *name;
int score;
char winmark;
};
struct turn
{
struct player *player;
	char dice[DICE_SLOTS];
	char num_remaining_dice;
	int score;
	
};
struct action
{
	char *name;
	char dice[DICE_SLOTS];
	char num_dice;
	int points;
};
int cb_1(struct turn *trn, struct action *act)
{
	int where=0;
	for(int i=0; i<trn->num_remaining_dice; i++)
	{
		if(trn->dice[i]==1)
		{
			where=i;
			break;
		}
	}
	if(!where)
		return 0;
	act->name="One";
	act->num_dice=1;
	act->points=100;
	act->dice[0]=where;
	return 1;
}
int (*roll_callbacks[])(struct turn*, struct action*) = {*cb_1};
void reroll_dice(struct turn *trn)
{
	if(trn->num_remaining_dice == 0)
		trn->num_remaining_dice=6;
	for(int i=0; i<trn->num_remaining_dice; i++)
	{
		trn->dice[i]=roll_die();
	}
}
void print_dice_array(char dice[], int num_dice)
{
	for(int i=0; i<num_dice; i++)
		printf("%d  ",dice[i]);
}
void print_dice(struct turn *trn)
{
	print_dice_array(trn->dice,trn->num_remaining_dice);
}
char action_can_roll_dice(struct turn *trn)
{
	if(trn->num_remaining_dice == DICE_SLOTS)
		return false;
		return true;
}

char execute_turn(struct turn *trn)
{
	//Construct menu.
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
	if(selection == 'r' && can_roll)
	{
		reroll_dice(trn);
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
void loop(struct player players[], int num_players)
{
	for(int i=0; i<num_players; i++)
	{
		struct player *curr_player=&players[i];
		printf("It is %s's turn.\n",curr_player->name);
		struct turn trn;
		trn.player=curr_player;
		trn.score=0;
		for(int i=0; i<DICE_SLOTS; i++)
			trn.dice[i]=0;
		trn.num_remaining_dice=0;
		for(;;)
		{
			char cont=execute_turn(&trn);
			if(!cont)
				break;
		}
		
	}
}
int main()
{
	srand(time(0));
	//create some temp players.
	struct player p1;
	p1.name="Bob";
	struct player p2;
	p2.name="Joe";
	struct player plrs[]={p1,p2};
	loop(plrs, 2);
	
	return 0;
}