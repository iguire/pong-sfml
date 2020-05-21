#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <fstream>
#include <json/json.h>

using namespace std;

sf::Vector2f PLAYER_SIZE{ 10.f,100.f };
sf::Vector2f AI_SIZE = PLAYER_SIZE;

float BALL_SIZE = 10.f;
float MARGIN_SIZE = 100.f;
float DIVIDER_SIZE = 5.f;

static string filename = "player_scores.json";

class MyDrawable : public sf::Drawable
{
public:
	MyDrawable();
	
	bool Enabled;
	bool Visible;
	
	void toggleVisible();
	void toggleEnabled();
};

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


MyDrawable::MyDrawable()
{
	Enabled = true;
	Visible = true;
}

void MyDrawable::toggleEnabled()
{
	Enabled = !Enabled;
}

void MyDrawable::toggleVisible()
{
	Visible = !Visible;
}



Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 800), player_name);
	player = new sf::RectangleShape(PLAYER_SIZE);
	ai_player = new sf::RectangleShape(AI_SIZE);
	divider = new sf::RectangleShape(sf::Vector2f(DIVIDER_SIZE, window->getSize().y));
	ball = new sf::CircleShape(BALL_SIZE);

	w_event = new sf::Event();

	entities.push_back((MyDrawable*)player);
	entities.push_back((MyDrawable*)ai_player);
	entities.push_back((MyDrawable*)divider);
	entities.push_back((MyDrawable*)ball);

	player->setFillColor(sf::Color::Green);
	ai_player->setFillColor(sf::Color::Red);
	ball->setFillColor(sf::Color::White);
	divider->setFillColor(sf::Color::White);

	divider->setPosition(sf::Vector2f(window->getSize().x / 2, 0));
	reset_players();
	reset_ball(player);

	reset_score();
	set_state(splash);
}



Game::game_states Game::get_state() const
{
	return this->GAME_STATE;
}

void Game::set_state(game_states g_state)
{
	GAME_STATE = g_state;
}

void Game::toggle_pause()
{
	if (GAME_STATE == paused)
		set_state(in_progress);
	if (GAME_STATE == in_progress)
		set_state(paused);
}

void Game::increment_player_score()
{
	player_score++;
}

void Game::increment_ai_score()
{
	ai_score++;
}

void Game::reset_game()
{
	GAME_STATE = splash;
	reset_score();
	reset_players();
	reset_ball(player);
}

void Game::reset_score()
{
	player_score = 0;
	ai_score = 0;
}

void Game::reset_ball(sf::RectangleShape* owner)
{
	ball->setPosition(owner->getPosition().x - (owner->getSize().x + ball->getRadius()), owner->getPosition().y + owner->getSize().y / 2 - ball->getRadius());
}

void Game::reset_players()
{
	player->setPosition(window->getSize().x - player->getSize().x - MARGIN_SIZE, window->getSize().y / 2 - player->getSize().y / 2);
	ai_player->setPosition(MARGIN_SIZE + ai_player->getSize().x, window->getSize().y / 2 - ai_player->getSize().y / 2);
}

void Game::handle_events()
{
	while (window->pollEvent(*w_event))
	{
		if (w_event->type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window->close();
		if (w_event->type == sf::Event::LostFocus)
			set_state(Game::paused);
	}
}

void Game::handle_input()
{
	
	//if (sf::Event::KeyPressed)
	//	switch (w_event->key.code)
	//	{
	//	case sf::Keyboard::Left:
	//		player->move(-1, 0);
	//		break;
	//	case sf::Keyboard::Right:
	//		player->move(1, 0);
	//		break;
	//	case sf::Keyboard::Up:
	//		player->move(0, -1);
	//		break;
	//	case sf::Keyboard::Down:
	//		player->move(0, 1);
	//		break;
	//	case sf::Keyboard::Space:
	//		toggle_pause();
	//	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		player->move(-1, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		player->move(1, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		player->move(0, -1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		player->move(0, 1);
	}
	
}

void Game::sync()
{
	window->clear();

	for (auto i : entities)
		window->draw(*i);
	
	window->display();
}


Json::Value load_scores(string);
bool save_scores(std::vector<std::pair<string, int>> player_scores, string file);

bool collision_detector(sf::RectangleShape* owner, sf::RenderWindow* window);



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
		myGame->handle_events();
		myGame->handle_input();
		myGame->sync();
	
	}
	return 0;
}

Json::Value load_scores(string file)
{
	std::ifstream read_scores_file(file, ios::binary);
	Json::Value player_scores;
	
	if (!read_scores_file.fail())
		read_scores_file >> player_scores;
	else
		cout << "Failed to read scores file." << endl;
	return player_scores;
}

bool save_scores(std::vector<std::pair<string,int>> player_scores, string file)
{
	std::ofstream write_scores_file(file, ios::trunc);
	Json::Value record_scores;
	
	for (int i = 0; i < player_scores.size(); i++)
	{
		record_scores["all_scores"][i]["name"] = player_scores[i].first;
		record_scores["all_scores"][i]["size"] = player_scores[i].second;
	}

	if (!write_scores_file.fail())
	{
		try
		{
			write_scores_file << record_scores;
		}
		catch (Json::Exception e)
		{
			cout << e.what() << endl;
		}
	}
	else
	{
		cout << "Failed to write to score file. Check permissions." << endl;
		return false;
	}
	return true;
}

bool collision_detector(sf::RectangleShape* owner,sf::RenderWindow* window)
{
	if ((owner->getPosition().y < 0) ||	((owner->getPosition().y + owner->getSize().y) > window->getSize().y) ||
		(owner->getPosition().x < 0) || ((owner->getPosition().x + owner->getSize().x) > window->getSize().x))
	{
		return true;
	}
	else
		return false;
}
