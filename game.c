#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include "game.h"
#include "turn.h"

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
	//roll_callbacks = {&cb_1,&cb_5};
	//num_roll_callbacks=2;  //Make sure to change this if adding new ones! This seems like a really dumb bug-causing thing that could really easily happen.
	//create some temp players.
	struct player p1={"bob",0,0};
	struct player p2={"joe",0,0};

	struct player plrs[]={p1,p2};
	loop(plrs, 2);
	
	return 0;
}