#include "file_io.h"
#include <iostream>
#include <fstream>

using namespace std;

static string filename = "player_scores.json";

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

bool save_scores(std::vector<std::pair<string, int>> player_scores, string file)
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

bool collision_detector(sf::RectangleShape* owner, sf::RenderWindow* window)
{
	if ((owner->getPosition().y < 0) || ((owner->getPosition().y + owner->getSize().y) > window->getSize().y) ||
		(owner->getPosition().x < 0) || ((owner->getPosition().x + owner->getSize().x) > window->getSize().x))
	{
		return true;
	}
	else
		return false;
}