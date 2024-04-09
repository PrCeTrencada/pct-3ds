#pragma once
#ifndef SCREENMAIN_HPP
#define SCREENMAIN_HPP

#include <string>

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

#include "../common/commonValues.hpp"
#include "../screens/ScreenBottomGeneric.hpp"
#include "../screens/ScreenTopGeneric.hpp"
#include "../screens/ScreenTraduccions.hpp"
#include "../screens/ScreenCatalanitza.hpp"
#include "../screens/ScreenCredits.hpp"
#include "../screens/ScreenOpcions.hpp"
#include "../util/Graphics.hpp"
#include "../util/Input.hpp"
#include "../util/SpritePrinter.hpp"

using namespace std;

class ScreenMain 
{
public:
	ScreenMain(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen);

private:
	enum MAIN_STEPS {
		STEP_1_PREPARE_PRELOAD = 0,
		STEP_2_PRELOAD,
		STEP_3_MAIN
	};
	
	enum MAIN_OPTIONS {
		SCREENMAIN_OPTION_TRADUCCIONS = 0,
		SCREENMAIN_OPTION_CATALANITZA,
		SCREENMAIN_OPTION_OPCIONS,
		SCREENMAIN_OPTION_CREDITS
	};
	
	C3D_RenderTarget* m_topScreen, * m_bottomScreen;
	const char* const m_title = "Us donem la benvinguda a PCT-3DS";
	const size_t m_nTexts = 4;
	const char* const screenMain_options[4] =
	{
		"Traduccions Comunitat \'Ce Trencada\'",
		"Catalanitza la meva 3DS",
		"Opcions",
		"Crèdits"
	};

	const string m_textAppName = "PCT-3DS";
	const string m_textLoading = "S'està iniciant l'aplicació.\nSi us plau, espereu sense tocar res.\nL'espera dependrà de la vostra connexió.";

	MAIN_STEPS m_step;

	SpritePrinter m_spritePrinter;

	bool m_entradesUpdated = false;
};

#endif // !SCREENMAIN_HPP
