#include "ScreenTraduccions.hpp"

ScreenTraduccions::ScreenTraduccions(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen, bool& entradesUpdated) :
	m_topScreen(topScreen), m_bottomScreen(bottomScreen), m_entradesUpdated(&entradesUpdated)
{
	m_spritePrinter = SpritePrinter();
}

void ScreenTraduccions::render()
{
	m_spritePrinter.start();
	
	// Main program loop
	while (true)
	{
		Input::start();

		if (Input::isPressed(KEY_B))
			break;


		Graphics::startFrame(m_topScreen, m_bottomScreen);

		m_spritePrinter.startFrame();


		// Top screen (400x240)

		Graphics::activeScreen(m_topScreen);

		ScreenTopGeneric().render(m_spritePrinter);



		// Bottom screen (320x240)

		Graphics::activeScreen(m_bottomScreen);

		ScreenBottomGeneric(m_title.c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_ALRB_LIST);

		//TextPrinter::print(to_string(m_idebug).c_str(), C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * 1, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

		// Print the entries
		if (m_nEntrades > 0)
		{
			// For debug purposes. Needs screenBottomGeneric_render to be commented.
			//char* test = (char*)malloc(0x40);
			//sprintf(test, "lower: %i, current: %i, upper: %i", current, lower, upper);
			//textPrinter(test, C2D_AlignLeft, FONT_MARGIN, 0.0f, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_WHITE));

			for (size_t i = m_lower; i < m_upper; i++)
			{
				if (i >= m_nEntrades)
					break;
				
				u32 textColor = C2D_Color32(COLOR_WHITE);

				if (i == m_current)
				{
					textColor = C2D_Color32(COLOR_BLACK);
					C2D_DrawRectSolid(0.0f, FONT_HEIGHT_STD * (i - m_lower + 1), 0.0f, BOTTOM_SCREEN_WIDTH, FONT_HEIGHT_STD, C2D_Color32(0xD3, 0xD3, 0xD3, 0xFF));
				}

				if (!m_entrades[i].empty() && m_entrades[i].is_object() && m_entrades[i].contains("nom"))
				{
					TextPrinter::print(string(m_entrades[i]["nom"]).c_str(), C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * (i - m_lower + 1), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, textColor);
				}
			}
		}
		else
		{
			TextPrinter::print("No s'han pogut llegir les entrades", C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, BOTTOM_SCREEN_HEIGHT / 2, 0.5f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
		}


		if (Input::isPressed(KEY_A))
		{
			ScreenEntrada screenEntrada = ScreenEntrada(m_topScreen, m_bottomScreen, m_entrades, m_current);
			screenEntrada.render();
		}

		if (Input::isHeld(KEY_UP) || Input::isPressed(KEY_ZL))
		{
			if (m_framesHeldUp >= FRAMES_WAIT_HOLDING)
				m_framesHeldUp = FRAMES_WAIT_HOLDING;
			else
				m_framesHeldUp++;
		}
		if (Input::isReleased(KEY_UP) || Input::isReleased(KEY_ZL))
		{
			m_framesHeldUp = 0;
		}

		if (Input::isHeld(KEY_DOWN) || Input::isPressed(KEY_ZR))
		{
			if (m_framesHeldDown >= FRAMES_WAIT_HOLDING)
				m_framesHeldDown = FRAMES_WAIT_HOLDING;
			else
				m_framesHeldDown++;
		}
		if (Input::isReleased(KEY_DOWN) || Input::isReleased(KEY_ZR))
		{
			m_framesHeldDown = 0;
		}

		if (Input::isPressed(KEY_L) || Input::isPressed(KEY_CSTICK_UP))
		{
			// Set to the start of the list
			m_current = 0;
			m_lower = 0;
			m_upper = m_MAX_ENTRIES;
		}

		if (Input::isPressed(KEY_R) || Input::isPressed(KEY_CSTICK_DOWN))
		{
			// Set to the end of the list
			m_current = m_nEntrades - 1;
			m_lower = m_nEntrades - m_MAX_ENTRIES;
			m_upper = m_nEntrades;
		}

		if (Input::isPressed(KEY_UP) || Input::isPressed(KEY_ZL) || m_framesHeldUp >= FRAMES_WAIT_HOLDING)
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
				m_current = m_nEntrades - 1;
				m_lower = m_nEntrades - m_MAX_ENTRIES;
				m_upper = m_nEntrades;
				m_framesHeldDown = 0;
			}
		}
		else if (Input::isPressed(KEY_DOWN) || Input::isPressed(KEY_ZR) || m_framesHeldDown >= FRAMES_WAIT_HOLDING)
		{
			if (m_current < m_nEntrades - 1)
			{
				m_current++;

				if (m_current == m_upper - 1 && m_upper < m_nEntrades)
				{
					m_lower++;
					m_upper++;
				}
			}
			else if (m_current == m_nEntrades - 1)
			{
				// Set to the start of the list
				m_current = 0;
				m_lower = 0;
				m_upper = m_MAX_ENTRIES;
				m_framesHeldDown = 0;
			}
		}

		// END

		m_spritePrinter.endFrame();

		Graphics::endFrame();
	}

	m_spritePrinter.exit();
}

//int updateEntradesOld()
//{
//	//HTTP::entradesWeb();
//	FS::createDirectory("/PCT");
//	HTTP::downloadTextToFile(HTTP::URL_ENTRADES_WEB, "sdmc:/PCT/", "entradesweb.json");
//
//	// File
//
//	file = fopen("sdmc:/PCT/entradesweb.json", "rb");
//
//	if (file != NULL)
//	{
//		// Get the file size
//		fseek(file, 0, SEEK_END);
//		long fileSize = ftell(file);
//		fseek(file, 0, SEEK_SET);
//
//		// Read the file
//		char* fileData = (char*)malloc(fileSize + 1);
//		fread(fileData, 1, fileSize, file);
//		fileData[fileSize] = '\0';
//
//		// Close the file
//		fclose(file);
//
//		// Parse the JSON
//		cJSON* json = cJSON_Parse(fileData);
//		nEntrades = cJSON_GetArraySize(json);
//		
//		/*
//		* JSON STRUCTURE:
//		*
//		* [
//		*	{
//		*		"id": string,
//		*		"nom": string,
//		*		"any": string,
//		*		"desenv": string,
//		*		"imatge": string,
//		*		"text_curt": string,
//		*		"categoria": string,
//		*		"categoria_nom": string
//		*	},
//		*	...
//		* ]
//		*
//		*/
//
//		cJSON* entry = json->child;
//		size_t entryIndex = 0;
//
//		while (entryIndex < nEntrades)
//		{
//			// Get the entry data
//			cJSON* element = entry->child;
//
//			char* id = element->valuestring;
//			element = element->next;
//			char* nom = element->valuestring;
//			/*element = element->next;
//			char* any = element->valuestring;
//			element = element->next;
//			char* desenv = element->valuestring;
//			element = element->next;
//			char* imatge = element->valuestring;
//			element = element->next;
//			char* text_curt = element->valuestring;
//			element = element->next;
//			char* categoria = element->valuestring;
//			element = element->next;
//			char* categoria_nom = element->valuestring;*/
//
//			// Print the entry data
//			//textPrinter(id, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * entryIndex, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
//
//			// Add the entry to the array
//			m_entradesNoms.push_back(nom);
//			m_entradesId.push_back(id);
//
//			// Next entry
//			entry = entry->next;
//			entryIndex++;
//		}
//
//		cJSON_Delete(json);
//		free(fileData);
//	}
//	else
//	{
//		//textPrinter("No s'ha pogut obrir el fitxer \"sdmc:/3ds/entradesweb.txt\".", C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, BOTTOM_SCREEN_HEIGHT / 2, 0.5f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
//		
//		return -1;
//	}
//
//	return 0;
//}/**/

int ScreenTraduccions::updateEntrades()
{
	if (!*m_entradesUpdated && osGetWifiStrength() != 0)
	{
		FS::createDirectory("/PCT");
		HTTP::downloadTextToFile(HTTP::URL_ENTRADES_WEB, "sdmc:/PCT/", "entradesweb.json");
		*m_entradesUpdated = true;
	}

	ifstream file("sdmc:/PCT/entradesweb.json");
	m_entrades = JSON::parse(file);
	m_nEntrades = m_entrades.size();
	
	return 0;

	//  JSON STRUCTURE:
	/*
	* [
	*	{
	*		"id": string,
	*		"nom": string,
	*		"any": string,
	*		"desenv": string,
	*		"imatge": string,
	*		"text_curt": string,
	*		"categoria": string,
	*		"categoria_nom": string
	*	},
	*	...
	* ]
	*
	*/
}
