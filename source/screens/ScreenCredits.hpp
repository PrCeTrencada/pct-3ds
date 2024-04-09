#pragma once

#ifndef SCREENCREDITS_HPP
#define SCREENCREDITS_HPP

#include <3ds.h>
#include <citro2d.h>

#include "../common/commonValues.hpp"
#include "../common/specialChars.h"
#include "../util/Input.hpp"
#include "../util/Graphics.hpp"
#include "../util/SpritePrinter.hpp"
#include "../util/TextPrinter.hpp"
#include "../screens/ScreenBottomGeneric.hpp"
#include "../screens/ScreenTopGeneric.hpp"

class ScreenCredits
{
public:
	ScreenCredits(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen);

private:
	C3D_RenderTarget* m_topScreen, * m_bottomScreen;
	const char* const m_title = "Crèdits";
	const char* const m_text =
	{
		"- Nil Caballero i Milà: \n"
		"Desenvolupament de l\'aplicació.\n"
		"\n"
		"- Luna Caoimhe: \n"
		"Extracció de textos i traducció de les apps.\n"
		"\n"
		" - Projecte 'Ce Trencada':\n"
		"Coordinació i suport tecnicolingüístic.\n"
		"\n"
		"\n"
		"Podeu consultar tots els detalls del projecte \'PCT-3DS\' al repositori de GitHub:\n"
		"https://github.com/PrCeTrencada/pct-3ds"
	};

	SpritePrinter m_spritePrinter;
};

#endif // !SCREENCREDITS_HPP
