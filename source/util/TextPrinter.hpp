#pragma once

#ifndef TEXTPRINTER_HPP
#define TEXTPRINTER_HPP

#include <3ds.h>
#include <citro2d.h>
#include "../common/commonValues.hpp"

#include <stdlib.h>

namespace TextPrinter
{
	void print(const char* text, u32 flags, float x, float y, float z, float scaleX, float scaleY, u32 _C2D_Color32f);
	bool print(const char* text, u32 flags, float x, float y, float z, float scaleX, float scaleY, u32 _C2D_Color32f, float wrap);
};

#endif // !TEXTPRINTER_HPP
