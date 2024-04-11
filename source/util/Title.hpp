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

using namespace std;

namespace Title
{
	Result launch(u64 titleId, FS_MediaType mediaType);
	Result deletePrevious(u64 titleid, FS_MediaType media);
	Result install(const char* ciaPath, bool updateSelf);

	Result updateSelf();
}

#endif // !TITLE_HPP
