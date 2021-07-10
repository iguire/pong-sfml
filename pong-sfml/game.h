#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "file_io.h"
#include "my_drawable.h"

using namespace std;

class Game
{
public:
	Game();

	const enum game_states { paused, in_progress, splash };

	game_states get_state() const;
	void set_state(game_states);
	void toggle_pause();


	void increment_player_score();
	void increment_ai_score();
	void reset_game();

	void handle_events();
	void handle_input();
	void sync();

	sf::RenderWindow* window;
	sf::Event* w_event;
	sf::Clock* timeClock;
	sf::RectangleShape* player;
	sf::RectangleShape* ai_player;
	sf::RectangleShape* divider;
	sf::CircleShape* ball;

	//wait until all entities are initialized
	//before assigning pointers!
	vector<MyDrawable*> entities;

private:
	game_states GAME_STATE;
	string player_name = "player1";
	int player_score = 0;
	int ai_score = 0;

	void reset_score();
	void reset_ball(sf::RectangleShape* owner);
	void reset_players();
};
#endif