#include "pch.h"
#include "Inven.h"
#include "Image.h"

void Inven::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"InvenUI");
	mSizeX = mImage->GetFrameWidth() * 2.f;
	mSizeY = mImage->GetFrameHeight() * 2.f;
	//mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
}

void Inven::Update()
{
	for (int i = 0; i < mBagItemList.size(); ++i)
	{
		mBagItemList[i]->SetLocation(10, 134 + 30 * i);
		//int x = 
		//int x = ownerX - 24 + 8 + 16 * (i % 2);
		//int y = ownerY - 24 - 16 - 16 * (i / 2);

		//mBagItemList[i]->SetLocation(x, y);
	}
}

void Inven::Render(HDC hdc)
{
	for (int i = 0; i < 6; ++i)
		mImage->AlphaScaleFrameRender(hdc, 4, 128 + 30 * i, 0, 0, mSizeX, mSizeY, 0.5f);
	
	for (int i = 0; i < mBagItemList.size(); ++i)
		mBagItemList[i]->Render(hdc);
}