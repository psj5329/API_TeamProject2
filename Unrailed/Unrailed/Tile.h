#pragma once

//구조체 변수는 대문자로 시작하게 짬, 나같은경우에는
enum class TileType : int
{
	Normal, // 모두 갈 수 있는 곳
	Wall, // 아무도 갈 수 없는 곳
	Water, // 리아코만 갈 수 있는 곳
	Lava, // 파이리만 갈 수 있는 곳
	ice // 미끄러지는 곳이지만 보류
};

struct TilePallete
{
	class Image* image;
	int positionX;
	int positionY;
	int width;
	int height;
	RECT rect;
	int frameX;
	int frameY;
	int type;
};

struct MapObjectPallete
{
	class Image* image;
	int positionX;
	int positionY;
	int width;
	int height;
	RECT rect;
	int frameX;
	int frameY;
	int type;
};

struct TypePallete
{
	int positionX;
	int positionY;
	RECT rect;
	int width;
	int height;
	int type;
	TileType tiletype;
	int a;
};

class Tile
{
	class Image* mImage;
	class Image* mCoveImage;
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	RECT mRect;
	int mFrameIndexX;
	int mFrameIndexY;
	float mAlpha;
	float mAlphaRate;

	TileType mTileType;

	int mCoverFrameIndexX;
	int mCoverFrameIndexY;
	float mCurrentFrameTime;	//현재 프레임 감는 시간
	float mFrameUpdateTime;		//프레임 갱신(증가) 해야하는 시간
	int mMaxFrame;

public:
	Tile(class Image* image, class Image* coverImage, float x, float y, float sizeX, float sizeY,
		int frameIndexX, int frameIndexY, int type);

	void Render(HDC hdc);

	//겟
	Image* GetImage()const { return mImage; }
	float GetX()const { return mX; }
	float GetY()const { return mY; }
	float GetSizeX()const { return mSizeX; }
	float GetSizeY()const { return mSizeY; }
	RECT GetRect()const { return mRect; }
	int GetFrameIndexX()const { return mFrameIndexX; }
	int GetFrameIndexY()const { return mFrameIndexY; }
	TileType GetTileType()const { return mTileType; }
	//셋
	void SetImage(Image* image) { mImage = image; }
	void SetX(float x) { mX = x; }
	void SetY(float y) { mY = y; }
	void SetSizeX(float sizeX) { mSizeX = sizeX; }
	void SetSizeY(float sizeY) { mSizeY = sizeY; }
	void SetRect(RECT rect) { mRect = rect; }
	void SetFrameIndexX(int indexX) { mFrameIndexX = indexX; }
	void SetFrameIndexY(int indexY) { mFrameIndexY = indexY; }
	void SetTileType(TileType tiletype) { mTileType = tiletype; }
	void SetCoverImage();


private:
	void UpdateCoverFrame();
};