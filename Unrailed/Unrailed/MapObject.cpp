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
	mRect = RectMake(mX, mY, mSizeX, mSizeY);
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
	//���� �μ����� ������ �����α�?
	if (mHp <= 0 && mActive == true)
	{
		Ore* ore = new Ore();
		ore->Drop(mX, mY, mType);
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
			CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
			//mImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
	}
}

void MapObject::Shake()
{
	
}