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

	mActive = true;
}

void MapObject::Release()
{
}

void MapObject::Update()
{
	//광물 부셔지면 아이템 만들어두기?
	if (mHp <= 0 && mActive == true)
	{
		Ore* ore = new Ore();
		ore->Drop((int)mX, (int)mY, mType);
		ObjectManager::GetInstance()->AddObject(ObjectLayer::ITEM, ore);
		mImage = nullptr;
		mType = ItemType::None;
		mActive = false;
	}
}

void MapObject::Render(HDC hdc)
{
	if (mType != ItemType::None)
	{
		if (mImage != nullptr)
			CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, (int)mSizeX, (int)mSizeY);
			//mImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
	}
}

void MapObject::Shake()
{
	
}