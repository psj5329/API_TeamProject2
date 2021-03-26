#include "pch.h"
#include "BagItem.h"

#include "Camera.h"
#include "Image.h"

void BagItem::Init()
{
	//mCount = 0; // 현재 카운트는 따로 출력은 안하고 있음

	mImage = IMAGEMANAGER->FindImage(L"ItemUI");
}

void BagItem::Init(const wstring& key, int x, int y, int sizeX, int sizeY, int frameIndexX, int frameIndexY)
{
	mImage = IMAGEMANAGER->FindImage(key);
	mX = x;
	mY = y;
	mSizeX = sizeX;
	mSizeY = sizeY;
	mFrameIndexX = frameIndexX;
	mFrameIndexY = frameIndexY;
	//mCount = 0;
}

void BagItem::Init(ItemName name, ItemType type)
{
	mItemName = name;
	mItemType = type;

	mImage = IMAGEMANAGER->FindImage(L"ItemUI");

	mSizeX = mImage->GetFrameWidth() * 2.f;
	mSizeY = mImage->GetFrameHeight() * 2.f;
	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);

	if (mItemType == ItemType::Green)
		mFrameIndexX = 0;
	else if (mItemType == ItemType::Blue)
		mFrameIndexX = 1;
	else if (mItemType == ItemType::Red)
		mFrameIndexX = 2;

	if (mItemName == ItemName::ItemOre)
		mFrameIndexY = 1;
	else if (mItemName == ItemName::ItemTrail)
		mFrameIndexY = 2;

	//mX = x;
	//mY = y;
}

void BagItem::Release()
{
}

void BagItem::Update()
{
}

void BagItem::Render(HDC hdc)
{
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mX, mY, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
}