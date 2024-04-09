#include "ScreenCredits.hpp"

ScreenCredits::ScreenCredits(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen) :
	m_topScreen(topScreen), m_bottomScreen(bottomScreen)
{
	m_spritePrinter = SpritePrinter();
	
	m_spritePrinter.start();
	
	while (true)
	{
		Input::start();

		Graphics::startFrame(m_topScreen, m_bottomScreen);

		m_spritePrinter.startFrame();

		if (Input::isPressed(KEY_B))
			break;


		// Top screen (400x240)

		Graphics::activeScreen(topScreen);

		ScreenTopGeneric().render(m_spritePrinter);


		// Bottom screen (320x240)

		Graphics::activeScreen(bottomScreen);

		ScreenBottomGeneric(m_title, SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_B);


		TextPrinter::print(m_text, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * 1, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

		//m_spritePrinter.print(SPRITE_QR_GITHUB_48, BOTTOM_SCREEN_WIDTH - FONT_MARGIN - 48.0f, BOTTOM_SCREEN_HEIGHT - FONT_HEIGHT_STD - FONT_MARGIN - 48.0f);
		

		// END

		m_spritePrinter.endFrame();

		Graphics::endFrame();
	}

	m_spritePrinter.exit();
}
