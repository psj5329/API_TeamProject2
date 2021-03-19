#pragma once
//#include <string>
//#include <vector>

enum FileType { IMAGE, SOUND, FILETYPEEND };

class FileSystemHelper
{
public:
	struct FileInfo
	{
		wstring FilePath;
		wstring FileName;
		FileType FileType;
	};

public:
	static vector<FileInfo> mVecFileInfo;

	static void FileSearch(wstring filePath, FileType type);
	static vector<FileInfo> GetAllFile(std::wstring folderPath, FileType type);
};