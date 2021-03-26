#include "pch.h"
#include "Image.h"
#include "MapObject.h"
#include "Ore.h"
#include "Camera.h"

MapObject::MapObject(class Image* image, float x, float y, float sizeX, float sizeY,
	int frameIndexX, int frameIndexY, ItemType type)
{
	mImage = image;
	mX = x;
	mY = y;
	mSizeX = sizeX;
	mSizeY = sizeY;
	mRect = RectMake((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
	mFrameIndexX = frameIndexX;
	mFrameIndexY = frameIndexY;
	mType = type;
	mHp = 3;
	mTimer = 0;
	mActive = true;
}

void MapObject::Release()
{
}

void MapObject::Update()
{

}

void MapObject::Render(HDC hdc)
{
	if (mType != ItemType::None)
	{
		if (mImage != nullptr)
		{
			//Èçµé±â
			if (mIsShaking)
			{
				if (mTimer >= 0 && mTimer < 42) {
					mTimer++;
					if (mTimer % 6 == 0|| mTimer % 6 == 1|| mTimer % 6 == 2) {
						mX -= 2;
					}
					else {
						mX += 2;
					}
					mRect = RectMake((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
				}
				else
				{
					mTimer = 0;
					mIsShaking = false;
				}
			}


			CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, (int)mSizeX, (int)mSizeY);
		}

	}
}

void MapObject::Shake()
{
	mIsShaking = true;
}

void MapObject::DeductHp()
{
	mHp -= 1; 
	Shake(); 

	if (mHp <= 0)
	{
		Ore* ore = new Ore();
		ore->Drop((int)mX, (int)mY, mType);
		ObjectManager::GetInstance()->AddObject(ObjectLayer::ITEM, ore);
		mImage = nullptr;
		mType = ItemType::None;
		mActive = false;
	}
}