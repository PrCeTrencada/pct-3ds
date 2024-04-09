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

FS_Path getPathInfo(const char* path, FS_ArchiveID* archive) {
	*archive = ARCHIVE_SDMC;
	FS_Path filePath = { PATH_INVALID, 0, nullptr };
	unsigned int prefixlen = 0;

	if (!strncmp(path, "sdmc:/", 6)) {
		prefixlen = 5;

	}
	else if (*path != '/') {
		/*
			if the path is local (doesnt start with a slash),
			it needs to be appended to the working dir to be valid.
		*/

		string actualPath = "/";
		actualPath += path;
		filePath = fsMakePath(PATH_ASCII, actualPath.c_str());
	}

	/* if the filePath wasnt set above, set it. */
	if (filePath.size == 0) filePath = fsMakePath(PATH_ASCII, path + prefixlen);

	return filePath;
}

Result makeDirs(char* path) {
	Result ret = 0;
	FS_ArchiveID archiveID;
	FS_Path filePath = getPathInfo(path, &archiveID);
	FS_Archive archive;

	ret = FSUSER_OpenArchive(&archive, archiveID, fsMakePath(PATH_EMPTY, ""));

	for (char* slashpos = strchr(path + 1, '/'); slashpos != NULL; slashpos = strchr(slashpos + 1, '/')) {
		char bak = *(slashpos);
		*(slashpos) = '\0';
		Handle dirHandle;

		ret = FSUSER_OpenDirectory(&dirHandle, archive, filePath);
		if (R_SUCCEEDED(ret)) FSDIR_Close(dirHandle);
		else ret = FSUSER_CreateDirectory(archive, filePath, FS_ATTRIBUTE_DIRECTORY);

		*(slashpos) = bak;
	}

	FSUSER_CloseArchive(archive);

	return ret;
}

Result openFile(Handle* fileHandle, const char* path, bool write) {
	FS_ArchiveID archive;
	FS_Path filePath = getPathInfo(path, &archive);
	u32 flags = (write ? (FS_OPEN_CREATE | FS_OPEN_WRITE) : FS_OPEN_READ);

	Result ret = 0;
	ret = makeDirs(strdup(path));
	ret = FSUSER_OpenFileDirectly(fileHandle, archive, fsMakePath(PATH_EMPTY, ""), filePath, flags, 0);
	if (write)	ret = FSFILE_SetSize(*fileHandle, 0); // truncate the file to remove previous contents before writing.

	return ret;
}

Result Title::launch(u64 titleId, FS_MediaType mediaType) {
	Result ret = 0;
	u8 param[0x300];
	u8 hmac[0x20];

	if (R_FAILED(ret = APT_PrepareToDoApplicationJump(0, titleId, mediaType))) {
		printf("Error In:\nAPT_PrepareToDoApplicationJump");
		return ret;
	}

	if (R_FAILED(ret = APT_DoApplicationJump(param, sizeof(param), hmac))) {
		printf("Error In:\nAPT_DoApplicationJump");
		return ret;
	}

	return 0;
}

Result Title::deletePrevious(u64 titleid, FS_MediaType media) {
	Result ret = 0;
	u32 titles_amount = 0;

	ret = AM_GetTitleCount(media, &titles_amount);
	if (R_FAILED(ret)) {
		printf("Error in:\nAM_GetTitleCount\n");
		return ret;
	}

	u32 read_titles = 0;
	u64* titleIDs = (u64*)malloc(titles_amount * sizeof(u64));

	ret = AM_GetTitleList(&read_titles, media, titles_amount, titleIDs);
	if (R_FAILED(ret)) {
		free(titleIDs);
		printf("Error in:\nAM_GetTitleList\n");
		return ret;
	}

	for (u32 i = 0; i < read_titles; i++) {
		if (titleIDs[i] == titleid) {
			ret = AM_DeleteAppTitle(media, titleid);
			break;
		}
	}

	free(titleIDs);

	if (R_FAILED(ret)) {
		printf("Error in:\nAM_DeleteAppTitle\n");
		return ret;
	}

	return 0;
}

static FS_MediaType getTitleDestination(u64 titleId) {
	u16 platform = (u16)((titleId >> 48) & 0xFFFF);
	u16 category = (u16)((titleId >> 32) & 0xFFFF);
	u8 variation = (u8)(titleId & 0xFF);

	//     DSiWare                3DS                    DSiWare, System, DLP         Application           System Title
	return platform == 0x0003 || (platform == 0x0004 && ((category & 0x8011) != 0 || (category == 0x0000 && variation == 0x02))) ? MEDIATYPE_NAND : MEDIATYPE_SD;
}

u32 installSize = 0, installOffset = 0;

Result Title::install(const char* ciaPath, bool updatingSelf) {
	u32 bytes_read = 0, bytes_written;
	installSize = 0, installOffset = 0; u64 size = 0;
	Handle ciaHandle, fileHandle;
	AM_TitleEntry info;
	Result ret = 0;
	FS_MediaType media = MEDIATYPE_SD;

	ret = openFile(&fileHandle, ciaPath, false);
	if (R_FAILED(ret)) {
		printf("Error in:\nopenFile\n");
		return ret;
	}

	ret = AM_GetCiaFileInfo(media, &info, fileHandle);
	if (R_FAILED(ret)) {
		printf("Error in:\nAM_GetCiaFileInfo\n");
		return ret;
	}

	media = getTitleDestination(info.titleID);

	if (!updatingSelf) {
		ret = Title::deletePrevious(info.titleID, media);
		if (R_FAILED(ret)) return ret;
	}

	ret = FSFILE_GetSize(fileHandle, &size);
	if (R_FAILED(ret)) {
		printf("Error in:\nFSFILE_GetSize\n");
		FSFILE_Close(fileHandle);
		return ret;
	}

	//if (FS::getAvailableSpace() >= size) {
	ret = AM_StartCiaInstall(media, &ciaHandle);
	if (R_FAILED(ret)) {
		printf("Error in:\nAM_StartCiaInstall\n");
		FSFILE_Close(fileHandle);
		return ret;
	}

	u32 toRead = 0x200000;
	u8* buf = new u8[toRead];

	if (!buf) {
		FSFILE_Close(fileHandle);
		return -1;
	}

	installSize = size;
	do {
		FSFILE_Read(fileHandle, &bytes_read, installOffset, buf, toRead);
		FSFILE_Write(ciaHandle, &bytes_written, installOffset, buf, toRead, FS_WRITE_FLUSH);
		installOffset += bytes_read;
	} while (installOffset < installSize);
	delete[] buf;

	ret = AM_FinishCiaInstall(ciaHandle);
	if (R_FAILED(ret)) {
		printf("Error in:\nAM_FinishCiaInstall\n");
		FSFILE_Close(fileHandle);
		return ret;
	}
	//}

	ret = FSFILE_Close(fileHandle);
	if (R_FAILED(ret)) {
		printf("Error in:\nFSFILE_Close\n");
		return ret;
	}

	if (updatingSelf) {
		if (R_FAILED(ret = Title::launch(info.titleID, MEDIATYPE_SD))) return ret;
	}

	return 0;
}
