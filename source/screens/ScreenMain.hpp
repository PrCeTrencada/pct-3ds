#pragma once
#ifndef SCREENMAIN_HPP
#define SCREENMAIN_HPP

#include <string>

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

#include <nlohmann/json.hpp>

#include "../common/commonValues.hpp"
#include "../screens/ScreenBottomGeneric.hpp"
#include "../screens/ScreenTopGeneric.hpp"
#include "../screens/ScreenTraduccions.hpp"
#include "../screens/ScreenCatalanitza.hpp"
#include "../screens/ScreenCredits.hpp"
#include "../screens/ScreenOpcions.hpp"
#include "../util/FS.hpp"
#include "../util/Graphics.hpp"
#include "../util/Input.hpp"
#include "../util/Title.hpp"
#include "../util/SpritePrinter.hpp"
#include "../util/Version.hpp"

using namespace std;
using JSON = nlohmann::json;

class ScreenMain 
{
public:
	ScreenMain(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen);

private:
	string checkForUpdate();
	
	enum MAIN_STEPS {
		STEP_1_PREPARE_PRELOAD = 0,
		STEP_2_PRELOAD,
		STEP_3_UPDATE_AVAILABLE,
		STEP_4_PREPARE_UPDATE,
		STEP_5_UPDATE,
		STEP_6_UPDATE_RESULT,
		STEP_7_MAIN
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
	const string m_textUpdateCheck = "Cercant actualitzacions de l'aplicació...";
	const string m_textUpdateAvailable[2] = {
		"Actualització disponible",
		"Si us plau, confirmeu-ne la instal·lació\npremeu START"
	};
	const string m_textUpdating = "S'està actualitzant l'aplicació.\nSi us plau, espereu sense tocar res.";
	const string m_textUpdateSuccess = "L'actualització s'ha completat.\nEs reiniciarà l'aplicació.";
	const string m_textUpdateFail = "No s'ha pogut actualitzar l'aplicació.\nSi us plau, torneu a intentar-ho més tard.";
	const string m_textUpdateResultConfirm = "Premeu START per continuar";

	JSON m_latestVersion;

	MAIN_STEPS m_step;

	SpritePrinter m_spritePrinter;

	bool m_entradesUpdated = false;
};

#endif // !SCREENMAIN_HPP
