#pragma once
#ifndef SCREENTOPGENERIC_HPP
#define SCREENTOPGENERIC_HPP

#include <3ds.h>
#include <citro2d.h>

#include <string>

#include "../common/specialChars.h"
#include "../common/commonValues.hpp"
#include "../util/Date.hpp"
#include "../util/TextPrinter.hpp"
#include "../util/SpritePrinter.hpp"
#include "../util/Version.hpp"

using namespace std;

enum {
	SCREEN_TOP_GENERIC_TEXT_PCT = 0
};

class ScreenTopGeneric
{
public:
	void render(SpritePrinter& spritePrinter);
	
private:
	const size_t m_nTexts = 1;
	const string m_texts[1] =
	{
		"Projecte \'Ce Trencada\'"
	};
};

#endif // !SCREENTOPGENERIC_HPP
