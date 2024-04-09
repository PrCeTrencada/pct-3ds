#pragma once

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>
#include <citro2d.h>

#include "../common/commonValues.hpp"

namespace Graphics
{
	void startFrame(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen);
	void activeScreen(C3D_RenderTarget* screen);
	void endFrame();
};

#endif // !GRAPHICS_HPP
