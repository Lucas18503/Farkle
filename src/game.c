#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include<string.h>
#define BL_NUMWORDS_IMPLEMENTATION
#include "bl_number_to_words.h"
#include "utils.h"
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
struct dice_group *group_all_dice(struct turn *trn)
{
	static struct dice_group ret[DICE_SLOTS];
	for(int i=0; i<DICE_SLOTS; i++)
	{
ret[i]=find_dice_by_value(i+1,trn);
	}
	return ret;
}

int run_configure_game(struct game *game)
{
	printf("Player names:\n");
	for(int i=0; i<game->num_players; i++)
	{
		printf("%d: %s\n",(i+1),game->players[i].name);
	}
	printf("+/-: Increese/decreese number of players\n");
	printf("W: Edit winning score (currently %d)\n",game->winning_score);
	printf("B: Edit initial bank score (currently %d)\n",game->bank_score);
	printf("D: done");
	printf("Enter the letter or number of your selection.\n");
	char selection;
	scanf("%c",&selection);
	flush_buffer();
	if(isdigit(selection))
	{
		int ind = selection - '0';
		if(ind <= game->num_players)
		{
		printf("Enter %s's new name.",game->players[(ind-1)].name);
		scanf("%s",game->players[(ind-1)].name);
		flush_buffer();
		}
		else
			printf("Invalid input\n");
		return true;
	}
	else if(selection == '+')
	{
		if(game->num_players < MAX_PLAYERS)
		{
			sprintf(game->players[game->num_players].name,"Player %d", (game->num_players+1));
			game->num_players++;
		}
		else
			printf("There can only be %d players in this game.",MAX_PLAYERS);
	}
	else if(selection == '-')
	{
		if(game->num_players > 2)
		{
			game->num_players--;
		}
		else
			printf("There can only be a minimum of 2 players in this game.");
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
void configure_game(struct game *game)
{
	for(;;)
	{
		char res=run_configure_game(game);
		if(!res)
			break;
	}
}
void turn_loop(struct game *game)
{
	for(int i=0; i<game->num_players; i++)
	{
		struct player *curr_player=&game->players[i];
		printf("It is %s's turn.\n",curr_player->name);
		struct turn trn;
		trn.game=game;
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
void game_loop(struct game *game)
{
	for(int i=1;;i++)
	{
		printf("Turn %d\n",i);
		turn_loop(game);
	}
}
int main()
{
	srand(time(0));
	//create a game.
	struct game game;
	game.num_players=2;
	strcpy(game.players[0].name, "Player 1");
	strcpy(game.players[1].name, "Player 2");
	game.winning_score=10000;
	game.bank_score=500;
	configure_game(&game);
	game_loop(&game);
	
	return 0;
}