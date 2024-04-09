#pragma once
#ifndef SCREENBOTTOMGENERIC_HPP
#define SCREENBOTTOMGENERIC_HPP

#include <3ds.h>
#include <time.h>

#include "../common/commonValues.hpp"
#include "../common/specialChars.h"
#include "../util/TextPrinter.hpp"

enum {
	SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_ABSTART = 0,
	SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_AB,
	SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_B,
	SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_ALRB_LIST,
	SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_LRB_LIST,
	SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_LRDHRB_LIST,
	SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_A_ACCEPTA,
	SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_BLANK
};

class ScreenBottomGeneric
{
public:
	ScreenBottomGeneric(const char* title, const size_t guideType);

private:
	const size_t m_nTexts = 8;
	const char* const m_texts[8] = {
		
	// (A) : Selecciona       (B) : Enrere       START : Surt //
	" " CHAR_BUTTON_A " : Selecciona       " CHAR_BUTTON_B " : Enrere       START : Surt ",
	
	// (A) : Selecciona                          (B) : Enrere //
	" " CHAR_BUTTON_A " : Selecciona                     " CHAR_BUTTON_B " : Enrere ",
	
	//                        (B) : Enrere                    //
	" " CHAR_BUTTON_B " : Enrere ",

	// (A) : Selecciona   (L) : /\    (R) : \/   (B) : Enrere //
	" " CHAR_BUTTON_A " : Selecciona     " CHAR_BUTTON_L " : " CHAR_UP_ARROW "     " CHAR_BUTTON_R " : " CHAR_DOWN_ARROW "     " CHAR_BUTTON_B " : Enrere ",

	// (L) : /\    (R) : \/   (B) : Enrere //
	" " CHAR_BUTTON_L " : " CHAR_UP_ARROW "               " CHAR_BUTTON_R " : " CHAR_DOWN_ARROW "               " CHAR_BUTTON_B " : Enrere ",

	// (L) : /\    (R) : \/   (+) : Canvia entrada   (B) : Enrere //
	" " CHAR_BUTTON_L " : " CHAR_UP_ARROW "       " CHAR_BUTTON_R " : " CHAR_DOWN_ARROW "       " CHAR_DPAD_HORIZONTAL " : Canvia       " CHAR_BUTTON_B " : Enrere ",

	// (A) : Accepta
		" " CHAR_BUTTON_A " : Accepta ",

	// Blank
		" "
	};

};

#endif // !SCREENBOTTOMGENERIC_HPP
