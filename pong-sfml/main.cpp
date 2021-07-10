#include "game.h"

using namespace std;

int main() {


	/*Game States:
	0. Splash screen
	1. Countdown, both players stationary, one owns the ball
	2. Game in progress, ball flying, players move, score is kept
	3. Game paused
	*/

	Game* myGame = new Game();
	
	
	while (myGame->window->isOpen()) 
	{
		myGame->timeClock->restart();
		myGame->handle_events();
		myGame->handle_input();
		myGame->sync();
	}
	return 0;
}