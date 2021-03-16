#include "pch.h"
#include "Ore.h"
#include "Image.h"

void Ore::Init()
{
		
}

void Ore::Release()
{


}

void Ore::Update()
{

}

void Ore::Render(HDC hdc)
{
	mImage->ScaleFrameRender(hdc, mX, mY, mFrameX, 0, mSizeX, mSizeY);
}

void Ore::Drop(int x, int y, int type)
{
	//속성정하고
	mImage = IMAGEMANAGER->FindImage(L"Ore");

	if (type == 1)
	{
		mType = OreType::Green;
		mFrameX = 1;
	}
	else if (type == 2)
	{
		mType = OreType::Blue;
		mFrameX = 2;
	}
	else
	{
		mType = OreType::Red;
		mFrameX = 0;
	}

	mX = x + 8;
	mY = y + 8;
	mSizeX = mImage->GetFrameWidth()*2.5;
	mSizeY = mImage->GetFrameHeight()*2.5;

}

int Ore::PickUp()
{
	mIsActive = false;
	return (int)(mType) +1;
}

void Ore::Place(int x, int y)
{
	mIsActive = true;
	mX = x;
	mY = y;
}