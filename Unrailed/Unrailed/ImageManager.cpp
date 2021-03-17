#include "pch.h"
#include "ImageManager.h"

#include "Image.h"

Singleton_NULL(ImageManager)

ImageManager::~ImageManager()
{
	ImageIter iter = mImageList.begin();
	for (; iter != mImageList.end(); ++iter)
	{
		SafeDelete(iter->second);
	}
}

void ImageManager::LoadFromFile(wstring key, wstring filePath, int width, int height, bool isTrans, COLORREF transColor)
{
	ImageIter iter = mImageList.find(key);
	if (iter != mImageList.end())
	{
		return;
	}

	Image* newImage = new Image();
	newImage->LoadFromFile(key, filePath, width, height, isTrans, transColor);

	mImageList.insert(make_pair(key, newImage));
}

void ImageManager::LoadFromFile(wstring key, wstring filePath, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	ImageIter iter = mImageList.find(key);
	if (iter != mImageList.end())
	{
		return;
	}

	Image* newImage = new Image();
	newImage->LoadFromFile(key, filePath, width, height,frameX,frameY, isTrans, transColor);

	mImageList.insert(make_pair(key, newImage));
}

void ImageManager::LoadAllBmpFile()
{
	wchar_t* ptr = nullptr;
	vector<wstring> vecFileName;

	filesystem::recursive_directory_iterator iter(filesystem::current_path() / "../Resources");
	while (iter != filesystem::end(iter))
	{
		const filesystem::directory_entry& entry = *iter;
		wstring strPath = (wstring)entry.path();

		// 폴더일 경우 거르자
		if (entry.is_directory())//(strPath.find(L".bmp") == wstring::npos)
		{
			++iter;
			continue;
		}

		wstring strName = wcstok_s(&strPath[0], L"/", &ptr);
		strName = wcstok_s(nullptr, L"\\", &ptr);

		// 폴더로 들어갈 경우 여기서 더 잘라야함
		if (wcsstr(ptr, L"\\") != NULL)
			strName = wcstok_s(nullptr, L"\\", &ptr);
		// 한번 더 폴더이면?
		if (wcsstr(ptr, L"\\") != NULL)
			strName = wcstok_s(nullptr, L"\\", &ptr);

		// 테스트용
		// 스프라이트의 경우 .으로 인덱스 수를 표시해보자
		// 아ㅏㅏ아ㅏㅏㅏㅏ 근데 파일 가로 세로 크기를 못알아오잖아아아ㅏㅏㅏㅏ
		// 안돼애애ㅐ애ㅐ애ㅐㅐㅐ


		// 마지막으로 .bmp 떼자
		strName = wcstok_s(nullptr, L".", &ptr);

		IMAGEMANAGER->LoadFromFile(strName, Resources(strName), 60, 16, 1, 1, true);

		vecFileName.push_back(strName);
		++iter;
	}
}

Image * ImageManager::FindImage(wstring key)
{
	ImageIter iter = mImageList.find(key);
	if (iter != mImageList.end())
	{
		return iter->second;
	}

	return nullptr;
}

wstring ImageManager::GetKeyName(Image* image)
{
	ImageIter iter = mImageList.begin();

	for (; iter != mImageList.end();)
	{
		if (iter->second == image)
			return iter->first;
		else
			iter++;
	}

	return NULL;
}
