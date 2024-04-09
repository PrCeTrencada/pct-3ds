/*
*   This file is part of PCT-3DS
*   Copyright (C) 2024 Nilcm01 + Projecte 'Ce Trencada'
*
*	Check the license details in /LICENSE.md
*
*	The code within this file belongs to the following sources and authors:
*
*	- Multidownload (hax0kartik) <https://github.com/hax0kartik/Multidownload>:
*		Functions «entradesWeb», «downloadFile» and «downloadText» based on
*		function «http_download» from file «download.cpp».
*	- Nilcm01 <https://github.com/Nilcm01>:
*		File structure, functions «entradesWeb», «downloadFile» and «downloadText»
*		and the code modifications from original source.
*/

#include "Http.hpp"

int HTTP::entradesWeb()
{
	const char* url = HTTP::URL_ENTRADES_WEB;
	Result ret = 0;
	httpcContext context;
	char* newurl = NULL;
	//u8* framebuf_top;
	u32 statuscode = 0;
	u32 contentsize = 0, readsize = 0, size = 0;
	u8* buf, * lastbuf;

	int l = 1;
	char* t = (char*)malloc(200);
	sprintf(t, "Downloading %s\n", url);
	//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
	l++;

	do {
		ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url, 1);
		sprintf(t, "return from httpcOpenContext: %" PRId32 "\n", ret);
		//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
		l++;

		// This disables SSL cert verification, so https:// will be usable
		ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
		sprintf(t, "return from httpcSetSSLOpt: %" PRId32 "\n", ret);
		//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
		l++;

		// Enable Keep-Alive connections
		ret = httpcSetKeepAlive(&context, HTTPC_KEEPALIVE_ENABLED);
		sprintf(t, "return from httpcSetKeepAlive: %" PRId32 "\n", ret);
		//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
		l++;

		// Set a User-Agent header so websites can identify your application
		ret = httpcAddRequestHeaderField(&context, "User-Agent", "httpc-example/1.0.0");
		sprintf(t, "return from httpcAddRequestHeaderField: %" PRId32 "\n", ret);
		//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
		l++;

		// Tell the server we can support Keep-Alive connections.
		// This will delay connection teardown momentarily (typically 5s)
		// in case there is another request made to the same server.
		ret = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");
		sprintf(t, "return from httpcAddRequestHeaderField: %" PRId32 "\n", ret);
		//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
		l++;

		ret = httpcBeginRequest(&context);
		if (ret != 0) {
			httpcCloseContext(&context);
			if (newurl != NULL) free(newurl);
			free(t);
			return -1;
		}

		ret = httpcGetResponseStatusCode(&context, &statuscode);
		if (ret != 0) {
			httpcCloseContext(&context);
			if (newurl != NULL) free(newurl);
			free(t);
			return -1;
		}

		if ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308)) {
			if (newurl == NULL) newurl = (char*)malloc(0x1000); // One 4K page for new URL
			if (newurl == NULL) {
				httpcCloseContext(&context);
				free(t);
				return -1;
			}
			ret = httpcGetResponseHeader(&context, "Location", newurl, 0x1000);
			url = newurl; // Change pointer to the url that we just learned
			sprintf(t, "redirecting to url: %s\n", url);
			//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
			l++;
			httpcCloseContext(&context); // Close this context before we try the next
		}
	} while ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308));

	if (statuscode != 200) {
		sprintf(t, "URL returned status: %" PRId32 "\n", statuscode);
		//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
		l++;
		httpcCloseContext(&context);
		if (newurl != NULL) free(newurl);
		free(t);
		return -1;
	}

	// This relies on an optional Content-Length header and may be 0
	ret = httpcGetDownloadSizeState(&context, NULL, &contentsize);
	if (ret != 0) {
		httpcCloseContext(&context);
		if (newurl != NULL) free(newurl);
		free(t);
		return -1;
	}

	sprintf(t, "reported size: %" PRId32 "\n", contentsize);
	//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
	l++;

	// Start with a single page buffer
	buf = (u8*)malloc(0x1000);
	if (buf == NULL) {
		httpcCloseContext(&context);
		if (newurl != NULL) free(newurl);
		free(t);
		return -1;
	}

	do {
		// This download loop resizes the buffer as data is read.
		ret = httpcDownloadData(&context, buf + size, 0x1000, &readsize);
		size += readsize;
		if (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING) {
			lastbuf = buf; // Save the old pointer, in case realloc() fails.
			buf = (u8*)realloc(buf, size + 0x1000);
			if (buf == NULL) {
				httpcCloseContext(&context);
				free(lastbuf);
				if (newurl != NULL) free(newurl);
				free(t);
				return -1;
			}
		}
	} while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);

	if (ret != 0) {
		httpcCloseContext(&context);
		if (newurl != NULL) free(newurl);
		free(buf);
		free(t);
		return -1;
	}

	// Resize the buffer back down to our actual final size
	lastbuf = buf;
	buf = (u8*)realloc(buf, size);
	if (buf == NULL) { // realloc() failed.
		httpcCloseContext(&context);
		free(lastbuf);
		if (newurl != NULL) free(newurl);
		free(t);
		return -1;
	}

	sprintf(t, "downloaded size: %" PRId32 "\n", size);
	//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD* l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
	l++;

	if (size > (240 * 400 * 3 * 2))size = 240 * 400 * 3 * 2;

	// DO SOMETHING WITH buf HERE

	//sprintf(t, "content: %s\n", buf);
	//textPrinter(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD* l, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
	//l++;

	// Check if directory "sdmc:/PCT" exists
	//FS_Archive sdmcArchive = 0;
	string dirPath = "/PCT";

	if (!FS::createDirectory(dirPath)) {
		// Error creating directory

		//sprintf(t, "Error creating directory\n");
		//TextPrinter::print(t, C2D_AlignLeft, FONT_MARGIN, FONT_HEIGHT_STD * 2, 1.0f, FONT_SIZE_STD, FONT_SIZE_STD, C2D_Color32f(COLOR_WHITE));
		//l++;
		httpcCloseContext(&context);
		free(buf);
		if (newurl != NULL) free(newurl);
		free(t);
		return -1;
	}


	// Export buf to a .txt file
	char* path = (char*)malloc(0x1B);
	sprintf(path, "sdmc:/PCT/entradesweb.json");

	FILE* file = fopen(path, "wb");

	if (file != NULL)
	{
		fwrite(buf, 1, size, file);
		fclose(file);
	}

	// Free the buffer

	httpcCloseContext(&context);

	// Figure out which of this deletions gives problems
	free(buf);
	if (newurl != NULL) free(newurl);
	//free(t);
	//free(path);

	return 0;
}

Result HTTP::downloadFile(string& outPath, string url, string loca, string filename)
{
	Result ret = 0;
	u32 statuscode = 0;
	u32 contentsize = 0, readsize = 0, size = 0x1000;
	char a[2048];
	string strNew;
	httpcContext context;
	FS::File out;
	outPath = "";

	// Start
	ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 0);
	if (ret != 0)
	{
		// Error: Unable to open context
		return 1;
	}
	ret = httpcAddRequestHeaderField(&context, (char*)"User-Agent", (char*)"MULTIDOWNLOAD++");
	if (ret != 0)
	{
		// Error: Unable to add request header
		return 1;
	}
	ret = httpcSetSSLOpt(&context, 1 << 9);
	if (ret != 0)
	{
		// Error: Unable to set SSL options
		return 1;
	}
	ret = httpcBeginRequest(&context);
	if (ret != 0)
	{
		// Error: Unable to begin request
		return 1;
	}
	ret = httpcGetResponseStatusCodeTimeout(&context, &statuscode, 6000000000);
	if (ret != 0)
	{
		// Error: Wrong protocol/Internet not connected
		httpcCloseContext(&context);
		return 1;
	}
	if ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308))
	{
		char newurl[1024];
		httpcGetResponseHeader(&context, (char*)"Location", newurl, 1024);
		httpcCloseContext(&context);
		return HTTP::downloadFile(outPath, newurl, loca, filename);
	}
	if (statuscode != 200)
		return 1;
	ret = httpcGetResponseHeader(&context, (char*)"Content-Disposition", a, 2048);
	if (ret != 0)
	{
		int pos = url.find_last_of("/");
		strNew = url.substr(pos + 1);
	}
	else
	{
		string contents(a, strlen(a));
		int pos = contents.find("=");
		strNew = contents.substr(pos + 1);
	}
	if (strNew.find_first_of("\"") != string::npos)
	{
		unsigned first = strNew.find_first_of("\"");
		unsigned last = strNew.find_last_of("\"");
		strNew = strNew.substr(first + 1, last - first - 1);
	}
	// Filename is now in strNew
	ret = httpcGetDownloadSizeState(&context, NULL, &contentsize);
	if (ret != 0)
	{
		// Error: could not get the file size
		return 1;
	}
	// File size is now in contentsize

	// New filename
	if (filename != "")
		strNew = filename;

	// Location: "/path/to/file.ext"
	string location = loca + strNew;
	out.openfile(location);
	do
	{
		u8* buf = (u8*)linearAlloc(size);
		memset(buf, 0, size);
		ret = httpcDownloadData(&context, buf, 0x1000, &readsize);
		size += readsize;
		out.writefile((const char*)buf, (size_t)readsize);
		linearFree(buf);
	} while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);
	out.closefile();
	if (ret != 0) {
		httpcCloseContext(&context);
		return 1;
	}

	outPath = location;

	httpcCloseContext(&context);
	return 0;
}

Result HTTP::downloadTextToFile(string url, string path, string filename)
{
	Result ret = 0;
	u32 statuscode = 0;
	u32 contentsize = 0, readsize = 0, size = 0x1000;
	//char a[2048];
	string strNew;
	httpcContext context;
	FS::File out;

	// Start
	ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 0);
	if (ret != 0)
	{
		// Error: Unable to open context
		return 1;
	}
	ret = httpcAddRequestHeaderField(&context, (char*)"User-Agent", (char*)"MULTIDOWNLOAD++");
	if (ret != 0)
	{
		// Error: Unable to add request header
		return 1;
	}
	ret = httpcSetSSLOpt(&context, 1 << 9);
	if (ret != 0)
	{
		// Error: Unable to set SSL options
		return 1;
	}
	ret = httpcBeginRequest(&context);
	if (ret != 0)
	{
		// Error: Unable to begin request
		return 1;
	}
	ret = httpcGetResponseStatusCodeTimeout(&context, &statuscode, 6000000000);
	if (ret != 0)
	{
		// Error: Wrong protocol/Internet not connected
		httpcCloseContext(&context);
		return 1;
	}
	if ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308))
	{
		char newurl[1024];
		httpcGetResponseHeader(&context, (char*)"Location", newurl, 1024);
		httpcCloseContext(&context);
		return HTTP::downloadTextToFile(newurl, path, filename);
	}
	if (statuscode != 200)
		return 1;
	ret = httpcGetDownloadSizeState(&context, NULL, &contentsize);
	if (ret != 0)
	{
		// Error: could not get the file size
		return 1;
	}
	// Location: "/path/to/file.ext"
	string location = path + filename;
	out.openfile(location);
	do
	{
		u8* buf = (u8*)linearAlloc(size);
		memset(buf, 0, size);
		ret = httpcDownloadData(&context, buf, 0x1000, &readsize);
		size += readsize;
		out.writefile((const char*)buf, (size_t)readsize);
		linearFree(buf);
	} while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);
	out.closefile();
	if (ret != 0) {
		httpcCloseContext(&context);
		return 1;
	}

	httpcCloseContext(&context);
	return 0;
}

Result HTTP::downloadText(string& out, string url)
{
	Result ret = 0;
	u32 statuscode = 0;
	u32 contentsize = 0, readsize = 0, size = 0x1000;
	//char a[2048];
	string strNew;
	httpcContext context;
	out = "";

	// Start
	ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 0);
	if (ret != 0)
	{
		// Error: Unable to open context
		return 1;
	}
	ret = httpcAddRequestHeaderField(&context, (char*)"User-Agent", (char*)"PCT-3DS");
	if (ret != 0)
	{
		// Error: Unable to add request header
		return 1;
	}
	ret = httpcSetSSLOpt(&context, 1 << 9);
	if (ret != 0)
	{
		// Error: Unable to set SSL options
		return 1;
	}
	ret = httpcBeginRequest(&context);
	if (ret != 0)
	{
		// Error: Unable to begin request
		return 1;
	}
	ret = httpcGetResponseStatusCodeTimeout(&context, &statuscode, 6000000000);
	if (ret != 0)
	{
		// Error: Wrong protocol/Internet not connected
		httpcCloseContext(&context);
		return 1;
	}
	if ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308))
	{
		char newurl[1024];
		httpcGetResponseHeader(&context, (char*)"Location", newurl, 1024);
		httpcCloseContext(&context);
		return HTTP::downloadText(out, newurl);
	}
	if (statuscode != 200)
		return 1;
	ret = httpcGetDownloadSizeState(&context, NULL, &contentsize);
	if (ret != 0)
	{
		// Error: could not get the total text size
		return 1;
	}
	
	// Save to out
	do
	{
		u8* buf = (u8*)linearAlloc(size);
		memset(buf, 0, size);
		ret = httpcDownloadData(&context, buf, 0x1000, &readsize);
		size += readsize;
		out += (const char*)buf;
		linearFree(buf);
	} while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);

	if (ret != 0) {
		httpcCloseContext(&context);
		return 1;
	}

	httpcCloseContext(&context);
	return 0;
}
