/*
*   This file is part of PCT-3DS
*   Copyright (C) 2024 Nilcm01 + Projecte 'Ce Trencada'
*
*	Check the license details in /LICENSE.md
*
*	The code within this file belongs to the following sources and authors:
*
*	- Universal-Updater (Universal-Team) <https://github.com/Universal-Team/Universal-Updater>:
*		Functions: «launch», «deletePrevious» and «install» from file «cia.cpp».
*	- Nilcm01 <https://github.com/Nilcm01>:
*		File structure and code modifications from original source.
*/

#include "Title.hpp"



Result Title::launchTitle(u64 titleId) {
	Result ret = 0;
	u8 param[0x300];
	u8 hmac[0x20];

	ret = APT_PrepareToDoApplicationJump(0, titleId, MEDIATYPE_SD);
	
	if (R_FAILED(ret)) {
		//Log::Error("APT_PrepareToDoApplicationJump failed: 0x%x\n", ret);
		return ret;
	}

	ret = APT_DoApplicationJump(param, sizeof(param), hmac);

	if (R_FAILED(ret)) {
		//Log::Error("APT_DoApplicationJump failed: 0x%x\n", ret);
		return ret;
	}

	return 0;
}

Result Title::installCia(string ciapath) {
	Result ret = 0;
	u32 bytes_read = 0, bytes_written = 0;
	u64 size = 0, offset = 0;
	Handle dst_handle, src_handle;
	AM_TitleEntry title;

	FS_Archive sdmc_archive;

	ret = FSUSER_OpenFile(&src_handle, sdmc_archive, fsMakePath(PATH_ASCII, ciapath.c_str()), FS_OPEN_READ, 0);
	
	if (R_FAILED(ret)) {
		//Log::Error("FSUSER_OpenFile failed: 0x%x\n", ret);
		return ret;
	}

	ret = AM_GetCiaFileInfo(MEDIATYPE_SD, &title, src_handle);
	
	if (R_FAILED(ret)) {
		//Log::Error("AM_GetCiaFileInfo failed: 0x%x\n", ret);
		return ret;
	}

	ret = FSFILE_GetSize(src_handle, &size);
	
	if (R_FAILED(ret)) {
		//Log::Error("FSFILE_GetSize failed: 0x%x\n", ret);
		return ret;
	}

	ret = AM_StartCiaInstall(MEDIATYPE_SD, &dst_handle);
	
	if (R_FAILED(ret)) {
		//Log::Error("AM_StartCiaInstall failed: 0x%x\n", ret);
		return ret;
	}

	u32 buffer_size = 0x10000;
	u8* buffer = new u8[buffer_size];

	do {
		std::memset(buffer, 0, buffer_size);

		ret = FSFILE_Read(src_handle, &bytes_read, offset, buffer, buffer_size);
		
		if (R_FAILED(ret)) {
			delete[] buffer;
			FSFILE_Close(src_handle);
			FSFILE_Close(dst_handle);
			//Log::Error("FSFILE_Read failed: 0x%x\n", ret);
			return ret;
		}

		ret = FSFILE_Write(dst_handle, &bytes_written, offset, buffer, bytes_read, FS_WRITE_FLUSH);

		if (R_FAILED(ret)) {
			delete[] buffer;
			FSFILE_Close(src_handle);
			FSFILE_Close(dst_handle);
			//Log::Error("FSFILE_Read failed: 0x%x\n", ret);
			return ret;
		}

		offset += bytes_read;
		//GUI::ProgressBar("Installing", "3DShell_UPDATE.cia", offset, size);
	} while (offset < size);

	if (bytes_read != bytes_written) {
		AM_CancelCIAInstall(dst_handle);
		delete[] buffer;
		//Log::Error(".CIA bytes read and written mismatch: 0x%x\n", ret);
		return ret;
	}

	delete[] buffer;

	ret = AM_FinishCiaInstall(dst_handle);

	if (R_FAILED(ret)) {
		//Log::Error("AM_FinishCiaInstall failed: 0x%x\n", ret);
		return ret;
	}

	ret = FSFILE_Close(src_handle);

	if (R_FAILED(ret)) {
		//Log::Error("FSFILE_Close failed: 0x%x\n", ret);
		return ret;
	}

	FSUSER_DeleteFile(sdmc_archive, fsMakePath(PATH_ASCII, ciapath.c_str())); // Delete update cia

	ret = Title::launchTitle(title.titleID);

	if (R_FAILED(ret))
		return ret;

	return 0;
}



Result Title::install3dsx(const string& dsxPath, bool updateSelf)
{
	return -1;
}

Result Title::updateSelf()
{
	Result ret;
	string out = "", ciaName;
	JSON jlatest;
	
	// Static definition of CIA as the app is only intended to be used this way.
	// Suport for 3DSX is not planned ATM.
	Title_Format format = FORMAT_CIA;

	////// Download the APP

	// Create directories (just in case)
	FS::createDirectory("/PCT");
	//FS::createDirectory("/PCT/actualitzacions"); // Done later

	// Get the data of the latest version
	ret = HTTP::downloadText(out, HTTP::URL_LATEST_VERSION);
	if (R_FAILED(ret) || out.empty()) 
		return -1;

	jlatest = JSON::parse(out);
	if (jlatest.is_null() || jlatest.empty() || jlatest.size() == 0)
		return -1;
	
	// Check if the version on the server is newer than the installed
	Version::Data latestVersion = Version::Data(string(jlatest["version"])),
		currentVersion = Version::getCurrentVersionData();

	if (!(latestVersion > currentVersion))
		return -1;

	// Prepare the download of the CIA
	//ciaName = string(jlatest["version"]) + ".cia";
	ciaName = "update.cia";

	// Delete the previous CIA (if it exists)
	//FS::deleteDirectory("/PCT/actualitzacions");
	//FS::createDirectory("/PCT/actualitzacions");

	// Download the CIA
	out = "";
	ret = HTTP::downloadFile(out, string(jlatest["url"]), "/PCT/", ciaName);
	if (R_FAILED(ret) || out.empty())
		return -1;
	
	////// Install the CIA
	
	ret = Title::installCia(out);
	if (R_FAILED(ret))
		return -1;

	return 0;
}
