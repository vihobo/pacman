#include "Game.h"

using namespace pacman;

int main( int argc, char * args[] ) {
	//get the game instance
	Game * game = &Game::getInstance();
	//start the game
	game->run();
	return 0;
}