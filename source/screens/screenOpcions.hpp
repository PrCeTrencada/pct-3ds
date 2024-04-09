#pragma once
#ifndef SCREENOPCIONS_HPP
#define SCREENOPCIONS_HPP

#include <3ds.h>
#include <citro2d.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "../common/commonValues.hpp"
#include "../util/FS.hpp"
#include "../util/Graphics.hpp"
#include "../util/Http.hpp"
#include "../util/Input.hpp"
#include "../util/SpritePrinter.hpp"
#include "../util/TextPrinter.hpp"
#include "../screens/ScreenTopGeneric.hpp"
#include "../screens/ScreenBottomGeneric.hpp"

using namespace std;

class ScreenOpcions
{
public:
	ScreenOpcions(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen);
	void render();

private:
	enum OPCIONS_TEXT {

	};
	
	C3D_RenderTarget* m_topScreen, * m_bottomScreen;
	const string m_title = "Opcions de l'aplicació";

	//const size_t m_nTexts = 0;
	//const string m_texts[0] = {};
	const string m_text =
		"Encara no hi ha res a fer aquí!"
		"\n"
		"\n"
		"En versions posteriors podreu configurar diferents aspectes de l'aplicació des d'aquest menú.";

	SpritePrinter m_spritePrinter;
};

#endif // !SCREENOPCIONS_HPP
