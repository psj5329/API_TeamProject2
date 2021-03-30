#include "pch.h"
#include "BagItem.h"

#include "Camera.h"
#include "Image.h"

void BagItem::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"ItemUI");
}

void BagItem::Init(ItemName name, ItemType type, bool isCamRender)
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
		mFrameIndexY = 0;
	else if (mItemName == ItemName::ItemTrail)
		mFrameIndexY = 1;
	
	mIsCamRender = isCamRender;
}

void BagItem::Release()
{
}

void BagItem::Update()
{
}

void BagItem::Render(HDC hdc)
{
	if (mIsCamRender)
		CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mX, mY, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
	else
		mImage->ScaleFrameRender(hdc, mX, mY, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
}