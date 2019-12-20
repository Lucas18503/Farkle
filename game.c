#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
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
	char just_rolled;
};
struct action
{
	char *name;
	char dice[DICE_SLOTS];
	char num_dice;
	int points;
};
struct dice_group
{
	int value; // The number all the dice in this group share in common.
	char dice_positions[DICE_SLOTS]; // The list of positions in the turn's dice array.
	int length; // The length of the dice_position array.
	
};
struct dice_group find_dice_by_value(int value, struct turn *trn)
{
	struct dice_group ret;
	ret.value=value;
	ret.length=0;
	for(int i=0; i<trn->num_remaining_dice; i++)
	{
		if(trn->dice[i]==value)
		{
			ret.dice_positions[ret.length]=i;
			ret.length++;
		}
	}
	return ret;
}
int cb_1(struct turn *trn, struct action *act)
{
	int where=-1;
	struct dice_group dg=find_dice_by_value(1,trn);
	if(dg.length)
		where=dg.dice_positions[0];
	if(where<0)
		return false;
	act->name="One";
	act->num_dice=1;
	act->points=100;
	act->dice[0]=where;
	return true;
}
int (*roll_callbacks[])(struct turn*, struct action*) = {&cb_1};
int num_roll_callbacks=1; //Make sure to change this if adding new ones! This seems like a really dumb bug-causing thing that could really easily happen.
int check_roll(struct turn *trn, struct action actions[])
{
	int ret=0;
	for(int i=0; i<num_roll_callbacks; i++)
	{
		struct action *act=&actions[i];
		if((*roll_callbacks[i])(trn,act))
		{
			ret++;
		}
	}
	return ret;
}
void apply_action(struct turn *trn, struct action *act)
{
	for(int i=0; i<act->num_dice; i++)
	{
		trn->dice[act->dice[i]]=trn->dice[(trn->num_remaining_dice-1)];
		trn->num_remaining_dice--;
	}
	trn->score+=act->points;
	return;
}
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
		trn.just_rolled=false;
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