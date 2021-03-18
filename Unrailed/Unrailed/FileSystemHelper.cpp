#include "pch.h"
//#include "FileSystemHelper.h"
//#include <filesystem>
//
//std::vector<FileInfo> FileSystemHelper::GetAllFile(std::wstring folderPath)
//{
//	//filesystem::recursive_directory_iterator iter(filesystem::current_path() / "../Resources");
//	std::filesystem::recursive_directory_iterator iter(folderPath);
//	while (iter != std::filesystem::end(iter))
//	{
//		const std::filesystem::directory_entry& entry = *iter;
//		std::wstring strPath = (std::wstring)entry.path();
//
//		// 폴더일 경우 거르자
//		if (entry.is_directory())//(strPath.find(L".bmp") == wstring::npos)
//		{
//			++iter;
//			continue;
//		}
//	}
//
//	return mVecFileInfo;
//}
