#include "pch.h"
#include "Hut.h"
#include "Image.h"
#include "Camera.h"

void Hut::Init(float x, float y, HutType type)
{
    SetType(type);
	mX = x;
	mY = y;
    mSizeX = mImage->GetFrameWidth();
    mSizeY = mImage->GetFrameHeight();
    mRect = RectMakeCenter(mX, mY,mSizeX,mSizeY);
}

void Hut::Release()
{

}


void Hut::Update()
{

}

void Hut::Render(HDC hdc)
{
    if(mImage != nullptr && mtype != HutType::None)
     CAMERAMANAGER->GetMainCamera()->ScaleRender(hdc, mImage, mX, mY, mSizeX, mSizeY);
}

void Hut::SetType(HutType type)
{
    mtype = type;
    switch (type)
    {
    case HutType::None:
        mImage = nullptr;
        break;
    case HutType::ChikoritaHut:
        mImage = IMAGEMANAGER->FindImage(L"ChikoritaHut");
        break;
    case HutType::TotodileHut:
        mImage = IMAGEMANAGER->FindImage(L"TotodileHut");
        break;
    case HutType::CharmanderHut:
        mImage = IMAGEMANAGER->FindImage(L"CharmanderHut");
        break;
    case HutType::MachopHut:
        mImage = IMAGEMANAGER->FindImage(L"MachopHut");
        break;
    case HutType::NormalHut:
        mImage = IMAGEMANAGER->FindImage(L"Hut");
        break;
    default:
        break;
    }
}