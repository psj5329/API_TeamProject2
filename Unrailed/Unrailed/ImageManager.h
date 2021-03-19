#pragma once

class Image;
class ImageManager
{
	Singleton(ImageManager)
private:
	map<wstring, Image*> mImageList;
	typedef map<wstring, Image*>::iterator ImageIter;
	IWICImagingFactory* wicFactory;
public:
	~ImageManager();

	void LoadFromFile(wstring key, wstring filePath, int width, int height,
		bool isTrans, COLORREF transColor = RGB(255,0,255));
	void LoadFromFile(wstring key, wstring filePath, int width, int height,
		int frameX, int frameY, bool isTrans, COLORREF transColor = RGB(255, 0, 255));
	void LoadAllBmpFile();

	Image* FindImage(wstring key);

public:
	wstring GetKeyName(Image* image);
};

#define IMAGEMANAGER ImageManager::GetInstance()