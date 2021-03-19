#include "pch.h"
#include <io.h>
#include "FileSystemHelper.h"

vector<FileSystemHelper::FileInfo> FileSystemHelper::mVecFileInfo = {};
_finddata_t fd;

void FileSystemHelper::FileSearch(wstring filePath, FileType type)
{
	intptr_t handle = NULL;
	string str;
	int check;

	str.assign(filePath.begin(), filePath.end());

	if ((handle = _findfirst(str.c_str(), &fd)) == -1)
		return;

	while (_findnext(handle, &fd) == 0)
	{
		wchar_t* ptr = nullptr;
		wstring strFile = wcstok_s(&filePath[0], L"*", &ptr);

		if (fd.attrib & _A_SUBDIR)
			check = 0;
		else
			check = 1;

		if (check == 0 && fd.name[0] != '.')
		{
			string path = fd.name;
			wstring temp;
			temp.assign(path.begin(), path.end());
			FileSearch(strFile.append(temp).append(L"\\*.*"), type);
		}
		else if (check == 1 && fd.size != 0 && fd.name[0] != '.')
		{
			// 파일 이름 넣기
			FileInfo file;
			string name = fd.name;
			file.FilePath = strFile;
			file.FileName.assign(name.begin(), name.end());
			file.FileType = type;
			mVecFileInfo.push_back(file);
		}
	}

	_findclose(handle);
}

vector<FileSystemHelper::FileInfo> FileSystemHelper::GetAllFile(wstring folderPath, FileType type)
{
	FileSearch(folderPath, type);




	/*intptr_t handle;

	string str;
	str.assign(folderPath.begin(), folderPath.end());

	if ((handle == _findfirst(str.c_str(), &fd)) != -1)
	{
		while (_findnext(handle, &fd) == 0)
		{
			if((fd.attrib & _A_SUBDIR) == 0)
		}
	}*/



	// string to wstring
	// wstr.assign(str.begin(), str.end());


	//WIN32_FIND_DATA fd;
	//HANDLE hFind = FindFirstFile(folderPath.c_str(), &fd);

	//if (hFind == INVALID_HANDLE_VALUE)
	//	return mVecFileInfo;
	//fd.
	//while (FindNextFile(hFind, &fd))
	//{
	//	if (!(fd.dwFileAttributes && FILE_ATTRIBUTE_DIRECTORY))
	//	{
	//		FileInfo file;
	//		file.FilePath = fd.cFileName;
	//		//file.FileName = ;
	//		mVecFileInfo.push_back(file);
	//	}
	//}

	// c++17에서만 가능한데 도저히... byte 오류를 고칠 수 없어따,,,
	/*filesystem::recursive_directory_iterator iter(filesystem::current_path() / "../Resources");
	filesystem::recursive_directory_iterator iter(folderPath);
	while (iter != filesystem::end(iter))
	{
		const filesystem::directory_entry& entry = *iter;
		std::wstring strPath = (std::wstring)entry.path();

		// 폴더일 경우 거르자
		if (entry.is_directory())//(strPath.find(L".bmp") == wstring::npos)
		{
			++iter;
			continue;
		}
	}*/

	return mVecFileInfo;
}
