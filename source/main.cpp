#include <3ds.h>
#include <citro2d.h>

#include "./screens/ScreenMain.hpp"

int main()
{
	// Initialize the libs
	romfsInit();
	fsInit();
	mcuHwcInit();
	ptmuInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	httpcInit(0);

	// Create screens
	C3D_RenderTarget* topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	// Go to main screen
	ScreenMain(topScreen, bottomScreen);

	// Deinitialize the libs
	fsExit();
	httpcExit();
	mcuHwcExit();
	ptmuExit();
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
