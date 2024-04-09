#include "Graphics.hpp"

void Graphics::startFrame(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen)
{
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(topScreen, C2D_Color32(COLOR_BLACK));
	C2D_TargetClear(bottomScreen, C2D_Color32(COLOR_BLACK));
}

void Graphics::activeScreen(C3D_RenderTarget* screen)
{
	C2D_SceneBegin(screen);
}

void Graphics::endFrame()
{
	C2D_Flush();
	C3D_FrameEnd(0);
}
