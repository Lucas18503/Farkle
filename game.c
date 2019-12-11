#include<stdio.h>
#include<stdlib.h>
#define DICE_SLOTS 6
#define DEBUG 1
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
struct player player;
	char dice[DICE_SLOTS];
	char num_remaining_dice;
	int score;
	
};
char execute_turn(struct turn trn)
{
	//Construct menu.
	printf("C: check remaining dice.\n");
	if(DEBUG)
		printf("!: instant turn end.\n");
	//Check for input.
	printf("Enter the letter or number of your selection.\n");
	char selection;
	scanf("%c",&selection);
	flush_buffer();
	if(selection == 'c')
	{
		return 1;
	}
	else if(selection == '!' && DEBUG)
		return 0;
	else
	{
		printf("Invalid input.\n");
		return 1;
	}
}
void loop(struct player players[], int num_players)
{
	for(int i=0; i<num_players; i++)
	{
		struct player curr_player=players[i];
		printf("It is %s's turn.\n",curr_player.name);
		struct turn trn;
		trn.player=curr_player	;
		trn.score=0;
		for(int i=0; i<DICE_SLOTS; i++)
			trn.dice[i]=0;
		trn.num_remaining_dice=0;
		for(;;)
		{
			char cont=execute_turn(trn);
			if(!cont)
				break;
		}
		
	}
}
int main()
{
	//create some temp players.
	struct player p1;
	p1.name="Bob";
	struct player p2;
	p2.name="Joe";
	struct player plrs[]={p1,p2};
	loop(plrs, 2);
	
	return 0;
}