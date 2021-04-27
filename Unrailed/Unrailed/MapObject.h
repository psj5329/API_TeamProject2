#pragma once

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
	ItemType mType;
	bool mActive;
	int mHp;

	int mTimer; //Èçµé±â¿ë
	bool mIsShaking;

public:
	MapObject(class Image* image, float x, float y, float sizeX, float sizeY,
		int frameIndexX, int frameIndexY, ItemType type);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	//°Ù
	inline Image* GetImage()const { return mImage; }
	inline float GetX()const { return mX; }
	inline float GetY()const { return mY; }
	inline float GetSizeX()const { return mSizeX; }
	inline float GetSizeY()const { return mSizeY; }
	inline RECT GetRect()const { return mRect; }
	inline int GetFrameIndexX()const { return mFrameIndexX; }
	inline int GetFrameIndexY()const { return mFrameIndexY; }
	inline int GetIntType()const { return (int)mType; }
	inline ItemType GetMapObjectType()const { return mType; }
	inline bool GetActive()const { return mActive; }
	inline int GetHp()const { return mHp; }

	//¼Â
	void SetImage(Image* image) { mImage = image; }
	void SetX(float x) { mX = x; }
	void SetY(float y) { mY = y; }
	void SetSizeX(float sizeX) { mSizeX = sizeX; }
	void SetSizeY(float sizeY) { mSizeY = sizeY; }
	void SetRect(RECT rect) { mRect = rect; }
	void SetFrameIndexX(int indexX) { mFrameIndexX = indexX; }
	void SetFrameIndexY(int indexY) { mFrameIndexY = indexY; }
	void SetObjectType(ItemType type) { mType = type; }
	void SetActive(bool isActive) { mActive = isActive; }
  
	void DeductHp();
	int GetHp() { return mHp; }
	void SetHp(int hp) { mHp = hp; }

private:
	void Shake();
};