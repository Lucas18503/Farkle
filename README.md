# Farkle

Eleventh grade Comp-sci project.

## How to play

There are many guides available online on how to play Farkle.
Basically, you roll dice and then, depending on the values you rolled, you can take different numbers of points in exchange for taking dice out of play. Three of a kind, a straight, etc.
If you cannot make any combinations, that is known as a farkle and you lose all saved points. Otherwise, when you bank your score, that gets added to your point total.

## Terms

* Combination: A group of dice that can be taken out of play for points. Examples are one, five, three ones, straight, etc.
* Player: Someone in the game. The struct contains data such as the player's name and score.
* Turn: the container for data that only applies to one player while they are playing. When farkling or banking, the turn is no longer needed because a new one will be created when the player plays again.
* Game: Container for the players and other data like minimum bank score, winning score, etc. This will get passed as part of every turn, so all data should be available where it needs to be.
* Dice group: Different than a combination; a struct that stores positions of all dice that are of the same value.
* Debug: A flag you can enable in game.h. Should be disabled in release because it provides features to cheat, like a screen to modify your dice roll.

## Compilation
### Windows

Install GCC from [here](https://gcc.gnu.org/install/binaries.html). Then, in your command prompt, run `run game`.

### Other platforms

The game may run on some of these with some modification, such as many Linux distros, but I do not have easy access to them and so cannot test.

## [License](/LICENSE)

I've put this under the Unlicense; do what you like with it.
