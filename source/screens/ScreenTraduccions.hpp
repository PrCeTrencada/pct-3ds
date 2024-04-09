#pragma once

#ifndef SCREENTRADUCCIONS_HPP
#define SCREENTRADUCCIONS_HPP

#include <3ds.h>
#include <citro2d.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

//#include "/src/nlohmann/json.hpp"
#include <nlohmann/json.hpp>

#include "../common/commonValues.hpp"
#include "../util/FS.hpp"
#include "../util/Graphics.hpp"
#include "../util/Http.hpp"
#include "../util/Input.hpp"
#include "../util/SpritePrinter.hpp"
#include "../util/TextPrinter.hpp"
#include "../screens/ScreenTopGeneric.hpp"
#include "../screens/ScreenBottomGeneric.hpp"
#include "../screens/ScreenEntrada.hpp"

using namespace std;
using JSON = nlohmann::json;

class ScreenTraduccions
{
public:
	ScreenTraduccions(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen, bool &entradesUpdated);
	void render();
	int updateEntrades();

private:
	
	C3D_RenderTarget* m_topScreen, * m_bottomScreen;
	const string m_title = "Traduccions Comunitat \'Ce Trencada\'";

	SpritePrinter m_spritePrinter;
	
	bool *m_entradesUpdated;

	JSON m_entrades;
	size_t m_nEntrades = 0;
	
	size_t m_framesHeldUp = 0, m_framesHeldDown = 0;
	
	const size_t m_MAX_ENTRIES = 13;
	size_t m_lower = 0, m_upper = m_MAX_ENTRIES, m_current = 0;
};

#endif // !SCREENTRADUCCIONS_HPP
