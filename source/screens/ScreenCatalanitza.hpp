#pragma once
#ifndef SCREENCATALANITZA_HPP
#define SCREENCATALANITZA_HPP

#include <3ds.h>
#include <citro2d.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

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

using namespace std;
using JSON = nlohmann::json;

enum CAT_OPTION {
	CATALANITZA_HOME = 0
};

enum CAT_ACTION_RESULT {
	ACTION_RESULT_UNKNOWN = 0,
	ACTION_RESULT_INSTALL_OK,
	ACTION_RESULT_INSTALL_FAIL,
	ACTION_RESULT_UNINSTALL_OK,
	ACTION_RESULT_UNINSTALL_FAIL
};

enum CAT_ACTION {
	ACTION_UNKNOWN = -1,
	ACTION_INSTALL,
	ACTION_REINSTALL,
	ACTION_UNINSTALL
};

enum CAT_STEPS {
	STEP_1_SELECT_TRANSLATION = 0,
	STEP_2_SELECT_ACTION,
	STEP_3_PREPARE_ACTION,
	STEP_4_ACTION,
	STEP_5_RESULT
};

class ScreenCatalanitza
{
public:
	ScreenCatalanitza(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen);
	void render();

	bool updateTranslations();

	bool checkIfInstalled(size_t catalanitza);
	void updateInstalledStatus();

private:
	bool setTranslation();
	bool unsetTranslation();

	C3D_RenderTarget* m_topScreen, * m_bottomScreen;
	const string m_title = "Catalanitza les apps de la teva 3DS";

	/* Structure:
	*
	* [
	*	{
	*		"id": "",
	*		"name": "",
	*		"titleID": "",
	* 		"version": "",
	* 		"url": "",
	*		"desc": ""
	*	},
	*	...
	* ]
	*/
	JSON m_translations;
	size_t m_nTranslations;
	bool m_updated = false;

	const string m_textNoTranslations = "No s'ha pogut trobar cap traducció.\n\nReviseu la connexió a internet i intenteu-ho de nou.";
	
	vector<bool> m_installed;

	CAT_ACTION m_currentAction;
	
	const string m_actions[3] = {
		"Instal·la " CHAR_BUTTON_Y,
		"Reinstal·la " CHAR_BUTTON_Y,
		"Desinstal·la " CHAR_BUTTON_X
	};

	const string m_doingAction[3] = {
		"Baixant i instal·lant la traducció...\n\nSi us plau, espereu i no toqueu cap botó ni tanqueu la tapa de la consola. El temps d'espera dependrà de la vostra connexió a internet.",
		// Unused, but kept for consistency across the code
		"Reinstal·lant...\n\nSi us plau, espereu i no toqueu cap botó ni tanqueu la tapa de la consola.",
		"Desinstal·lant la traducció...\n\nSi us plau, espereu i no toqueu cap botó ni tanqueu la tapa de la consola."
	};

	const string m_result[5] = {
		// 0 : ACTION_RESULT_UNKNOWN
		"Error desconegut.\n\n"
		"Reinicia la consola i torna-ho a intentar.",

		// 1 : ACTION_RESULT_INSTALL_OK
		"Traducció instal·lada correctament.\n\n"
		"Reinicia la consola per aplicar els canvis.",

		// 2 : ACTION_RESULT_INSTALL_FAIL
		"Error en instal·lar la traducció.\n\n"
		"Comprova que tens connexió a internet i torna-ho a intentar.",

		// 3 : ACTION_RESULT_UNINSTALL_OK
		"Traducció desinstal·lada correctament.\n\n"
		"Reinicia la consola per aplicar els canvis.",

		// 4 : ACTION_RESULT_UNINSTALL_FAIL
		"Error en desinstal·lar la traducció.\n\n"
		"Reinicia la consola i torna-ho a intentar."
	};


	SpritePrinter m_spritePrinter;

	CAT_STEPS m_step;
	
	bool m_actionResult = false;

	const size_t m_MAX_ENTRIES = 13;
	size_t m_lower = 0, m_upper = m_MAX_ENTRIES, m_current = 0;
};

#endif // !SCREENCATALANITZA_HPP
