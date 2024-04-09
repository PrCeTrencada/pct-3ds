#include "FS.hpp"

void FS::File::openfile(string location)
{
	fp = fopen(location.c_str(), "w+b");
}

void FS::File::writefile(const char* buffer, size_t size)
{
	fwrite(buffer, size, 1, fp);
}
void FS::File::writefile(string location, string buffer)
{
	FS::File::openfile(location);
	FS::File::writefile(buffer.c_str(), buffer.size());
	FS::File::closefile();
}
string FS::File::readfile(string place)
{
	ifstream input_file(place);
	if (!input_file.is_open())
	{
		input_file.close();// check for successful opening
		return "error";
	}

	stringstream buffer;
	buffer << input_file.rdbuf();
	
	
	input_file.close();
	return buffer.str();
}
void FS::File::closefile()
{
	fclose(fp);
}

bool FS::createDirectory(const string path)
{
	FS_Archive archive;
	FSUSER_OpenArchive(&archive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
	
	std::u16string path_u16 = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(path.data());
	
	Result ret = FSUSER_CreateDirectory(archive, fsMakePath(PATH_UTF16, path_u16.c_str()), 0);

	FSUSER_CloseArchive(archive);

	return R_SUCCEEDED(ret);
}

bool FS::deleteDirectory(const string path)
{
	FS_Archive archive;
	FSUSER_OpenArchive(&archive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
	
	std::u16string path_u16 = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(path.data());
	
	Result ret = FSUSER_DeleteDirectoryRecursively(archive, fsMakePath(PATH_UTF16, path_u16.c_str()));

	FSUSER_CloseArchive(archive);

	return R_SUCCEEDED(ret);
}

bool FS::deleteFile(const string path)
{
	FS_Archive archive;
	FSUSER_OpenArchive(&archive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
	
	std::u16string path_u16 = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(path.data());
	
	Result ret = FSUSER_DeleteFile(archive, fsMakePath(PATH_UTF16, path_u16.c_str()));

	FSUSER_CloseArchive(archive);

	return R_SUCCEEDED(ret);
}

// @param path : Path with '/' at the end
bool FS::dirExists(const string& path)
{
	//string modPath = path;
	//modPath += (modPath.back() != '/') ? "/" : "";

	FS_Archive archive;
	FSUSER_OpenArchive(&archive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));

	Handle handle;
	std::u16string path_u16 = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(path.data());

	if (R_FAILED(FSUSER_OpenDirectory(&handle, archive, fsMakePath(PATH_UTF16, path_u16.c_str()))))
		return false;

	if (R_FAILED(FSDIR_Close(handle)))
		return false;

	return true;
}

bool FS::ZIP::extract(const string zipfile, const string dest)
{
	return zip_extract(zipfile.c_str(), dest.c_str(), NULL, NULL) == 0;
}
