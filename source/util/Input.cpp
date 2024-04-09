#include "Input.hpp"

void Input::start()
{
	hidScanInput();
}

bool Input::isPressed(const int key)
{
	return (hidKeysDown() & key);
}

bool Input::isHeld(const int key)
{
	return (hidKeysHeld() & key);
}

bool Input::isReleased(const int key)
{
	return (hidKeysUp() & key);
}
