#include "pch.h"
#include "BagItem.h"

#include "Camera.h"
#include "Image.h"

void BagItem::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"ItemUI");
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
		mFrameIndexY = 0;// 1;
	else if (mItemName == ItemName::ItemTrail)
		mFrameIndexY = 1;// 2;
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