#pragma once
//#include <string>
//#include <vector>

class FileSystemHelper
{
public:
	struct FileInfo
	{
		wstring FilePath;
		wstring FileName;
	};

public:
	static vector<FileInfo> mVecFileInfo;

	static void FileSearch(wstring filePath);
	static vector<FileInfo> GetAllFile(std::wstring folderPath);
};