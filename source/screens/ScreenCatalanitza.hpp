#pragma once
#ifndef SCREENCATALANITZA_HPP
#define SCREENCATALANITZA_HPP

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

	bool checkIfInstalled(size_t catalanitza);
	void updateInstalledStatus();

private:
	bool setTranslation();
	bool unsetTranslation();

	C3D_RenderTarget* m_topScreen, * m_bottomScreen;
	const string m_title = "Catalanitza les apps de la teva 3DS";

	const size_t m_nTexts = 1;
	const string m_texts[1] = {
		"Menú Home (menú d'inici)"
	};

	const string m_desc[1] = {
		// 0 : Menú Home
		"El menú d'inici de la consola, el primer lloc on apareixes en encendre-la. És el lloc des del qual s'accedeix a la configuració, aplicacions i jocs instal·lats.\n\n"
		"És compatible amb qualsevol de les llengües del sistema."

		// 1 : Configuració de la consola
		//"L'aplicació del sistema des de la qual es gestiona la configuració de molts aspectes tals com la connexió Wi-Fi, les restriccions de control parental, la calibració de diversos elements de la consola i demés.\n\n"
		//"És compatible amb qualsevol de les llengües del sistema."
	};

	const string m_titleId[1] = {
		// 0 : Menú Home
		"0004003000009802"
	};

	bool m_installed[1] = { false };

	CAT_ACTION m_currentAction;
	
	const string m_actions[3] = {
		CHAR_BUTTON_Y " : Instal·la la traducció",
		CHAR_BUTTON_Y " : Reinstal·la la traducció",
		CHAR_BUTTON_X " : Desinstal·la la traducció"
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
