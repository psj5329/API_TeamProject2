#pragma once
enum class MapObjectType 
{
	None,
	Green,
	Blue,
	Red
};


class MapObject
{
	class Image* mImage;
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	RECT mRect;
	int mFrameIndexX;
	int mFrameIndexY;
	MapObjectType mType;
	bool mActive;

	int mHp;

public:

	MapObject(class Image* image, float x, float y, float sizeX, float sizeY,
		int frameIndexX, int frameIndexY, int type);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	//°Ù
	Image* GetImage()const { return mImage; }
	float GetX()const { return mX; }
	float GetY()const { return mY; }
	float GetSizeX()const { return mSizeX; }
	float GetSizeY()const { return mSizeY; }
	RECT GetRect()const { return mRect; }
	int GetFrameIndexX()const { return mFrameIndexX; }
	int GetFrameIndexY()const { return mFrameIndexY; }
	int GetIntType()const { return (int)mType; }
	MapObjectType GetMapObjectType()const { return mType; }
	bool GetActive()const { return mActive; }

	//¼Â
	void SetImage(Image* image) { mImage = image; }
	void SetX(float x) { mX = x; }
	void SetY(float y) { mY = y; }
	void SetSizeX(float sizeX) { mSizeX = sizeX; }
	void SetSizeY(float sizeY) { mSizeY = sizeY; }
	void SetRect(RECT rect) { mRect = rect; }
	void SetFrameIndexX(int indexX) { mFrameIndexX = indexX; }
	void SetFrameIndexY(int indexY) { mFrameIndexY = indexY; }
	void SetObjectType(int type) { mType = (MapObjectType)type; }
	void SetActive(bool isActive) { mActive = isActive; }


	int GetHp() { return mHp; }
	void SetHp(int hp) { mHp = hp; }
	void DamagedHp() { mHp -= 1; }
};

