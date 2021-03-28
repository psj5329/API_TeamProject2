#include "pch.h"
#include "Inven.h"
#include "Image.h"

void Inven::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"InvenUI");
	mSizeX = mImage->GetFrameWidth() * 2.f;
	mSizeY = mImage->GetFrameHeight() * 2.f;
	mHiddenItem = 0;
}

void Inven::Update()
{
	for (int i = 0; i < mBagItemList.size(); ++i)
		mBagItemList[i]->SetLocation(10, 134 + 30 * i);
}

void Inven::Render(HDC hdc)
{
	for (int i = 0; i < 6; ++i)
		mImage->AlphaScaleFrameRender(hdc, 4, 128 + 30 * i, 0, 0, mSizeX, mSizeY, 0.5f);

	if (mBagItemList.size() && (mBagItemList[0]->GetName() == ItemName::ItemTrail))
		for (int i = 0; i < 3; ++i)
			mImage->ScaleFrameRender(hdc, 10, 224 + 30 * i, 1, 1, mSizeX, mSizeY);

	if (mHiddenItem) // 0: not find 1: find 2: used
		mImage->AlphaScaleFrameRender(hdc, 4, 308, 1, 0, mSizeX, mSizeY, 0.5f);

	if (mHiddenItem == 1)
		mImage->ScaleFrameRender(hdc, 8, 312, 0, 1, mSizeX, mSizeY);

	for (int i = 0; i < mBagItemList.size(); ++i)
		mBagItemList[i]->Render(hdc);
}