#include "pch.h"
#include "Sableye.h"

#include "Image.h"

void Sableye::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"Sableye");
	mName = "Sableye";
	mX = WINSIZEX / 2;
	mY = WINSIZEY / 2;
	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Sableye::Release()
{
}

void Sableye::Update()
{
}

void Sableye::Render(HDC hdc)
{
}
