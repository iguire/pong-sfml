#ifndef PONG_FILE_IO
#define PONG_FILE_IO

#include <json/json.h>
#include <SFML/Graphics.hpp>

using namespace std;

Json::Value load_scores(string);
bool save_scores(std::vector<std::pair<string, int>> player_scores, string file);

bool collision_detector(sf::RectangleShape* owner, sf::RenderWindow* window);

#endif