#ifndef PONG_MY_DRAWABLE
#define PONG_MY_DRAWABLE

#include <SFML/Graphics.hpp>

class MyDrawable : public sf::Drawable
{
public:
	MyDrawable();

	bool Enabled;
	bool Visible;

	void toggleVisible();
	void toggleEnabled();
};

#endif