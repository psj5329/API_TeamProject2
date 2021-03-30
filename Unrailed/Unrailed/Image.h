#pragma once
class Image
{
private:
	enum class LoadType : int
	{
		Resource,
		File, 
		Empty,
		End
	};

	struct ImageBuffer
	{
		ULONG registerID;
		HDC hdc; 
		HBITMAP bitmap;
		HBITMAP oldBitmap;
		int width; 
		int height;
		int frameX;
		int frameY;
		int frameWidth;
		int frameHeight;
		LoadType loadType;

		ImageBuffer() :registerID(0), hdc(nullptr), bitmap(nullptr), oldBitmap(nullptr), width(0), height(0),
			frameX(0), frameY(0), frameWidth(0), frameHeight(0), loadType(LoadType::End) {}
	};
private:
	ImageBuffer* mImageBuffer;
	wstring mFileName;
	bool mIsTrans;
	COLORREF mTransColor;
	wstring mKeyName;

	ImageBuffer* mBlendImageBuffer;
	BLENDFUNCTION* mBlendFunc;
public:
	Image();
	~Image();

	void ReleaseBuffer();

public:
	bool CreateEmpty(int width, int height);
	bool LoadFromFile(wstring keyName, wstring fileName, int width, int height, bool isTrans, COLORREF transColor = RGB(255,0,255));
	bool LoadFromFile(wstring keyName, wstring fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor = RGB(255,0,255));

public:
	void Render(HDC hdc, int x, int y);
	void Render(HDC hdc, int x, int y, int tempX, int tempY, int tempWidth, int tempHeight);
	void FrameRender(HDC hdc, int x, int y, int frameX, int frameY);
	void FrameRender(HDC hdc, int x, int y, int frameX1, int frameY1, int frameX2, int frameY2);
	void AlphaRender(HDC hdc,int x, int y, float alpha);
	void AlphaFrameRender(HDC hdc, int x, int y, int frameX, int frameY, float alpha);
	void ScaleRender(HDC hdc, int x, int y, int width, int height);
	void ScaleFrameRender(HDC hdc, int x, int y, int frameX, int frameY, int width, int height);
	void AlphaScaleRender(HDC hdc, int x, int y, int width, int height, float alpha);
	void AlphaScaleFrameRender(HDC hdc, int x, int y, int frameX, int frameY, int width, int height, float alpha);

public:
	inline HDC GetHDC()const { return mImageBuffer->hdc; }
	inline int GetFrameX()const { return mImageBuffer->frameX; }
	inline int GetFrameY()const { return mImageBuffer->frameY; }
	inline int GetFrameWidth()const { return mImageBuffer->frameWidth; }
	inline int GetFrameHeight()const { return mImageBuffer->frameHeight; }
	inline int GetWidth()const { return mImageBuffer->width; }
	inline int GetHeight()const { return mImageBuffer->height; }
	inline wstring GetKeyName()const { return mKeyName; }
};

