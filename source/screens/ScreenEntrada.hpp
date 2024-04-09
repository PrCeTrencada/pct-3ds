#pragma once
#ifndef SCREENENTRADA_HPP
#define SCREENENTRADA_HPP

#include <3ds.h>
#include <citro2d.h>

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "../common/commonValues.hpp"
#include "../util/Graphics.hpp"
#include "../util/Input.hpp"
#include "../util/SpritePrinter.hpp"
#include "../util/TextPrinter.hpp"
#include "../screens/ScreenTopGeneric.hpp"
#include "../screens/ScreenBottomGeneric.hpp"

using namespace std;
using JSON = nlohmann::json;

class ScreenEntrada
{
public:
	ScreenEntrada(C3D_RenderTarget* topScreen, C3D_RenderTarget* bottomScreen, JSON &entrades, size_t &currentEntrada);
	void render();

private:

	C3D_RenderTarget* m_topScreen, * m_bottomScreen;

	SpritePrinter m_spritePrinter;

	bool* m_entradesUpdated;

	JSON m_entrades;
	size_t m_currentEntrada;
};

#endif // !SCREENENTRADA_HPP
