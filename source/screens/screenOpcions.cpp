#include "screenOpcions.hpp"

ScreenOpcions::ScreenOpcions(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen) :
	m_topScreen(topScreen), m_bottomScreen(bottomScreen)
{
	m_spritePrinter = SpritePrinter();
}

void ScreenOpcions::render()
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
		ScreenBottomGeneric(m_title.c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_B);

		TextPrinter::print(m_text.c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, FONT_HEIGHT_STD * 5, 0.5f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

		
		// END

		m_spritePrinter.endFrame();

		Graphics::endFrame();
	}

	m_spritePrinter.exit();
}
