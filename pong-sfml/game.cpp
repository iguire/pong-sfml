#include "game.h"
#include "entity_manager.h"

using namespace std;

sf::Vector2f PLAYER_SIZE{ 10.f,100.f };
sf::Vector2f AI_SIZE = PLAYER_SIZE;

float BALL_SIZE = 10.f;
float MARGIN_SIZE = 100.f;
float DIVIDER_SIZE = 5.f;
int PLAYER_SPEED = 5000;

Game::Game()
{

	// init named drawables
	window = new sf::RenderWindow(sf::VideoMode(800, 800), player_name);
	player = new sf::RectangleShape(PLAYER_SIZE);
	ai_player = new sf::RectangleShape(AI_SIZE);
	divider = new sf::RectangleShape(sf::Vector2f(DIVIDER_SIZE, window->getSize().y));
	ball = new sf::CircleShape(BALL_SIZE);
	timeClock = new sf::Clock();
	w_event = new sf::Event();

	// init drawable entity vector
	drawable_mgr = EntityManager();

	// populate vector with entities
	drawable_mgr.entities.push_back((MyDrawable*)player);
	drawable_mgr.entities.push_back((MyDrawable*)ai_player);
	drawable_mgr.entities.push_back((MyDrawable*)divider);
	drawable_mgr.entities.push_back((MyDrawable*)ball);


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
	sf::Time deltaT = timeClock->getElapsedTime();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		player->move(-1 * PLAYER_SPEED * deltaT.asSeconds(), 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		player->move(1 * PLAYER_SPEED * deltaT.asSeconds(), 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		player->move(0, -1 * PLAYER_SPEED * deltaT.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		player->move(0, 1 * PLAYER_SPEED * deltaT.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		toggle_pause();
}

void Game::sync()
{
	window->clear();

	for(auto i : drawable_mgr.entities)
		if (i->Enabled && i->Visible)
			window->draw(*i);

	window->display();
}

bool collision_detector(sf::RectangleShape* c_shape, sf::RenderWindow* window)
{
	if ((c_shape->getPosition().y < 0) || ((c_shape->getPosition().y + c_shape->getSize().y) > window->getSize().y) ||
		(c_shape->getPosition().x < 0) || ((c_shape->getPosition().x + c_shape->getSize().x) > window->getSize().x))
	{
		return true;
	}
	else
		return false;
}