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
	UINT width, height;
	int frameX, frameY;

	//filesystem::recursive_directory_iterator iter(filesystem::current_path() / "../Resources");
	//while (iter != filesystem::end(iter))
	//{
	//	const filesystem::directory_entry& entry = *iter;
	//	wstring strPath = (wstring)entry.path();

	//	 폴더일 경우 거르자
	//	if (entry.is_directory())//(strPath.find(L".bmp") == wstring::npos)
	//	{
	//		++iter;
	//		continue;
	//	}
	//	
	//	wstring strName = wcstok_s(&strPath[0], L"/", &ptr);
	//	strName = wcstok_s(nullptr, L"\\", &ptr);

	//	 폴더로 들어갈 경우 여기서 더 잘라야함
	//	if (wcsstr(ptr, L"\\") != NULL)
	//		strName = wcstok_s(nullptr, L"\\", &ptr);
	//	 한번 더 폴더이면?
	//	if (wcsstr(ptr, L"\\") != NULL)
	//		strName = wcstok_s(nullptr, L"\\", &ptr);

	//	// {{ 쌤 d2d 코드에서 가져옴
	//	// 테스트용
	//	IWICImagingFactory* wicFactory;
	//	//디코더 생성
	//	IWICBitmapDecoder* ipDecoder = nullptr;
	//	wicFactory->CreateDecoderFromFilename(strName.c_str(), NULL, GENERIC_READ,
	//		WICDecodeMetadataCacheOnDemand, &ipDecoder);
	//	//디코더에서 프레임얻음
	//	IWICBitmapFrameDecode* ipFrame = nullptr;
	//	ipDecoder->GetFrame(0, &ipFrame);
	//	ipFrame->GetSize(&width, &height);
	//	// }}


	//	 사이즈를 알아왔으니 프레임까지? 없으면 .bmp 떼기
	//	strName = wcstok_s(nullptr, L".", &ptr);

	//	if (wcsstr(ptr, L"\\") != NULL)
	//	{
	//		wstring str = wcstok_s(nullptr, L".", &ptr);
	//		frameX = _wtoi(str.c_str());

	//		str = wcstok_s(nullptr, L".", &ptr);
	//		frameY = _wtoi(str.c_str());

	//		IMAGEMANAGER->LoadFromFile(strName, Resources(strName), width, height, frameX, frameY, true);
	//	}
	//	else
	//		IMAGEMANAGER->LoadFromFile(strName, Resources(strName), width, height, true);
	//	

	//	vecFileName.push_back(strName);
	//	++iter;
	//}
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
