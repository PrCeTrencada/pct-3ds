#pragma once
#ifndef FS_HPP
#define FS_HPP

#include <string>
#include <locale>
#include <codecvt>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include <3ds.h>
#include <sys/stat.h>
//#include <sys/statvfs.h>

//#include "../../extlibs/zip/zip.h"
#include <zip/zip.h>

using namespace std;

namespace FS
{
	class File {
	public:
		void openfile(string location);
		void writefile(const char* buffer, size_t size);
		void writefile(string location, string buffer);
		void closefile();
		string readfile(string place);
	private:
		FILE* fp;
	};

	bool createDirectory(const string path);
	bool deleteDirectory(const string path);

	bool deleteFile(const string path);

	bool dirExists(const string& path);

	namespace ZIP
	{
		bool extract(const string zipfile, const string dest);
	}
}

#endif //!FS_HPP
