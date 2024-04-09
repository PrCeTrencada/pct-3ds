#pragma once
#ifndef SPRITEPRINTER_HPP
#define SPRITEPRINTER_HPP

#include <3ds.h>
#include <citro2d.h>

#include <stdlib.h>
#include <vector>

using namespace std;

// Images located in: "$(root)/gfx/"
// ID here corresponds to the position in the "sprites.t3s" file,
// not the actual file position in the directory.
enum {
	SPRITE_WIFI0 = 0,
	SPRITE_WIFI1,
	SPRITE_WIFI2,
	SPRITE_WIFI3,
	SPRITE_PCT_LOGO_150
	//SPRITE_QR_GITHUB_48
};

class SpritePrinter
{
public:
	void start();
	void startFrame();
	void print(size_t imageId, float x, float y);
	void endFrame();
	void exit();

private:
	void addSprite(size_t imageId, float x, float y);
	int findUnrenderedSprite(size_t imageId);

	C2D_SpriteSheet m_spriteSheet;
	// imageId , Sprite
	vector<pair<size_t, C2D_Sprite>> m_sprites;
	vector<bool> m_rendered;
};

#endif // !SPRITEPRINTER_HPP
