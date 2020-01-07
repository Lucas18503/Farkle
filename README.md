# Farkle
Eleventh grade Comp-sci project.
## Why?
I like Farkle.
## Terms
* Combination: A group of dice that can be taken out of play for points. Examples are one, five, three ones, straight, etc.
* Player: Someone in the game. The struct contains data like the player's name and score, and other misc stuff.
* Turn: the container for data that only applies to one player while they are playing. When farkling or banking, the turn is no longer needed because a new one will be created when the player plays again.
* Game: Container for the players and other data like minimum bank score, winning score, etc. This will get passed as part of every turn, so all data should be available where it needs to be.
* Dice group: Different than a combination; a struct that stores positions of all dice that are of the same value.
* Debug: A flag you can enable in game.h. Should be disabled in release because it provides features to cheat, like a screen to modify your dice roll.
## Compilation
### Windows
In your command line, run `run game'. Alternatively look at run.bat and compile it yourself.
### MacOS/Linux/iOS/Android/Dos/Apple2/NES/Atari
Good luck. Actually the game may run on some of these, but I do not own any of them and so cannot test.
## Contact me
Don't.