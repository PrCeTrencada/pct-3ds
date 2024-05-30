/*
*   This file is part of PCT-3DS
*   Copyright (C) 2024 Nilcm01 + Projecte 'Ce Trencada'
*
*	Check the license details in /LICENSE.md
*
*	The code within this file belongs to the following sources and authors:
* 
*	- Universal-Updater (Universal-Team) <https://github.com/Universal-Team/Universal-Updater>:
*		Functions: «launch», «deletePrevious» and «install» from file «cia.hpp».
*	- Nilcm01 <https://github.com/Nilcm01>:
*		File structure and code modifications from original source.
*/

#pragma once
#ifndef TITLE_HPP
#define TITLE_HPP

#include <string>

#include <3ds.h>
#include <nlohmann/json.hpp>

#include "../common/commonValues.hpp"
#include "FS.hpp"
#include "Http.hpp"
#include "Version.hpp"

using namespace std;
using JSON = nlohmann::json;

namespace Title
{
	enum Title_Format {
		FORMAT_UNDEFINED = -1,
		FORMAT_CIA,
		FORMAT_3DSX
	};
	
	Result launchTitle(u64 titleId);
	Result installCia(string ciapath);
	// TODO
	Result install3dsx(const string& dsxPath, bool updateSelf);

	Result updateSelf();
}

#endif // !TITLE_HPP
