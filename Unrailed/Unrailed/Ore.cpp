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
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mX, mY, mFrameX, 0, mSizeX, mSizeY);
	//mImage->ScaleFrameRender(hdc, mX, mY, mFrameX, 0, mSizeX, mSizeY);

	// {{ �׽�Ʈ�� // ���� // ��Ʈ�� ��ġ �����ؾ� ��
	wstring strCount = to_wstring(mCount);
	TextOut(hdc, (int)mX, (int)mY, strCount.c_str(), (int)strCount.length());
	// �׽�Ʈ�� // ���� }}
}

void Ore::Drop(int x, int y, ItemType type)
{
	//�Ӽ����ϰ�
	mImage = IMAGEMANAGER->FindImage(L"Ore");
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
	mSizeX = mImage->GetFrameWidth()*2.5;
	mSizeY = mImage->GetFrameHeight()*2.5;
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