#include "TextPrinter.hpp"

void TextPrinter::print(const char* text, u32 flags, float x, float y, float z, float scaleX, float scaleY, u32 _C2D_Color32f)
{
	C2D_TextBuf buf = C2D_TextBufNew(strlen(text) * 2);
	C2D_Text textBuf;
	C2D_TextParse(&textBuf, buf, text);
	C2D_TextOptimize(&textBuf);
	C2D_DrawText(&textBuf, C2D_WithColor | flags, x, y, z, scaleX, scaleY, _C2D_Color32f);
	C2D_TextBufDelete(buf);
}

bool TextPrinter::print(const char* text, u32 flags, float x, float y, float z, float scaleX, float scaleY, u32 _C2D_Color32f, float wrap)
{
	C2D_TextBuf buf = C2D_TextBufNew(strlen(text) * 2);
	C2D_Text textBuf;
	C2D_TextParse(&textBuf, buf, text);
	C2D_TextOptimize(&textBuf);

	bool wrapped = false;
	float w, h;
	C2D_TextGetDimensions(&textBuf, scaleX, scaleY, &w, &h);
	wrapped = (w >= wrap);
	C2D_DrawText(&textBuf, C2D_WithColor | C2D_WordWrap | flags, x, y, z, scaleX, scaleY, _C2D_Color32f, wrap);
	C2D_TextBufDelete(buf);

	return wrapped;
}
