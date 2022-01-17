#ifndef PONG_FILE_IO
#define PONG_FILE_IO

#include <SFML/Graphics.hpp>
#include <json/json.h>

using namespace std;

Json::Value load_scores(string);
bool save_scores(std::vector<std::pair<string, int>> player_scores, string file);
#endif