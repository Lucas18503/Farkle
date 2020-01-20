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

/*
Returns a group of dice that are the same value.
*/

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
/*
Returns an array of length 6 (or DICE_SLOTS), containing all dice grouped.
*/

struct dice_group *group_all_dice(struct turn *trn)
{
	static struct dice_group ret[DICE_SLOTS];
	for(int i=0; i<DICE_SLOTS; i++)
	{
ret[i]=find_dice_by_value(i+1,trn);
	}
	return ret;
}
int compare_scores(const void *p1, const void *p2) // These are typecast ot players.
{
	return (((struct player *)p2)->score-((struct player *)p1)->score);
}

/*
Prints the scores of all players.
*/

void show_scores(struct game *game)
{
	struct player players[game->num_players];
	for(int i=0; i<game->num_players; i++) players[i]=game->players[i];
	qsort(players,game->num_players,sizeof(struct player),compare_scores);
	for(int i=0; i<game->num_players; i++) printf("%s: %d points.\n",players[i].name,players[i].score);
}

/*
Shows the game configuration screen. Does not loop.
*/

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
	printf("D: done\n");
	printf("Enter the letter or number of your selection.\n");
	char selection;
	scanf("%c",&selection);
	flush_buffer();
	if(isdigit(selection))
	{
		int ind = selection - '0';
		if(ind <= game->num_players)
		{
		printf("Enter %s's new name.\n",game->players[(ind-1)].name);
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
			game->players[game->num_players].score=0;
			game->num_players++;
		}
		else
			printf("There can only be %d players in this game.\n",MAX_PLAYERS);
	}
	else if(selection == '-')
	{
		if(game->num_players > 2)
		{
			game->num_players--;
		}
		else
			printf("There can only be a minimum of 2 players in this game.\n");
	}
	else if(selection=='w')
	{
		printf("New winning score (more than 50):\n");
		int ws;
		scanf("%d",&ws);
		if(ws<50)
		{
			printf("Invalid input.\n");
			pause();
			return true;
		}
		game->winning_score=ws;
		return true;
	}
	else if(selection=='b')
	{
		printf("New minimum bank score (more than 50):\n");
		int ws;
		scanf("%d",&ws);
		if(ws<50)
		{
			printf("Invalid input.\n");
			pause();
			return true;
		}
		game->bank_score=ws;
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
Runs the game configuration screen until it is closed from user input.
*/

void configure_game(struct game *game)
{
	for(;;)
	{
		char res=run_configure_game(game);
		if(!res)
			break;
	}
}

/*
Constructs a turn object and loops for input until the turn is finished.
*/

void turn_loop(struct game *game)
{
	struct turn trn;
	trn.game=game;
	trn.player=&game->players[game->current_player];
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

/*
The main game loop. Handles winning and turn looping.
*/

void game_loop(struct game *game)
{
	for(;;)
	{
		for(; game->current_player<game->num_players; game->current_player++)
		{
		if(game->current_player==game->winner)
		{
			printf("%s won the game.\n",game->players[game->winner].name);
			printf("Final scores:\n");
			show_scores(game);
			pause();
			return;
		}
			turn_loop(game);
			if((game->winner<0&&game->players[game->current_player].score>=game->winning_score)||(game->players[game->current_player].score>game->players[game->winner].score))
			{
				game->winner=game->current_player;
				printf("%s has reached the winning score. All other players will now have an opportunity to exceed this score.\n",game->players[game->winner].name);
				pause();
			}
		}
		game->current_player=0;
	}
}

void new_game()
{
	struct game game;
	game.num_players=2;
	for(int i=0; i<game.num_players; i++)
	{
		sprintf(game.players[i].name, "Player %d",(i+1));
		game.players[i].score=0;
	}
	game.current_player=0;
	game.winner=-1;
	game.winning_score=10000;
	game.bank_score=500;
	configure_game(&game);
	game_loop(&game);
}
int main()
{
	srand(time(0));
	for(;;)
	{
		system("cls");
		printf("Farkle\nBy Lucas Brown\n\n");
		printf("1: Start game\n");
		printf("2: Load game\n");
		printf("0: Exit\n");
		printf("Choose an option.\n");
		int selection;
		scanf("%d",&selection);
		if(selection == 0)
		{
			break;
		}
		else if(selection == 1)
		{
			new_game();
		}
		else if(selection == 2)
		{
			//load_game();
		}
		else
		{
			printf("Invalid selection.\n");
			pause();
		}
	}
	return 0;
}