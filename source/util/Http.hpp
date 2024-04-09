/*
*   This file is part of PCT-3DS
*   Copyright (C) 2024 Nilcm01 + Projecte 'Ce Trencada'
*
*	Check the license details in /LICENSE.md
*
*	The code within this file belongs to the following sources and authors:
*
*	- Multidownload (hax0kartik) <https://github.com/hax0kartik/Multidownload>:
*		Functions «entradesWeb», «downloadFile» and «downloadTextToFile» based on
*		function «http_download» from file «download.hpp».
*	- Nilcm01 <https://github.com/Nilcm01>:
*		File structure, functions «entradesWeb», «downloadFile» and «downloadText»
*		and the code modifications from original source.
*/

#pragma once

#ifndef HTTP_HPP
#define HTTP_HPP

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <iostream>

#include <3ds.h>

#include "../common/commonValues.hpp"
#include "../util/FS.hpp"
#include "../util/TextPrinter.hpp"

using namespace std;

namespace HTTP
{
	const char* const URL_ENTRADES_WEB = "http://cetrencada.cat/comunitat/api/v1/request3ds.php?data=entrada";
	const char* const URL_ZIP_HOME = "https://drive.usercontent.google.com/u/0/uc?id=1IxIYjB-lvmU9JTc6jtZnTEL-V0Cjb2w8&export=download";

	// DEPRECATED
	// Use downloadTextToFile instead
	int entradesWeb();
	// Downloads a file from the specified URL and saves it to the specified location
	// @param outPath: (out) The path where the downloaded file is located
	// @param url : URL of the file to download
	// @param location : Location to save the file to
	// @param filename : Name (including extension) of the file to save. Empty string to use the original filename
	// @return 0 if the download was successful, 21 if it failed
	Result downloadFile(string& outPath, string url, string path, string filename);
	// @param filename : Includes the extension
	Result downloadTextToFile(string url, string path, string filename);

	Result downloadText(string& out, string url);
}

#endif // !HTTP_HPP
