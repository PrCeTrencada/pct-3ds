#pragma once

#ifndef INPUT_HPP
#define INPUT_HPP

#include <3ds.h>
#include <citro2d.h>

namespace Input
{
	void start();
	bool isPressed(const int key);
	bool isHeld(const int key);
	bool isReleased(const int key);
};

#endif // !INPUT_HPP
