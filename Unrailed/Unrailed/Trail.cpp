#include "pch.h"
#include "Trail.h"
#include "Image.h"
#include "Animation.h"

void Trail::Init(int x, int y, int type, int direction)
{
    mTrailType = (TrailType)(type - 1);
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

    AnimationInit();
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

}

void Trail::Release()
{

}

void Trail::Update()
{

}

void Trail::Render(HDC hdc)
{
    mImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
}

//플레이어의 돌리기
void Trail::Turn()
{
	switch (mDirection)
	{
    case TrailDirection::Down:
        mDirection = TrailDirection::Left;
        break;
    case TrailDirection::Up:
        mDirection = TrailDirection::Right;
        break;
    case TrailDirection::Left:
        mDirection = TrailDirection::Up;
        break;
    case TrailDirection::Right:
        mDirection = TrailDirection::Down;
        break;
    default:
        break;
	}

}

void Trail::AnimationInit()
{
    mDown = new Animation();
    mDown->InitFrameByStartEnd(0, 1, 3, 1, false);
    mDown->SetIsLoop(true);
    mDown->SetFrameUpdateTime(0.2f);

    mUp = new Animation();
    mUp->InitFrameByStartEnd(0, 0, 3, 0, false);
    mUp->SetIsLoop(true);
    mUp->SetFrameUpdateTime(0.2f);

    mLeft = new Animation();
    mLeft->InitFrameByStartEnd(0, 3, 3, 3, false);
    mLeft->SetIsLoop(true);
    mLeft->SetFrameUpdateTime(0.2f);

    mRight = new Animation();
    mRight->InitFrameByStartEnd(0, 2, 3, 2, false);
    mRight->SetIsLoop(true);
    mRight->SetFrameUpdateTime(0.2f);

}

int Trail::PickUp()
{
    mIsActive = false;
    mIsDestroy = true;
    return (int)mTrailType;
}

void Trail::SetIsConnected()
{

}