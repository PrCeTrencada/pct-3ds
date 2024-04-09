#include "ScreenBottomGeneric.hpp"

ScreenBottomGeneric::ScreenBottomGeneric(const char* title, const size_t guideType)
{
	C2D_DrawRectSolid(0.0f, 0.0f, 0.0f, BOTTOM_SCREEN_WIDTH, FONT_HEIGHT_STD, C2D_Color32(COLOR_GRAY));
	TextPrinter::print(title, C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, 0, 1, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_YELLOW));

	C2D_DrawRectSolid(0.0f, TOP_SCREEN_HEIGHT - FONT_HEIGHT_STD, 0.0f, BOTTOM_SCREEN_WIDTH, FONT_HEIGHT_STD, C2D_Color32(COLOR_GRAY));
	TextPrinter::print(m_texts[guideType], C2D_AlignCenter, BOTTOM_SCREEN_WIDTH / 2, BOTTOM_SCREEN_HEIGHT - FONT_HEIGHT_STD, 1, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32(COLOR_YELLOW));
}
