#include "pch.h"
#include "Ore.h"
#include "Image.h"
#include "Camera.h"

void Ore::Init()
{
}

void Ore::Release()
{
}

void Ore::Update()
{
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Ore::Render(HDC hdc)
{
	if (mCount == 1)
		CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mX, mY, mFrameX, 0, mSizeX, mSizeY);
	else if (mCount == 2)
		CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mX, mY, mFrameX, 1, mSizeX, mSizeY);
	else if (mCount >= 3)
		CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mX, mY, mFrameX, 2, mSizeX, mSizeY);

	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mNumImage, mX + 28, mY + 22, mCount - 1, 0, 8, 16);
}

void Ore::Drop(int x, int y, ItemType type)
{
	//속성정하고
	mImage = IMAGEMANAGER->FindImage(L"Ore");
	mNumImage = IMAGEMANAGER->FindImage(L"Num");
	mType = type;
	mCount = 1;

	if (type == ItemType::Green)
	{
		mFrameX = 1;
	}
	else if (type == ItemType::Blue)
	{
		mFrameX = 2;
	}
	else 
	{
		mType = ItemType::Red;
		mFrameX = 0;
	}

	mX = x + 8;
	mY = y + 8;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
}

ItemType Ore::PickUp()
{
	mIsActive = false;
	return mType;
}

void Ore::Place(int x, int y)
{
	mIsActive = true;
	mX = x;
	mY = y;
}