#include "my_drawable.h"

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