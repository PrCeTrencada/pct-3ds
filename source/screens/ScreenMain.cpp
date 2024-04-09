#include "ScreenMain.hpp"

ScreenMain::ScreenMain(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen) :
	m_topScreen(topScreen), m_bottomScreen(bottomScreen)
{
	m_spritePrinter = SpritePrinter();

	m_step = STEP_1_PREPARE_PRELOAD;

	ScreenTraduccions screenTraduccions(m_topScreen, m_bottomScreen, m_entradesUpdated);
	ScreenCatalanitza screenCatalanitza(m_topScreen, m_bottomScreen);
	ScreenOpcions screenOpcions(m_topScreen, m_bottomScreen);
	size_t currentOption = 0;

	bool exit = false;

	m_spritePrinter.start();

	// Main program loop
	while (aptMainLoop())
	{
		Input::start();

		Graphics::startFrame(m_topScreen, m_bottomScreen);

		m_spritePrinter.startFrame();


		switch (m_step)
		{

		case ScreenMain::STEP_1_PREPARE_PRELOAD:
			// Top screen (400x240)
			Graphics::activeScreen(m_topScreen);
			m_spritePrinter.print(SPRITE_PCT_LOGO_150, (TOP_SCREEN_WIDTH / 2) - 75, (TOP_SCREEN_HEIGHT / 2) - 75);

			// Bottom screen (320x240)
			Graphics::activeScreen(m_bottomScreen);
			
			TextPrinter::print(m_textAppName.c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, FONT_HEIGHT_STD * 5, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_YELLOW), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

			TextPrinter::print(m_textLoading.c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, FONT_HEIGHT_STD * 7, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

			m_step = STEP_2_PRELOAD;
			break;



		case ScreenMain::STEP_2_PRELOAD:
			// Load config : TODO
			// Check for updates : TODO
			screenTraduccions.updateEntrades();
			
			m_step = STEP_3_MAIN;
			break;



		case ScreenMain::STEP_3_MAIN:
			if (Input::isPressed(KEY_START))
			{
				exit = true;
				break; // break in order to return to hbmenu or exit the app
			}
			
			// Top screen (400x240)
			Graphics::activeScreen(m_topScreen);
			ScreenTopGeneric().render(m_spritePrinter);

			// Bottom screen (320x240)
			Graphics::activeScreen(m_bottomScreen);
			ScreenBottomGeneric(m_title, SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_ABSTART);

			
			for (size_t i = 0; i < m_nTexts; i++)
			{
				u32 textColor = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);

				if (i == currentOption)
				{
					textColor = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
					C2D_DrawRectSolid(0.0f, FONT_HEIGHT_STD * (i + 1), 0.0f, BOTTOM_SCREEN_WIDTH, FONT_HEIGHT_STD, C2D_Color32(0xD3, 0xD3, 0xD3, 0xFF));
				}

				TextPrinter::print(screenMain_options[i], C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * (i + 1), 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, textColor);
			}

			if (Input::isPressed(KEY_UP) || Input::isPressed(KEY_ZL))
			{
				if (currentOption > 0)
					currentOption--;
				else
					currentOption = m_nTexts - 1;
			}
			else if (Input::isPressed(KEY_DOWN) || Input::isPressed(KEY_ZR))
			{
				if (currentOption < m_nTexts - 1)
					currentOption++;
				else
					currentOption = 0;
			}
			else if (Input::isPressed(KEY_A))
			{
				switch (currentOption)
				{
				case SCREENMAIN_OPTION_TRADUCCIONS:
					screenTraduccions.render();
					break;

				case SCREENMAIN_OPTION_CATALANITZA:
					screenCatalanitza.render();
					break;

				case SCREENMAIN_OPTION_OPCIONS:
					screenOpcions.render();
					break;

				case SCREENMAIN_OPTION_CREDITS:
					ScreenCredits(m_topScreen, m_bottomScreen);
					break;
				}
			}
			break;



		default:
			break;
		}


		// END

		m_spritePrinter.endFrame();

		Graphics::endFrame();

		if (exit) break;
	}

	m_spritePrinter.exit();
}
