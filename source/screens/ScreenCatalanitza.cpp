#include "ScreenCatalanitza.hpp"

ScreenCatalanitza::ScreenCatalanitza(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen) :
	m_topScreen(topScreen), m_bottomScreen(bottomScreen)
{
	m_spritePrinter = SpritePrinter();
}

void ScreenCatalanitza::render()
{
	m_spritePrinter.start();

	if (!m_updated || m_nTranslations == 0)
		updateTranslations();
	
	updateInstalledStatus();

	m_step = STEP_1_SELECT_TRANSLATION;
	m_currentAction = ACTION_UNKNOWN;
	size_t res = ACTION_RESULT_UNKNOWN;

	bool exitLoop = false;

	// Main program loop
	while (true)
	{
		Input::start();

		Graphics::startFrame(m_topScreen, m_bottomScreen);

		m_spritePrinter.startFrame();



		// Top screen (400x240)

		Graphics::activeScreen(m_topScreen);

		ScreenTopGeneric().render(m_spritePrinter);



		// Bottom screen (320x240)

		Graphics::activeScreen(m_bottomScreen);


		switch (m_step)
		{


		case STEP_1_SELECT_TRANSLATION:
			if (m_nTranslations == 0)
			{
				ScreenBottomGeneric(m_title.c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_B);

				TextPrinter::print(m_textNoTranslations.c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, FONT_HEIGHT_STD * 5, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);
			}
			else
			{
				ScreenBottomGeneric(m_title.c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_AB);

				for (size_t i = m_lower; i < m_upper; i++)
				{
					if (i >= m_nTranslations)
						break;

					u32 textColor = (m_installed[i]) ? C2D_Color32(COLOR_GREEN) : C2D_Color32(COLOR_RED);

					if (i == m_current)
					{
						//textColor = C2D_Color32(COLOR_BLACK);
						textColor = (m_installed[i]) ? C2D_Color32(COLOR_GREEN_DARK) : C2D_Color32(COLOR_RED_DARK);
						C2D_DrawRectSolid(0.0f, FONT_HEIGHT_STD * (i - m_lower + 1), 0.0f, BOTTOM_SCREEN_WIDTH, FONT_HEIGHT_STD, C2D_Color32(0xD3, 0xD3, 0xD3, 0xFF));
					}

					TextPrinter::print(string(m_translations[i]["name"]).c_str(), C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * (i - m_lower + 1), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, textColor);
				}

				if (Input::isPressed(KEY_A))
					m_step = STEP_2_SELECT_ACTION;

				if (Input::isPressed(KEY_UP) || Input::isPressed(KEY_ZL))
				{
					if (m_current > 0)
					{
						m_current--;

						if (m_current == m_lower && m_lower > 0)
						{
							m_lower--;
							m_upper--;
						}
					}
					else if (m_current == 0)
					{
						// Set to the end of the list
						m_current = m_nTranslations - 1;
						m_lower = m_nTranslations - m_MAX_ENTRIES;
						m_upper = m_nTranslations;
					}
				}
				else if (Input::isPressed(KEY_DOWN) || Input::isPressed(KEY_ZR))
				{
					if (m_current < m_nTranslations - 1)
					{
						m_current++;

						if (m_current == m_upper - 1 && m_upper < m_nTranslations)
						{
							m_lower++;
							m_upper++;
						}
					}
					else if (m_current == m_nTranslations - 1)
					{
						// Set to the start of the list
						m_current = 0;
						m_lower = 0;
						m_upper = m_MAX_ENTRIES;
					}
				}
			}

			if (Input::isPressed(KEY_B))
			{
				exitLoop = true;
				break;
			}
			break;



		case STEP_2_SELECT_ACTION:
			ScreenBottomGeneric(string(m_translations[m_current]["name"]).c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_B);

			TextPrinter::print(string(m_translations[m_current]["desc"]).c_str(), C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * 1, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

			if (m_installed[m_current])
			{
				// ESTAT
				string state = "Estat: INSTAL·LADA";
				TextPrinter::print(state.c_str(), C2D_AlignLeft, FONT_MARGIN, BOTTOM_SCREEN_HEIGHT - (FONT_HEIGHT_STD * 3), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_GREEN), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

				// VERSION
				if (!string(m_translations[m_current]["version"]).empty())
				{
					TextPrinter::print(string(m_translations[m_current]["version"]).c_str(), C2D_AlignLeft, FONT_MARGIN, BOTTOM_SCREEN_HEIGHT - (FONT_HEIGHT_STD * 2), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_YELLOW));
				}

				// REINSTALL
				TextPrinter::print(m_actions[ACTION_REINSTALL].c_str(), C2D_AlignRight, BOTTOM_SCREEN_WIDTH - FONT_MARGIN, BOTTOM_SCREEN_HEIGHT - (FONT_HEIGHT_STD * 3), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_BLUE));

				// UNINSTALL
				TextPrinter::print(m_actions[ACTION_UNINSTALL].c_str(), C2D_AlignRight, BOTTOM_SCREEN_WIDTH - FONT_MARGIN, BOTTOM_SCREEN_HEIGHT - (FONT_HEIGHT_STD * 2), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_RED));
			}
			else
			{
				// ESTAT
				string state = "Estat: NO INSTAL·LADA";
				TextPrinter::print(state.c_str(), C2D_AlignLeft, FONT_MARGIN, BOTTOM_SCREEN_HEIGHT - (FONT_HEIGHT_STD * 3), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_RED), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

				// VERSION
				if (!string(m_translations[m_current]["version"]).empty())
				{
					TextPrinter::print(string(m_translations[m_current]["version"]).c_str(), C2D_AlignLeft, FONT_MARGIN, BOTTOM_SCREEN_HEIGHT - (FONT_HEIGHT_STD * 2), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_YELLOW));
				}

				// INSTALL
				TextPrinter::print(m_actions[ACTION_INSTALL].c_str(), C2D_AlignRight, BOTTOM_SCREEN_WIDTH - FONT_MARGIN, BOTTOM_SCREEN_HEIGHT - (FONT_HEIGHT_STD * 2), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_GREEN));
			}



			if (Input::isPressed(KEY_Y) && !m_installed[m_current])
			{
				m_currentAction = ACTION_INSTALL;
				m_step = STEP_3_PREPARE_ACTION;
			}

			else if (Input::isPressed(KEY_X) && m_installed[m_current])
			{
				m_currentAction = ACTION_UNINSTALL;
				m_step = STEP_3_PREPARE_ACTION;
			}

			else if (Input::isPressed(KEY_B))
			{
				m_currentAction = ACTION_UNKNOWN;
				m_step = STEP_1_SELECT_TRANSLATION;
			}
			break;



		case STEP_3_PREPARE_ACTION:
			ScreenBottomGeneric(string(m_translations[m_current]["name"]).c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_BLANK);

			TextPrinter::print(m_doingAction[m_currentAction].c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, BOTTOM_SCREEN_HEIGHT / 2 - (2 * FONT_HEIGHT_STD), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

			m_step = STEP_4_ACTION;
			break;



		case STEP_4_ACTION:
			if (m_currentAction == ACTION_INSTALL)
				m_actionResult = setTranslation();
			else if (m_currentAction == ACTION_UNINSTALL)
				m_actionResult = unsetTranslation();
			else
				m_actionResult = false;

			m_step = STEP_5_RESULT;
			break;



		case STEP_5_RESULT:
			ScreenBottomGeneric(string(m_translations[m_current]["name"]).c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_A_ACCEPTA);

			res = ACTION_RESULT_UNKNOWN;

			if (m_currentAction == ACTION_INSTALL && m_actionResult)
				res = ACTION_RESULT_INSTALL_OK;
			else if (m_currentAction == ACTION_INSTALL && !m_actionResult)
				res = ACTION_RESULT_INSTALL_FAIL;
			else if (m_currentAction == ACTION_UNINSTALL && m_actionResult)
				res = ACTION_RESULT_UNINSTALL_OK;
			else if (m_currentAction == ACTION_UNINSTALL && !m_actionResult)
				res = ACTION_RESULT_UNINSTALL_FAIL;

			TextPrinter::print(m_result[res].c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, BOTTOM_SCREEN_HEIGHT / 2 - FONT_HEIGHT_STD * 2, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

			if (Input::isPressed(KEY_A))
			{
				m_step = STEP_1_SELECT_TRANSLATION;
				m_currentAction = ACTION_UNKNOWN;

				updateInstalledStatus();
			}
			break;



		default:
			ScreenBottomGeneric(string(m_translations[m_current]["name"]).c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_BLANK);
				
			TextPrinter::print(m_result[ACTION_RESULT_UNKNOWN].c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, BOTTOM_SCREEN_HEIGHT / 2 - FONT_HEIGHT_STD * 2, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);
			break;
		}


		// END

		m_spritePrinter.endFrame();

		Graphics::endFrame();

		if (exitLoop)
			break;
	}

	m_spritePrinter.exit();
}

bool ScreenCatalanitza::updateTranslations()
{
	string pathOut = "";
	
	if (!m_updated && osGetWifiStrength() != 0)
	{
		FS::createDirectory("/PCT");
		
		if (R_FAILED(HTTP::downloadFile(pathOut, HTTP::URL_TRADUCCIONS, "sdmc:/PCT/", "3ds-cat.json")))
			return false;
		
		m_updated = true;
	}
	
	ifstream file("sdmc:/PCT/3ds-cat.json");
	m_translations = JSON::parse(file);

	if (m_translations.empty())
	{
		m_nTranslations = 0;
		return false;
	}

	m_nTranslations = m_translations.size();

	return true;
}

bool ScreenCatalanitza::checkIfInstalled(size_t catalanitza)
{
	string path = "/luma/titles/" + string(m_translations[catalanitza]["titleID"]);

	bool res = FS::createDirectory(path);

	// True: could create, it is not installed -> delete and return false
	// False: could not create, it is installed -> return true
	if (res)
	{
		FS::deleteDirectory(path);
		return false;
	}
	else
	{
		return true;
	}
}

void ScreenCatalanitza::updateInstalledStatus()
{
	m_installed.clear();
	
	for (size_t i = 0; i < m_nTranslations; i++)
		m_installed.push_back(checkIfInstalled(i));
}

bool ScreenCatalanitza::setTranslation()
{
	// Check if already installed
	// True -> delete and continue
	// False -> continue
	if (checkIfInstalled(m_current))
	{
		string path = "/luma/titles/" + string(m_translations[m_current]["titleID"]);
		FS::deleteDirectory(path);
	}

	// Create the directory "/luma/"
	FS::createDirectory("/luma");

	// Create the directory "/luma/titles/"
	FS::createDirectory("/luma/titles");

	// Download the translation into the "/PCT/Baixades" directory
	FS::createDirectory("/PCT");
	FS::createDirectory("/PCT/baixades");

	// Download the zip file
	string filePath, fileName = string(m_translations[m_current]["titleID"]) + ".zip";

	// Make sure the file does not exist
	FS::deleteFile("/PCT/baixades/" + fileName);

	if (R_FAILED(HTTP::downloadFile(filePath, string(m_translations[m_current]["url"]), "/PCT/baixades/", fileName)))
		return false;

	if (filePath.empty())
		return false;

	// Extract the zip file
	//if (!FS::ZIP::extract(filePath, "/PCT/baixades/"))
	if (!FS::ZIP::extract(filePath, "/luma/titles/"))
		return false;

	// Delete the zip file
	// FS::deleteFile(filePath); -> Not needed

	return true;
}

bool ScreenCatalanitza::unsetTranslation()
{
	// Delete the directory "/luma/titles/" + m_titleId[m_current]
	string path = "/luma/titles/" + string(m_translations[m_current]["titleID"]);
	return FS::deleteDirectory(path);
}
