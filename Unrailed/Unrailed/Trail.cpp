#include "pch.h"
#include "Trail.h"
#include "Image.h"
#include "Animation.h"
#include "Camera.h"
#include "TrailManager.h"

void Trail::Init(int x, int y, int type, int direction)
{
    mTrailType = (TrailType)type;
    mDirection = (TrailDirection)direction;

    //이미지정하기
    if (mTrailType == TrailType::Green)
    {
        mImage = IMAGEMANAGER->FindImage(L"GreenTrail");
    }
    else if (mTrailType == TrailType::Blue)
    {
        mImage = IMAGEMANAGER->FindImage(L"BlueTrail");
    }
    else 
    {
        mImage = IMAGEMANAGER->FindImage(L"RedTrail");
    }
    
        
    mX = x;
    mY = y;
    mSizeX = mImage->GetFrameWidth() * 2;
    mSizeY = mImage->GetFrameHeight() * 2;
    mRect = RectMake(mX, mY, mSizeX, mSizeY);
    mIsConnected = false;
    mIsTail = false;
    mIsPassed = false;
    mOrder = 0;
}

void Trail::Release()
{
    SafeDelete(mUp);
    SafeDelete(mDown);
    SafeDelete(mLeft);
    SafeDelete(mRight);
    SafeDelete(mCurrentAnimation);
}

void Trail::Update()
{

}

void Trail::Render(HDC hdc)
{
    if (mTrailType != TrailType::None)
    {
        if (mIsConnected)
        {
            CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc,mImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
           
            if (INPUT->GetKey('N'))
            {
                wstring str3 = L"No." + to_wstring(mOrder);
                TextOut(hdc,mX + 10, mY + 10, str3.c_str(), str3.length());
            }
        }
        else
        {
            if(mDirection == TrailDirection::Up)
            { 
                CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, 0, 0, mSizeX, mSizeY);
            }
            else if (mDirection == TrailDirection::Down)
            {
                CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, 0, 1, mSizeX, mSizeY);
            }
            else if (mDirection == TrailDirection::Left)
            {
                CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, 0, 3, mSizeX, mSizeY);
            }
            else
            {
                CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, 0, 2, mSizeX, mSizeY);
            }

        }
    }

    
}


//플레이어의 돌리기
void Trail::Turn()
{
	switch (mDirection)
	{
    case TrailDirection::Down:
        mDirection = TrailDirection::Left;
        mCurrentAnimation = mLeft;
        break;
    case TrailDirection::Up:
        mDirection = TrailDirection::Right;
        mCurrentAnimation = mRight;
        break;
    case TrailDirection::Left:
        mDirection = TrailDirection::Up;
        mCurrentAnimation = mUp;
        break;
    case TrailDirection::Right:
        mDirection = TrailDirection::Down;
        mCurrentAnimation = mDown;
        break;
    default:
        break;
	}

}

void Trail::AnimationInit(Animation* down, Animation* up, Animation* left, Animation* right)
{
    mDown = down;

    mUp = up;

    mLeft = left;

    mRight = right;

    if (mDirection == TrailDirection::Down)
    {
        mCurrentAnimation = mDown;
    }
    else if (mDirection == TrailDirection::Up)
    {
        mCurrentAnimation = mUp;
    }
    else if (mDirection == TrailDirection::Left)
    {
        mCurrentAnimation = mLeft;
    }
    else
    {
        mCurrentAnimation = mRight;
    }
    //mCurrentAnimation->Play();

}

int Trail::PickUp()
{
    mIsActive = false;
    mIsDestroy = true;
    return (int)mTrailType;
}

//속성바꾸기
void Trail::SetTrailType(int type) 
{ 
    mTrailType = (TrailType)type; 
    //이미지정하기
    if (mTrailType == TrailType::Green)
    {
        mImage = IMAGEMANAGER->FindImage(L"GreenTrail");
    }
    else if (mTrailType == TrailType::Blue)
    {
        mImage = IMAGEMANAGER->FindImage(L"BlueTrail");
    }
    else
    {
        mImage = IMAGEMANAGER->FindImage(L"RedTrail");
    }
}

//방향바꾸기
void Trail::SetDirection(int dir) 
{ 
    mDirection = (TrailDirection)dir; 

    switch (mDirection)
    {
    case TrailDirection::Down:
        mCurrentAnimation = mDown;
        break;
    case TrailDirection::Up:
        mCurrentAnimation = mUp;
        break;
    case TrailDirection::Left:
        mCurrentAnimation = mLeft;
        break;
    case TrailDirection::Right:
        mCurrentAnimation = mRight;
        break;
    default:
        break;
    }

}