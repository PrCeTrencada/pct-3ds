#include "ScreenEntrada.hpp"

ScreenEntrada::ScreenEntrada(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen, JSON& entrades, size_t &currentEntrada) :
	m_topScreen(topScreen), m_bottomScreen(bottomScreen), m_entrades(entrades), m_currentEntrada(currentEntrada)
{
	m_spritePrinter = SpritePrinter();
}

void ScreenEntrada::render()
{
	m_spritePrinter.start();

	float pos = 1;
	JSON entrada;

	// Main program loop
	while (true)
	{
		Input::start();

		if (Input::isPressed(KEY_B))
			break;

		entrada = m_entrades[m_currentEntrada];


		Graphics::startFrame(m_topScreen, m_bottomScreen);

		m_spritePrinter.startFrame();


		// Top screen (400x240)

		Graphics::activeScreen(m_topScreen);

		ScreenTopGeneric().render(m_spritePrinter);



		// Bottom screen (320x240)

		Graphics::activeScreen(m_bottomScreen);

		
		string categoria = "Categoria: " + string(entrada["categoria_nom"]);
		TextPrinter::print(categoria.c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, FONT_HEIGHT_STD * (pos + 1), 0.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

		bool desenvWrapped = TextPrinter::print(string(entrada["desenv"]).c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, FONT_HEIGHT_STD * (pos + 3), 0.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

		int wrapOver = (desenvWrapped) ? 1 : 0;
		
		TextPrinter::print(string(entrada["any"]).c_str(), C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, FONT_HEIGHT_STD * (pos + 4 + wrapOver), 0.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

		
		TextPrinter::print(string(entrada["text_curt"]).c_str(), C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * (pos + 6 + wrapOver), 0.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE), BOTTOM_SCREEN_WIDTH - FONT_MARGIN * 2);

		if (Input::isPressed(KEY_L) || Input::isPressed(KEY_CSTICK_UP))
		{
			pos = 1;
		}
		else if (Input::isPressed(KEY_R) || Input::isPressed(KEY_CSTICK_DOWN))
		{
			pos = -5;
		}
		
		if (Input::isPressed(KEY_DOWN) || Input::isPressed(KEY_ZR))
		{
			if (pos > -4)
				pos--;
		}
		else if (Input::isPressed(KEY_UP) || Input::isPressed(KEY_ZL))
		{
			if (pos < 1)
				pos++;
		}

		if (Input::isPressed(KEY_LEFT))
		{
			if (m_currentEntrada > 0)
				m_currentEntrada--;
		}
		else if (Input::isPressed(KEY_RIGHT))
		{
			if (m_currentEntrada < m_entrades.size() - 1)
				m_currentEntrada++;
		}

		ScreenBottomGeneric(string(entrada["nom"]).c_str(), SCREEN_BOTTOM_GENERIC_TEXT_GUIDE_SCREEN_LRDHRB_LIST);
		


		// END

		m_spritePrinter.endFrame();

		Graphics::endFrame();
	}

	m_spritePrinter.exit();
}
