#include "pch.h"
#include "Image.h"
#include "MapObject.h"

MapObject::MapObject(class Image* image, float x, float y, float sizeX, float sizeY,
	int frameIndexX, int frameIndexY, int type)
{
	mImage = image;
	mX = x;
	mY = y;
	mSizeX = sizeX;
	mSizeY = sizeY;
	mRect = RectMake(mX, mY, mSizeX, mSizeY);
	mFrameIndexX = frameIndexX;
	mFrameIndexY = frameIndexY;
	mType = (MapObjectType)type;
}

void MapObject::Release()
{


}

void MapObject::Update()
{

}

void MapObject::Render(HDC hdc)
{
	if (mType != MapObjectType::None)
	{
		if (mImage != nullptr)
			mImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
	}
}