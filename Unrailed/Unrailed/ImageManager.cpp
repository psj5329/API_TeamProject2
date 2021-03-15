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

Image * ImageManager::FindImage(wstring key)
{
	ImageIter iter = mImageList.find(key);
	if (iter != mImageList.end())
	{
		return iter->second;
	}

	return nullptr;
}
