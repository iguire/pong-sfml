#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER

#include <vector>
#include "my_drawable.h"

using namespace std;

class EntityManager
{
	friend class Game;
	vector<MyDrawable*> entities;
};

#endif