#include "SpritePrinter.hpp"

void SpritePrinter::start()
{
	m_spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!m_spriteSheet) svcBreak(USERBREAK_PANIC);

	m_sprites.clear();
	m_rendered.clear();
}

void SpritePrinter::startFrame()
{
	m_rendered.clear();
	m_rendered.resize(m_sprites.size(), false);
}

void SpritePrinter::print(size_t imageId, float x, float y)
{
	int spriteIndex = findUnrenderedSprite(imageId);
	
	if (spriteIndex == -1)
	{
		addSprite(imageId, x, y);
		spriteIndex = m_sprites.size() - 1;
	}
	else
		C2D_SpriteSetPos(&m_sprites[spriteIndex].second, x, y);
	
	
	C2D_DrawSprite(&m_sprites[spriteIndex].second);
	m_rendered[spriteIndex] = true;
}

void SpritePrinter::endFrame()
{
	// Erase the elements from vector "sprites" that have a "false" value in vector "rendered"
	size_t i = 0;
	while (i < m_sprites.size())
	{
		if (!m_rendered[i])
		{
			m_sprites.erase(m_sprites.begin() + i);
			m_rendered.erase(m_rendered.begin() + i);
		}
		else
			i++;
	}
}

void SpritePrinter::exit()
{
	C2D_SpriteSheetFree(m_spriteSheet);
	m_sprites.clear();
	m_rendered.clear();
}

void SpritePrinter::addSprite(size_t imageId, float x, float y)
{
	C2D_Sprite sprite;
	C2D_SpriteFromSheet(&sprite, m_spriteSheet, imageId);
	C2D_SpriteSetPos(&sprite, x, y);

	m_sprites.push_back(make_pair(imageId, sprite));
	m_rendered.push_back(false);
}

int SpritePrinter::findUnrenderedSprite(size_t imageId)
{
	int pos = -1;

	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		if (m_sprites[i].first == imageId && !m_rendered[i])
		{
			pos = i;
			break;
		}
	}

	return pos;
}
